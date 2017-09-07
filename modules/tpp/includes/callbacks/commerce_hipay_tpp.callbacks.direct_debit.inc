<?php

/**
 * @file
 * Miscellaneous helper functions.
 */

/**
 * Returns default settings for the Hipay Direct Debit payment method.
 *
 * @see commerce_hipay_tpp_direct_debit_settings_form()
 */
function commerce_hipay_tpp_direct_debit_settings_default() {
  $default_settings = commerce_hipay_tpp_settings_default();

  $default_settings['disclaimer'] = '<p><b>Create new SEPA Direct Debit Mandate</b></p>
<p>By sending this mandate form, you authorize until further notice (A) Sample Corp to send instructions to your bank to debit your account and (B) your bank to debit your account in accordance with the instructions from Sample Corp.</p>
<p>As part of your rights, you are entitled to a refund from your bank under the terms and conditions of your agreement with your bank. A refund must be claimed within 8 weeks starting from the date on which your account was debited.</p>';

  // Payment method does not support 3-D Secure.
  unset($default_settings['3ds']);

  return $default_settings;
}

/**
 * Payment method callback: Direct Debit payment method settings form.
 *
 * @see hook_commerce_payment_method_info()
 */
function commerce_hipay_tpp_direct_debit_settings_form($settings = array()) {
  // Merge default settings into the stored settings array.
  // Settings are stored in multi-level array, so we need recursive merge. And
  // actually replace instead of merge, to make sure the default settings are
  // correctly overriden by custom user settings (array_merge_recursive() would
  // just add new array elements instead of overriding existing ones).
  $settings = array_replace_recursive(commerce_hipay_tpp_direct_debit_settings_default(), $settings);

  $form = commerce_hipay_tpp_settings_form($settings);

  // Direct debits support only "sale" mode, no "authorization" possible.
  $form['operation']['#type'] = 'value';

  // Normally we do not use any offsite form in case of direct debit payments,
  // so there is no point in configuring the redirect type.
  $form['template']['#type'] = 'value';

  $form['disclaimer'] = array(
    '#type' => 'textarea',
    '#title' => t('New mandate disclaimer'),
    '#description' => t('Disclaimer shown to a customer when creating a new direct debit mandate.'),
    '#default_value' => $settings['disclaimer'],
  );

  // Payment method does not support 3-D Secure.
  unset($form['3ds']);
  unset($form['css']);

  // Payment method does not support commerce_cardonfile.
  unset($form['commerce_cardonfile']);

  return $form;
}

/**
 * Payment method callback; generation callback for the payment submission form.
 *
 * @param $payment_method
 *   An array of the current settings.
 * @param $pane_values
 *   The current values of the pane.
 * @param $checkout_pane
 *   The checkout pane array. The checkout pane will be NULL if the payment is
 *   being added through the administration form.
 * @param $order
 *   The order object.
 * @return
 *   A form snippet for the checkout pane.
 */
function commerce_hipay_tpp_direct_debit_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $form = array();

  $conditions = array(
    'uid' => $GLOBALS['user']->uid,
    'debit_agreement_status' => COMMERCE_HIPAY_TPP_DIRECT_DEBIT_STATUS_AVAILABLE,
  );
  $stored_direct_debits = commerce_hipay_tpp_direct_debit_load_multiple(FALSE, $conditions);

  // Build options form.
  $subform = array('#tree' => TRUE);

  if (!empty($stored_direct_debits)) {
    // Get options list with labels.
    $options = commerce_hipat_tpp_direct_debit_element_options_list($stored_direct_debits);
    $default_value = key($options);

    $subform['direct_debit_id'] = array(
      '#type' => 'radios',
      '#title' => t('Select existing direct debit mandate, or create a new one'),
      '#options' => $options,
      '#default_value' => $default_value,
      '#weight' => -10,
      '#ajax' => array(
        'callback' => 'commerce_payment_pane_checkout_form_details_refresh',
        'wrapper' => 'payment-details',
      ),
    );
  }
  else {
    $subform['direct_debit_id'] = array(
      '#type' => 'value',
      '#value' => 'new',
    );
  }

  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $customer_billing_address_wrapper = $order_wrapper->commerce_customer_billing->commerce_customer_address;

  if (
    !empty($pane_values['payment_details']['commerce_payment']['payment_details']['commerce_hipay_tpp_direct_debit']['direct_debit_id'])
    && $pane_values['payment_details']['commerce_payment']['payment_details']['commerce_hipay_tpp_direct_debit']['direct_debit_id'] == 'new'
    || !empty($subform['direct_debit_id']['#value']) && $subform['direct_debit_id']['#value'] == 'new'
  ) {

    if (!empty($payment_method['settings']['disclaimer'])) {
      $subform['disclaimer'] = array(
        '#markup' => $payment_method['settings']['disclaimer'],
      );
    }

    $subform['gender'] = array(
      '#type' => 'radios',
      '#title' => t('Gender'),
      '#options' => array(
        'M' => t('male'),
        'F' => t('female'),
      ),
      '#required' => TRUE,
    );

    $subform['first_name'] = array(
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#default_value' => $customer_billing_address_wrapper->first_name->value(),
      '#required' => TRUE,
    );

    $subform['last_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Last name'),
      '#default_value' => $customer_billing_address_wrapper->last_name->value(),
      '#required' => TRUE,
    );

    $subform['bank_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Bank name'),
      '#required' => TRUE,
    );

    $subform['issuer_bank_id'] = array(
      '#type' => 'textfield',
      '#title' => t('BIC'),
      '#required' => TRUE,
    );

    $subform['iban'] = array(
      '#type' => 'textfield',
      '#title' => t('IBAN'),
      '#required' => TRUE,
    );
  }

  $form['commerce_payment']['payment_details']['commerce_hipay_tpp_direct_debit'] = $subform;
  return $form;
}

/**
 * Payment method callback; validate callback for the payment submission form.
 *
 * @param $payment_method
 *   An array of the current settings.
 * @param $pane_form
 *   The pane form.
 * @param $pane_values
 *   The current values of the pane.
 * @param $order
 *   The order object.
 * @param $form_parents
 *   The identifier of the base element of the payment pane.
 */
function commerce_hipay_tpp_direct_debit_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  if (
    $payment_method['method_id'] != 'commerce_hipay_tpp_direct_debit'
    || empty($pane_values['commerce_payment']['payment_details']['commerce_hipay_tpp_direct_debit'])
  ) {
    drupal_set_message(t('Invalid values provided.'), 'error');
    return FALSE;
  }

  $direct_debit_values = $pane_values['commerce_payment']['payment_details']['commerce_hipay_tpp_direct_debit'];

  if ($direct_debit_values['direct_debit_id'] !== 'new') {
    $conditions = array(
      'uid' => $GLOBALS['user']->uid,
      'debit_agreement_status' => COMMERCE_HIPAY_TPP_DIRECT_DEBIT_STATUS_AVAILABLE,
    );
    $stored_direct_debits = commerce_hipay_tpp_direct_debit_load_multiple(FALSE, $conditions);

    if (!in_array($direct_debit_values['direct_debit_id'], array_keys($stored_direct_debits))) {
      drupal_set_message(t('The selected direct debit mandate is no longer valid.'), 'error');
      return FALSE;
    }
  }
  else {

  }
}

/**
 * Payment method callback; submit callback for the payment submission form.
 *
 * @param $payment_method
 *   An array of the current settings.
 * @param $pane_form
 *   The pane form.
 * @param $pane_values
 *   The current values of the pane.
 * @param $order
 *   The order object.
 * @param $charge
 *   A price structure that needs to be charged.
 */
function commerce_hipay_tpp_direct_debit_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  $direct_debit_values = $pane_values['commerce_payment']['payment_details']['commerce_hipay_tpp_direct_debit'];

  // Use existing direct debit mandate entity.
  if ($direct_debit_values['direct_debit_id'] !== 'new') {
    $direct_debit = commerce_hipay_tpp_direct_debit_load($direct_debit_values['direct_debit_id']);
  }
  // Create new direct debit mandate entity.
  else {
    $direct_debit = commerce_hipay_tpp_direct_debit_new();
    $direct_debit->gender = $direct_debit_values['gender'];
    $direct_debit->first_name = $direct_debit_values['first_name'];
    $direct_debit->last_name = $direct_debit_values['last_name'];
    $direct_debit->bank_name = $direct_debit_values['bank_name'];
    $direct_debit->issuer_bank_id = $direct_debit_values['issuer_bank_id'];
    $direct_debit->iban = $direct_debit_values['iban'];
    $direct_debit->log = t('Created from data provided by customer during order @order_id checkout.', array('@order_id' => $order->order_id));
    commerce_hipay_tpp_direct_debit_save($direct_debit);

    // Create new direct debit mandate in Hipay.
    $response = commerce_hipay_tpp_api_direct_debit_create($direct_debit);

    if (!isset($response['code']) || $response['code'] !== COMMERCE_HIPAY_TPP_REST_STATUS_SUCCESS) {
      watchdog('commerce_hipay_tpp_direct_debit', 'Error creating direct debit mandate @direct_debit_id for order @order_id in Hipay.', array(
        '@direct_debit_id' => $direct_debit->direct_debit_id,
        '@order_id' => $order->order_id,
      ), WATCHDOG_ERROR);
      drupal_set_message(t(COMMERCE_HIPAY_TPP_MESSAGE_PAYMENT_ERROR_GENERAL), 'error');
      return FALSE;
    }
  }

  $order->data['commerce_hipay_tpp_direct_debit'] = $direct_debit->direct_debit_id;
  commerce_order_save($order);

  // Add order id to the direct debit entity.
  $direct_debit_wrapper = entity_metadata_wrapper('commerce_hipay_tpp_direct_debit', $direct_debit);
  $direct_debit_wrapper->direct_debit_order[] = $order->order_id;
  $direct_debit->revision = TRUE;
  $direct_debit->log = t('Used to pay for order @order_id.', array('@order_id' => $order->order_id));
  commerce_hipay_tpp_direct_debit_save($direct_debit);
}

/**
 * Payment method callback; redirect form for the payment submission.
 *
 * @see hook_commerce_payment_method_info()
 * @see commerce_hipay_tpp_redirect_form_validate()
 * @see commerce_hipay_tpp_redirect_form_submit()
 */
function commerce_hipay_tpp_direct_debit_redirect_form($form, &$form_state, $order, $payment_method) {
  return commerce_hipay_tpp_redirect_form($form, $form_state, $order, $payment_method);
}

/**
 * Direct debit callback: background charge payment.
 *
 * @param object $payment_method
 *  The payment method instance definition array.
 * @param object $order
 *   The order object that is being processed
 *
 * @return
 *   TRUE if the transaction was successful
 *
 * @see commerce_hipay_tpp_commerce_payment_method_info()
 */
function commerce_hipay_tpp_direct_debit_charge($payment_method, $order) {
  // By default we will return FALSE, if something doesn't work on the way.
  $return = FALSE;

  // Get generic API request data.
  $request_data = commerce_hipay_tpp_get_request_data($order, $payment_method);

  // Create payment transaction before we call Hipay API.
  $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->amount = commerce_currency_decimal_to_amount($request_data['amount'], $request_data['currency']);
  $transaction->currency_code = $request_data['currency'];
  $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
  $transaction->message = t('New order requested.');
  $transaction->message_variables = array(
    '@operation' => ucfirst($payment_method['settings']['operation']),
  );
  $transaction->data['operation'] = $payment_method['settings']['operation'];
  $transaction->log = t('Created for order @order_id direct debit payment.', array('@order_id' => $order->order_id));
  commerce_payment_transaction_save($transaction);

  $request_data['cdata2'] = $transaction->transaction_id;

  // Allow other modules to alter the $request_data array.
  drupal_alter('commerce_hipay_tpp_api_order_request', $request_data, $order, $payment_method);

  // Add request data to the transaction payload.
  $transaction->payload['order-request-' . time()] = $request_data;
  commerce_payment_transaction_save($transaction);

  // Perform the call to Hipay TPP API.
  $url = commerce_hipay_tpp_get_server_url($payment_method) . COMMERCE_HIPAY_TPP_RESOURCE_ORDER;
  $context = array(
    'call_type' => 'Order request',
    'currency_code' => $transaction->currency_code,
  );
  $response = commerce_hipay_tpp_api_request($url, $request_data, $payment_method, $context, 'commerce_hipay_tpp_direct_debit');

  // Update payment transaction with response details.
  $transaction->payload['order-response-' . time()] = $response;

  // Handle API exceptions.
  if (!empty($response['code'])) {
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->message = t('Error @error_code: @error_message: @error_description');
    $transaction->message_variables = array(
      '@error_code' => $response['code'],
      '@error_message' => $response['message'],
      '@error_description' => $response['description'],
    );
    drupal_set_message(t(COMMERCE_HIPAY_TPP_MESSAGE_PAYMENT_ERROR_GENERAL), 'error');
  }
  else {
    switch ($response['state']) {
      case COMMERCE_HIPAY_TPP_API_STATUS_COMPLETED:
      case COMMERCE_HIPAY_TPP_API_STATUS_PENDING:
        $return = TRUE;
        break;

      default:
        drupal_set_message(t(COMMERCE_HIPAY_TPP_MESSAGE_PAYMENT_ERROR_GENERAL), 'error');
    }
  }

  // Save updated transaction.
  commerce_payment_transaction_save($transaction);

  // Allow other modules to process the order request response.
  foreach (module_implements('commerce_hipay_tpp_api_order_request_response') as $module) {
    $function = $module . '_commerce_hipay_tpp_api_order_request_response';
    $function($response, $transaction, $order, $payment_method);
  }

  return $return;
}

/**
 * Payment method callback: redirect form return validation.
 *
 * @see hook_commerce_payment_method_info()
 * @see commerce_hipay_tpp_redirect_form()
 * @see commerce_hipay_tpp_redirect_form_submit()
 * @see commerce_payment_redirect_pane_checkout_form()
 */
function commerce_hipay_tpp_direct_debit_redirect_form_validate($order, $payment_method) {
  return commerce_hipay_tpp_redirect_form_validate($order, $payment_method);
}

/**
 * Payment method callback: redirect form return submission.
 *
 * @see hook_commerce_payment_method_info()
 * @see commerce_hipay_tpp_redirect_form()
 * @see commerce_hipay_tpp_redirect_form_validate()
 * @see commerce_payment_redirect_pane_checkout_form()
 */
function commerce_hipay_tpp_direct_debit_redirect_form_submit($order, $payment_method) {
  return commerce_hipay_tpp_redirect_form_submit($order, $payment_method);
}

/**
 * Returns an options array for selecting direct debit mandate during checkout.
 *
 * @param $stored_direct_debits
 *   An array of stored direct debit mandate data arrays keyed by
 *   direct_debit_id.
 *
 * @return
 *   An options array for selecting a direct debit mandate.
 */
function commerce_hipat_tpp_direct_debit_element_options_list($stored_direct_debits) {
  $options = array();

  foreach ($stored_direct_debits as $direct_debit_id => $direct_debit) {
    $options[$direct_debit_id] = t('@bank_name, @iban', array(
      '@bank_name' => $direct_debit->bank_name,
      '@iban' => $direct_debit->iban,
    ));
  }
  // Add the special option for adding a new card.
  $options['new'] = t('Create a new direct debit mandate');

  return $options;
}

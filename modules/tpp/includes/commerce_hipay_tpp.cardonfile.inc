<?php

/**
 * @file
 * Provides Card On File features for Commerce Hipay TPP module.
 */

/**
 * Implements hook_commerce_cardonfile_checkout_pane_form_alter().
 *
 * Adds a checkbox to store the credit card for future use.
 *
 * Card on File module adds this checkbox only for onsite payment methods,
 * but in case of Hipay we still want to add it even though it is offsite.
 *
 * @see commerce_cardonfile_form_alter()
 * @see commerce_hipay_tpp_cardonfile_commerce_checkout_form_submit()
 */
function commerce_hipay_tpp_commerce_cardonfile_checkout_pane_form_alter(&$payment_details, &$form) {
  $instance_id = $form['commerce_payment']['payment_method']['#default_value'];
  $payment_method = commerce_payment_method_instance_load($instance_id);

  if ($payment_method['method_id'] == 'commerce_hipay_tpp') {
    // Card on File doesn't add a checkbox to store the credit card for future
    // use for offsite payment methods. In case of Hipay though we still want
    // this checkbox to be added, even though it is offsite.
    $storage = variable_get('commerce_cardonfile_storage', 'opt-in');

    if (in_array($storage, array('opt-in', 'opt-out'))) {
      $payment_details['cardonfile_store'] = array(
        '#type' => 'checkbox',
        '#title' => t('Store this credit card on file for future use.'),
        '#default_value' => $storage == 'opt-out',
      );

      $payment_details['cardonfile_instance_default']['#weight'] = 5;

      // Add submit handler.
      if (isset($form['buttons']['continue'])) {
        $form['buttons']['continue']['#submit'][] = 'commerce_hipay_tpp_cardonfile_commerce_checkout_form_submit';
      }
    }
    else {
      $payment_details['cardonfile_store'] = array(
        '#type' => 'value',
        '#value' => TRUE,
      );
    }
  }
}

/**
 * Checkout form submit callback to process Hipay card on file options.
 *
 * @see commerce_hipay_tpp_commerce_cardonfile_checkout_pane_form_alter()
 */
function commerce_hipay_tpp_cardonfile_commerce_checkout_form_submit($form, &$form_state) {
  // Add selected value for storing the credit card details into the order,
  // as we will need it when processing the server-to-server notification.
  // See commerce_hipay_tpp_process_notification().
  if (isset($form_state['values']['commerce_payment']['payment_details']['cardonfile_store'])) {
    $form_state['order']->data['commerce_hipay_tpp']['cardonfile_store'] = $form_state['values']['commerce_payment']['payment_details']['cardonfile_store'];
    commerce_order_save($form_state['order']);
  }
}

/**
 * Saves new Card on File if required.
 *
 * @param array $feedback
 *   An associative array containing the Hipay API call feedback.
 * @param object $order
 *   An order object being paid for.
 * @param array $payment_method
 *   The payment method instance used for the payment transaction.
 *
 * @see commerce_hipay_tpp_process_notification()
 */
function commerce_hipay_tpp_cardonfile_save($feedback, $order, $payment_method) {
  if (
    module_exists('commerce_cardonfile')
    && !empty($payment_method['settings']['cardonfile'])
    && !empty($feedback['payment_method']['token'])
  ) {
    $existing_cards = commerce_cardonfile_load_multiple_by_uid($order->uid, $payment_method['instance_id']);
    $existing_cards_by_token = entity_key_array_by_property($existing_cards, 'remote_id');

    if (!array_key_exists($feedback['payment_method']['token'], $existing_cards_by_token)) {
      $card_data = commerce_cardonfile_new();
      $card_data->uid = $order->uid;
      $card_data->payment_method = $payment_method['method_id'];
      $card_data->instance_id = $payment_method['instance_id'];
      $card_data->remote_id = $feedback['payment_method']['token'];
      $card_data->card_type = $feedback['payment_method']['brand'];
      $card_data->card_name = $feedback['payment_method']['card_holder'];
      $card_data->card_number = substr($feedback['payment_method']['pan'], -4);
      $card_data->card_exp_year = $feedback['payment_method']['card_expiry_year'];
      $card_data->card_exp_month = $feedback['payment_method']['card_expiry_month'];
      $card_data->status = 1;

      // Save and log the creation of the new card on file.
      commerce_cardonfile_save($card_data);
      watchdog('commerce_hipay_tpp', 'Card on file @card_id created for user @uid.', array(
        '@card_id' => $card_data->card_id,
        '@uid' => $order->uid,
      ));
    }
  }
}

/**
 * Card on file callback: background charge payment
 *
 * @param object $payment_method
 *  The payment method instance definition array.
 * @param object $card_data
 *   The stored credit card data array to be processed
 * @param object $order
 *   The order object that is being processed
 * @param array $charge
 *   The price array for the charge amount with keys of 'amount' and 'currency'
 *   If null the total value of the order is used.
 *
 * @return
 *   TRUE if the transaction was successful
 *
 * @see commerce_hipay_tpp_commerce_payment_method_info()
 */
function commerce_hipay_tpp_cardonfile_charge($payment_method, $card_data, $order, $charge = NULL) {
  // By default we will return FALSE, if something doesn't work on the way.
  $return = FALSE;

  // Get generic API request data.
  // The same function is also used for 'Initialize a new hosted payment page'
  // API resource (see commerce_hipay_tpp_api_initialize()).
  $request_data = commerce_hipay_tpp_get_request_data($order, $payment_method);

  // Add 'recurring' parameters.
  $request_data['payment_product'] = $card_data->card_type;
  $request_data['cardtoken'] = $card_data->remote_id;

  // Recurring e-commerce.
  $request_data['eci'] = COMMERCE_HIPAY_TPP_ECI_RECURRING_ECOMMERCE;

  // If we received specific charge details in the function parameter...
  if (isset($charge)) {
    // Make sure that charge currency is supported by Hipay.
    $charge_converted = commerce_hipay_tpp_get_charge_data($order, $payment_method, $charge);

    // Update request data with converted charge details.
    $request_data['amount'] = number_format(commerce_currency_amount_to_decimal($charge_converted['amount'], $charge_converted['currency_code']), 2, '.', '');
    $request_data['currency'] = $charge_converted['currency_code'];

    // If we were provided with custom charge amount, most probably the 'tax'
    // and 'shipping' amounts we have calculated from order details will be
    // incorrect, so it is safest to get rid of them completely.
    if (isset($request_data['tax'])) {
      unset($request_data['tax']);
    }
    if (isset($request_data['shipping'])) {
      unset($request_data['shipping']);
    }
  }

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
  $response = commerce_hipay_tpp_api_request($url, $request_data, $payment_method, $context);

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
 * Card on file callback: updates the associated customer payment profile.
 *
 * @see commerce_hipay_tpp_commerce_payment_method_info()
 */
function commerce_hipay_tpp_cardonfile_update($form, &$form_state, $payment_method, $card_data) {
  $request_data = array(
    'token' => $card_data->remote_id,
    'card_expiry_month' => $card_data->card_exp_month,
    'card_expiry_year' => $card_data->card_exp_year,
    'card_holder' => substr($card_data->card_name, 0, 25),
  );

  // Perform the call to Hipay TPP API.
  $url = commerce_hipay_tpp_get_secure_vault_url($payment_method) . COMMERCE_HIPAY_TPP_RESOURCE_TOKEN_UPDATE;
  $context = array(
    'call_type' => 'Token update',
    // @TODO: currency_code somehow.
  );
  $response = commerce_hipay_tpp_api_request($url, $request_data, $payment_method, $context);

  // If we received the 'code' parameter in the response it means that something
  // went wrong, and we need to return FALSE. Otherwise the card was updated
  // successfully, so we return TRUE for the local card details to be updated.
  return !isset($response['code']);
}

/**
 * Card on file callback: deletes the associated customer payment profile.
 *
 * Delete operation is not supported by Hipay Secure Vault Tokenization API,
 * but we still want to give user an option to delete a stored card - so we
 * tell cardonfile that deleting it remotely is always successful, so that
 * the local card can be deleted as well.
 *
 * @see commerce_hipay_tpp_commerce_payment_method_info()
 */
function commerce_hipay_tpp_cardonfile_delete($form, &$form_state, $payment_method, $card_data) {
  return TRUE;
}

<?php

/**
 * @file
 * Miscellaneous helper functions.
 */

/**
 * Returns default settings for the Hipay Virtual IBAN payment method.
 *
 * @see commerce_hipay_tpp_virtual_iban_settings_form()
 */
function commerce_hipay_tpp_virtual_iban_settings_default() {
  $default_settings = array(
    'endpoint' => COMMERCE_HIPAY_TPP_TEST,
    'creation_mode' => 'order',
    'expiry_date' => '+1 year',
    'completion_message' => '<p>To complete your order, please transfer <b>[commerce-order:commerce-order-total:amount_decimal] [commerce-order:commerce-order-total:currency_code]</b> to the following bank account:</p>
<p>BIC: <b>[commerce-order:payment-method:virtual-iban:bic]</b>
<br />IBAN: <b>[commerce-order:payment-method:virtual-iban:iban]</b></p>
<p>using reference: <b>[commerce-order:payment-method:virtual-iban:external-reference]</b></p>',
    'api_logging' => array(
      'request' => FALSE,
      'response' => FALSE,
    ),
  );
  foreach (array_keys(commerce_currencies(TRUE)) as $currency_code) {
    $default_settings['accounts'][$currency_code] = array(
      'api_username' => '',
      'api_password' => '',
      'secret_passphrase' => '',
    );
  }
  return $default_settings;
}

/**
 * Payment method callback: Virtual IBAN payment method settings form.
 *
 * @see hook_commerce_payment_method_info()
 */
function commerce_hipay_tpp_virtual_iban_settings_form($settings = array()) {
  // Merge default settings into the stored settings array.
  // Settings are stored in multi-level array, so we need recursive merge. And
  // actually replace instead of merge, to make sure the default settings are
  // correctly overriden by custom user settings (array_merge_recursive() would
  // just add new array elements instead of overriding existing ones).
  $settings = array_replace_recursive(commerce_hipay_tpp_virtual_iban_settings_default(), $settings);

  $default_currency = commerce_default_currency();
  $enabled_currencies = commerce_currencies(TRUE);

  $form = array();

  $form['endpoint'] = array(
    '#type' => 'radios',
    '#title' => t('Environment'),
    '#options' => array(
      COMMERCE_HIPAY_TPP_TEST => t('Test - process test transactions to a test account'),
      COMMERCE_HIPAY_TPP_PRODUCTION => t('Production - process real transactions to a production account'),
    ),
    '#default_value' => $settings['endpoint'],
    '#required' => TRUE,
  );

  foreach ($enabled_currencies as $currency_code => $currency) {
    $form['accounts'][$currency_code] = array(
      '#type' => 'fieldset',
      '#title' => t('@currency_code account', array('@currency_code' => $currency_code)),
      '#collapsible' => TRUE,
      '#collapsed' => !($currency_code == commerce_default_currency()) && empty($settings['accounts'][$currency_code]['api_username']) && empty($settings['accounts'][$currency_code]['api_password']),
      '#group' => 'accounts',
    );

    $form['accounts'][$currency_code]['api_username'] = array(
      '#type' => 'textfield',
      '#title' => t('API username'),
      '#description' => t('The name of the user for accessing Hipay TPP webservice. This, as well as API password, can be found in your Hipay Fullservice Account under <em>Integration / Security Settings</em>.'),
      '#default_value' => $settings['accounts'][$currency_code]['api_username'],
      '#required' => $currency_code == commerce_default_currency(),
    );

    $form['accounts'][$currency_code]['api_password'] = array(
      '#type' => 'textfield',
      '#title' => t('API password'),
      '#description' => t('The password for the user specified in the above field.'),
      '#default_value' => $settings['accounts'][$currency_code]['api_password'],
      '#required' => $currency_code == commerce_default_currency(),
    );

    $form['accounts'][$currency_code]['secret_passphrase'] = array(
      '#type' => 'textfield',
      '#title' => t('Secret Passphrase'),
      '#description' => t('This secret passphrase is used to generate a unique character string (signature) hashed with SHA algorithm. It should be the same as the value provided in <em>Secret Passphrase</em> field in your Hipay Fullservice Account configuration in <em>Integration » Security Settings</em>.'),
      '#default_value' => $settings['accounts'][$currency_code]['secret_passphrase'],
      '#required' => $currency_code == commerce_default_currency(),
    );
  }

  $form['creation_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Creation mode'),
    '#options' => array(
      'order' => t('new Virtual IBAN for each order'),
      'user' => t('one Virtual IBAN for each user'),
    ),
    '#default_value' => $settings['creation_mode'],
    '#required' => TRUE,
  );

  $form['expiry_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiration date'),
    '#description' => t('<a href="@relative_url">Relative format</a> parsable by <a href="@strtotime_url"><code>strtotime()</code></a>, for example: <em>+1 year</em>', array(
      '@relative_url' => url('http://php.net/manual/datetime.formats.relative.php'),
      '@strtotime_url' => url('http://php.net/manual/function.strtotime.php')
    )),
    '#default_value' => $settings['expiry_date'],
    '#required' => TRUE,
  );

  $form['completion_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Completion message'),
    '#description' => t('Message to be displayed on the completion step of the checkout process.'),
    '#default_value' => $settings['completion_message'],
  );

  $form['api_logging'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Log the following messages for debugging'),
    '#description' => t('Basic transaction-related request and response data will be saved in transaction <em>payload</em> property regardless of this setting. Enabling this option will save additional information on each transaction request and response in the dblog, and save cURL verbose information to @curllog.', array('@curllog' => file_directory_temp() . '/curl.log')),
    '#options' => array(
      'request' => t('API request messages'),
      'response' => t('API response messages'),
    ),
    '#default_value' => $settings['api_logging'],
  );

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
function commerce_hipay_tpp_virtual_iban_submit_form($payment_method, $pane_values, $checkout_pane, $order) {

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
function commerce_hipay_tpp_virtual_iban_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  // If order points to already existing Virtual IBAN entity, try to load it.
  if (!empty($order->data['commerce_hipay_tpp_virtual_iban_id'])) {
    if (!$virtual_iban = commerce_hipay_tpp_virtual_iban_load($order->data['commerce_hipay_tpp_virtual_iban_id'])) {
      watchdog('commerce_hipay_tpp', 'Unable to load Virtual IBAN @virtual_iban_id referenced by order @order_id.', array(
        '@virtual_iban_id' => $order->data['commerce_hipay_tpp_virtual_iban_id'],
        '@order_id' => $order->order_id,
      ), WATCHDOG_ERROR);
      drupal_set_message(t(COMMERCE_HIPAY_TPP_MESSAGE_PAYMENT_ERROR_GENERAL), 'error');
      return FALSE;
    }
  }

  // External reference depends on whether we create a new Virtual IBAN
  // for each order, or re-use one Virtual IBAN for each user.
  $external_reference = ($payment_method['settings']['creation_mode'] == 'user') ? 'USER-' . $order->uid : 'ORDER-' . $order->order_id;

  // Try to load an existing non-expired Virtual IBAN entity
  // by the "external_reference" value.
  if (empty($virtual_iban)) {
    $query = new EntityFieldQuery();
    $result = $query->entityCondition('entity_type', 'commerce_hipay_tpp_virtual_iban')
      ->propertyCondition('external_reference', $external_reference)
      ->propertyCondition('expires', time(), '>')
      ->execute();
    if (!empty($result['commerce_hipay_tpp_virtual_iban'])) {
      $virtual_iban_ids = array_keys($result['commerce_hipay_tpp_virtual_iban']);
      $virtual_iban = entity_load_single('commerce_hipay_tpp_virtual_iban', reset($virtual_iban_ids));
    }
  }

  // If at any point above we succeeded with loading an existing Virtual IBAN
  // entity, let's also verify that it exists in Hipay.
  if (!empty($virtual_iban)) {
    $response = commerce_hipay_tpp_api_virtual_iban_search($virtual_iban->external_reference);
    if (!isset($response['code']) || $response['code'] != COMMERCE_HIPAY_TPP_REST_STATUS_SUCCESS || empty($response['virtual_ibans'])) {
      watchdog('commerce_hipay_tpp', 'Unable to find Virtual IBAN @virtual_iban_id (%external_reference) for order @order_id in Hipay.', array(
        '@virtual_iban_id' => $virtual_iban->virtual_iban_id,
        '%external_reference' => $virtual_iban->external_reference,
        '@order_id' => $order->order_id,
      ), WATCHDOG_ERROR);
      drupal_set_message(t(COMMERCE_HIPAY_TPP_MESSAGE_PAYMENT_ERROR_GENERAL), 'error');
      return FALSE;
    }
  }

  // If loading of existing Virtual IBAN entity failed, or order doesn't point
  // to an already existing Virtual IBAN entity, create a new one.
  if (empty($virtual_iban)) {
    $virtual_iban = commerce_hipay_tpp_virtual_iban_new(array(
      'external_reference' => $external_reference,
      'expires' => strtotime($payment_method['settings']['expiry_date']),
    ));
  }

  $virtual_iban_wrapper = entity_metadata_wrapper('commerce_hipay_tpp_virtual_iban', $virtual_iban);
  $virtual_iban_wrapper->virtual_iban_order[] = $order->order_id;
  $virtual_iban_wrapper->save();

  $order->data['commerce_hipay_tpp_virtual_iban_id'] = $virtual_iban->virtual_iban_id;
  commerce_order_save($order);

  if (empty($virtual_iban->bic) || empty($virtual_iban->iban)) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $order_total = $order_wrapper->commerce_order_total->value();
    $context = array(
      'commerce_order' => $order,
      'currency_code' => $order_total['currency_code'],
    );

    $response = commerce_hipay_tpp_api_virtual_iban_create($virtual_iban, $payment_method, $context);

    if (!isset($response['code']) || $response['code'] != COMMERCE_HIPAY_TPP_REST_STATUS_SUCCESS || empty($response['virtual_iban'])) {
      drupal_set_message(t(COMMERCE_HIPAY_TPP_MESSAGE_PAYMENT_ERROR_GENERAL), 'error');
      return FALSE;
    }

    commerce_order_save($order);
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
function commerce_hipay_tpp_virtual_iban_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {

}

/**
 * Implements hook_commerce_checkout_pane_info_alter().
 */
function commerce_hipay_tpp_commerce_checkout_pane_info_alter(&$checkout_panes) {
  if (($order_id = arg(1)) && ($order = commerce_order_load($order_id))) {
    if (!empty($order->data['payment_method']) && $order->data['payment_method'] == 'commerce_hipay_tpp_virtual_iban|commerce_payment_commerce_hipay_tpp_virtual_iban') {
      $checkout_panes['checkout_completion_message']['callbacks']['checkout_form'] = 'commerce_hipay_tpp_virtual_iban_completion_message_pane_checkout_form';
    }
  }
}

/**
 * Checkout pane callback: Wraps the completion message with a custom div.
 */
function commerce_hipay_tpp_virtual_iban_completion_message_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  // Build the original message pane.
  $pane_form = commerce_checkout_completion_message_pane_checkout_form($form, $form_state, $checkout_pane, $order);

  if ($order->data['payment_method'] == 'commerce_hipay_tpp_virtual_iban|commerce_payment_commerce_hipay_tpp_virtual_iban') {
    $payment_method_instance = commerce_payment_method_instance_load('commerce_hipay_tpp_virtual_iban|commerce_payment_commerce_hipay_tpp_virtual_iban');

    // Add and extra div.
    $pane_form['commerce_hipay_tpp_virtual_iban'] = array(
      '#markup' => token_replace($payment_method_instance['settings']['completion_message'], array(
        'commerce-order' => $order,
      )),
      '#weight' => -10,
    );
  }

  return $pane_form;
}

/**
 * Processes the Virtual IBAN feedback returned by Hipay API.
 *
 * @param array $feedback
 *   An associative array containing the Hipay API call feedback.
 *
 * @return bool
 *   Boolean indicating whether the feedback was processed successfully.
 *
 * @see commerce_hipay_tpp_callback_notification()
 */
function commerce_hipay_tpp_process_sct_notification($feedback) {
  // Make sure that we have actually received the feedback from the gateway.
  if (empty($feedback) || !is_array($feedback)) {
    watchdog('commerce_hipay_tpp_virtual_iban', 'SCT notification: Missing or invalid feedback received in notification: !feedback', array(
      '!feedback' => '<pre>' . var_export($feedback, TRUE) . '</pre>',
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Load Virtual IBAN payment method instance.
  $payment_method_id = 'commerce_hipay_tpp_virtual_iban';
  $payment_method_instances = commerce_hipay_tpp_payment_method_instances($payment_method_id);
  $payment_method_instance = reset($payment_method_instances);

  // Log notification feedback if response logging is enabled.
  if (!empty($payment_method_instance['settings']['api_logging']['response'])) {
    watchdog('commerce_hipay_tpp_virtual_iban', 'SCT notification: !feedback', array(
      '!feedback' => '<pre>' . var_export($feedback, TRUE) . '</pre>',
    ), WATCHDOG_DEBUG);
  }

  // Validate the feedback signature.
  if (!commerce_hipay_tpp_feedback_validate_signature($feedback, $payment_method_instance)) {
    watchdog('commerce_hipay_tpp_virtual_iban', 'SCT notification: Signature hashes do not match.', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  // Validate we have received Virtual IBAN details in the notification.
  // Otherwise we won't be able to find the local order.
  if (
    empty($feedback['payment_method']['bic_creditor'])
    || empty($feedback['payment_method']['iban_creditor'])
    || empty($feedback['payment_method']['external_reference'])
  ) {
    watchdog('commerce_hipay_tpp_virtual_iban', 'SCT notification: Missing Virtual IBAN details.', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  // Try to load local Virtual IBAN entities matching notification details.
  $conditions = array(
    'bic' => $feedback['payment_method']['bic_creditor'],
    'iban' => $feedback['payment_method']['iban_creditor'],
    'external_reference' => $feedback['payment_method']['external_reference'],
  );
  // If no local Virtual IBAN entities were found.
  if (!$virtual_ibans = entity_load('commerce_hipay_tpp_virtual_iban', FALSE, $conditions)) {
    watchdog('commerce_hipay_tpp_virtual_iban', 'SCT notification: Unable to find any Virtual IBAN entities matching notification details.', array(), WATCHDOG_ERROR);
    return FALSE;
  }
  // If more than one local Virtual IBAN entity was found.
  if (count($virtual_ibans) != 1) {
    watchdog('commerce_hipay_tpp_virtual_iban', 'SCT notification: Error loading Virtual IBAN entities matching notification details - expected 1, found @count.', array(
      '@count' => count($virtual_ibans),
    ), WATCHDOG_ERROR);
    return FALSE;
  }
  // Otherwise all is fine, we found just one local Virtual IBAN entity.
  $virtual_iban = reset($virtual_ibans);

  // Try to load local payment transaction entity from notification's
  // "transaction_reference" value. This should work for all but first
  // notifications.
  if ($transactions = commerce_payment_transaction_load_multiple(array(), array('remote_id' => $feedback['transaction_reference']))) {
    $transaction = reset($transactions);

    // Make sure the Virtual IBAN details saved in the payment transaction
    // match those received in the notification.
    if (
      empty($transaction->data['commerce_hipay_tpp_virtual_iban_id'])
      || $transaction->data['commerce_hipay_tpp_virtual_iban_id'] != $virtual_iban->virtual_iban_id
      || empty($transaction->data['commerce_hipay_tpp_virtual_iban_external_reference'])
      || $transaction->data['commerce_hipay_tpp_virtual_iban_external_reference'] != $virtual_iban->external_reference
    ) {
      watchdog('commerce_hipay_tpp_virtual_iban', 'SCT notification: Mismatch between payment transaction @transaction_id and Virtual IBAN @virtual_iban_id data.', array(
        '@transaction_id' => $transaction->transaction_id,
        '@virtual_iban_id' => $virtual_iban->virtual_iban_id,
      ), WATCHDOG_ERROR);
      return FALSE;
    }

    // Make sure the payment transaction amount and currency code match those
    // received in the notification.
    if (
      $transaction->amount != commerce_currency_decimal_to_amount($feedback['order']['amount'], $feedback['order']['currency'])
      || $transaction->currency_code != $feedback['order']['currency']
    ) {
      watchdog('commerce_hipay_tpp_virtual_iban', 'SCT notification: Mismatch between transaction @transaction_id and notification amount data.', array(
        '@transaction_id' => $transaction->transaction_id,
      ), WATCHDOG_ERROR);
      return FALSE;
    }

    // Make sure the relevant order has not yet been paid for.
    if (!empty($transaction->order_id)) {
      $order = commerce_order_load($transaction->order_id);
      if (!empty($order->data['commerce_payment_order_paid_in_full_invoked'])) {
        watchdog('commerce_hipay_tpp_virtual_iban', 'SCT notification: Matched order @order_id has already been paid in full.', array(
          '@order_id' => $order->order_id,
        ), WATCHDOG_ERROR);
        return FALSE;
      }
    }
  }

  if (empty($order)) {
    // Get IDs of all orders this Virtual IBAN was used to pay for.
    $virtual_iban_wrapper = entity_metadata_wrapper('commerce_hipay_tpp_virtual_iban', $virtual_iban);
    $vi_order_ids = $virtual_iban_wrapper->virtual_iban_order->raw();
    sort($vi_order_ids);

    // Load all orders this Virtual IBAN was used to pay for.
    $vi_orders = entity_load('commerce_order', $vi_order_ids);
    // Filter out those that were already fully paid.
    foreach ($vi_orders as $vi_order_id => $vi_order) {
      if (
        empty($vi_order->data['payment_method'])
        || (strpos($vi_order->data['payment_method'], 'commerce_hipay_tpp_virtual_iban') === FALSE)
        || !empty($vi_order->data['commerce_payment_order_paid_in_full_invoked'])
      ) {
        unset($vi_orders[$vi_order_id]);
      }
    }

    // Try to find an order for this notification.
    // First try to match order from Virtual IBAN's "external_reference" value.
    if (strpos($virtual_iban->external_reference, 'ORDER-') === 0) {
      list(, $external_reference_order_id) = explode('-', $virtual_iban->external_reference);
      if (
        in_array($external_reference_order_id, array_keys($vi_orders))
        // Also make sure that received amount matches order's amount.
        && commerce_hipay_tpp_virtual_iban_test_order_amount($vi_orders[$external_reference_order_id], $feedback['order']['amount'], $feedback['order']['currency'])
      ) {
        $order = $vi_orders[$external_reference_order_id];
      }
    }
  }

  // If we couldn't match order from Virtual IBAN's "external_reference" value,
  // try to match it based on the transaction amount. To do this loop through
  // all unpaid-for orders and check if its total amount matches notification.
  if (empty($order)) {
    foreach ($vi_orders as $vi_order_id => $vi_order) {
      if (commerce_hipay_tpp_virtual_iban_test_order_amount($vi_order, $feedback['order']['amount'], $feedback['order']['currency'])) {
        $order = $vi_order;
        break;
      }
    }
  }

  if (empty($order)) {
    watchdog('commerce_hipay_tpp_virtual_iban', 'SCT notification: Unable to find any order matching notification details.', array(), WATCHDOG_ERROR);
    // To avoid "undefined variable" PHP notice when invoking hooks at the end.
    $order = NULL;
  }

  // Create or update order's payment transaction.
  // Try to find an existing payment transaction for this notification's
  // "transaction_reference" value.
  if ($transactions = commerce_payment_transaction_load_multiple(array(), array('remote_id' => $feedback['transaction_reference']))) {
    $transaction = reset($transactions);

    // When updating an existing transaction, always create a new revision.
    $transaction->revision = TRUE;
    $transaction->log = t('Updated from Hipay SCT notification.');
  }
  // If no existing payment transaction was found, create a new one.
  else {
    $transaction_order_id = !empty($order) ? $order->order_id : 0;
    $transaction = commerce_payment_transaction_new($payment_method_id, $transaction_order_id);
    $transaction->instance_id = $payment_method_instance['instance_id'];
    $transaction->remote_id = $feedback['transaction_reference'];
    $transaction->amount = commerce_currency_decimal_to_amount($feedback['order']['amount'], $feedback['order']['currency']);
    $transaction->currency_code = $feedback['order']['currency'];
    $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
    $transaction->remote_status = $feedback['status'];
    $transaction->data['operation'] = COMMERCE_HIPAY_TPP_OPERATION_SALE;
    $transaction->data['commerce_hipay_tpp_virtual_iban_id'] = $virtual_iban->virtual_iban_id;
    $transaction->data['commerce_hipay_tpp_virtual_iban_external_reference'] = $virtual_iban->external_reference;
    $transaction->log = t('Created from Hipay SCT notification.');
    commerce_payment_transaction_save($transaction);
  }

  $transaction->payload['Notification-' . time()] = $feedback;

  // Update the payment transaction with notification data.
  $transaction->data['authorized_amount'] = commerce_currency_decimal_to_amount($feedback['authorized_amount'], $feedback['currency']);
  $transaction->data['captured_amount'] = commerce_currency_decimal_to_amount($feedback['captured_amount'], $feedback['currency']);
  $transaction->data['refunded_amount'] = commerce_currency_decimal_to_amount($feedback['refunded_amount'], $feedback['currency']);

  // We do not want to change the main transaction status from one of 'captured'
  // statuses to one of 'authorized' statuses (which might happen when we start
  // with a 'sale' transaction, and Hipay might send us first notification for
  // 'capture requested', and only after that for 'authorized'.
  if (!(
    in_array($transaction->remote_status, commerce_hipay_tpp_get_remote_statuses_for_operation_type(COMMERCE_HIPAY_TPP_OPERATION_CAPTURE))
    && in_array($feedback['status'], commerce_hipay_tpp_get_remote_statuses_for_operation_type(COMMERCE_HIPAY_TPP_OPERATION_AUTHORIZATION))
  )) {
    $transaction->remote_status = $feedback['status'];
  }

  // We want to update the main transaction status only for the very first call,
  // which means only when the main transaction still has its 'pending' status.
  if ($transaction->status == COMMERCE_PAYMENT_STATUS_PENDING) {
    switch ($feedback['state']) {
      case 'completed':
        if (in_array($feedback['status'], commerce_hipay_tpp_get_remote_success_statuses_for_operation_type(COMMERCE_HIPAY_TPP_OPERATION_CAPTURE))) {
          $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;

          if (!empty($order)) {
            $order_balance = commerce_payment_order_balance($order);
            if ($order_balance['amount'] <= 0) {
              $order = commerce_order_status_update($order, 'pending', FALSE, TRUE, t('Customer successfully paid for the order externally via Virtual IBAN transfer.'));
            }
          }
        }
        break;

      case 'declined':
      case 'error':
      default:
        $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
        break;
    }
  }

  // Let's put status message in transaction's message_variables parameter,
  // as this way it will be easier to update it later on transaction refresh.
  $transaction->message = t('Last operation: @status_message');
  $transaction->message_variables = array(
    '@status_message' => commerce_hipay_tpp_get_payment_status_message($feedback['status']),
  );

  if (!empty($feedback['reason']['code'])) {
    $message = commerce_hipay_tpp_get_error_message($feedback['reason']['code']);
    $transaction->message .= '<br />' . t('Error @error_code: @error_message');
    $transaction->message_variables['@error_code'] = $feedback['reason']['code'];
    $transaction->message_variables['@error_message'] = (!empty($message)) ? $message : $feedback['reason']['message'];
  }

  // Authorized amount.
  if ($feedback['authorized_amount'] != 0) {
    $transaction->message .= '<br />' . t('Authorized amount: @authorized_amount');
    $transaction->message_variables['@authorized_amount'] = commerce_currency_format(commerce_currency_decimal_to_amount($feedback['authorized_amount'], $feedback['currency']), $feedback['currency']);
  }

  // Captured amount.
  if ($feedback['captured_amount'] != 0) {
    $transaction->message .= '<br />' . t('Total captured amount: @captured_amount');
    $transaction->message_variables['@captured_amount'] = commerce_currency_format(commerce_currency_decimal_to_amount($feedback['captured_amount'], $feedback['currency']), $feedback['currency']);
  }

  // Refunded amount.
  if ($feedback['refunded_amount'] != 0) {
    $transaction->message .= '<br />' . t('Total refunded amount: @refunded_amount');
    $transaction->message_variables['@refunded_amount'] = commerce_currency_format(commerce_currency_decimal_to_amount($feedback['refunded_amount'], $feedback['currency']), $feedback['currency']);
  }

  commerce_payment_transaction_save($transaction);

  $result = TRUE;

  // Allow other modules to process the notification.
  foreach (module_implements('commerce_hipay_tpp_process_sct_notification') as $module) {
    $function = $module . '_commerce_hipay_tpp_process_sct_notification';
    $result = $function($feedback, $transaction, $order, $result) && $result;
  }

  return $result;
}

/**
 * @param $order
 * @param $notification_amount
 * @param $notification_currency_code
 * @return bool
 */
function commerce_hipay_tpp_virtual_iban_test_order_amount($order, $notification_amount, $notification_currency_code) {
  $order_balance = commerce_payment_order_balance($order);
  $notification_amount = commerce_currency_decimal_to_amount($notification_amount, $notification_currency_code);

  return $notification_amount == $order_balance['amount'] && $notification_currency_code == $order_balance['currency_code'];
}

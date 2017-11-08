<?php

/**
 * @file
 * Miscellaneous helper functions.
 */

/**
 * Returns default settings for the Hipay TPP payment method.
 *
 * @see commerce_hipay_tpp_settings_form()
 */
function commerce_hipay_tpp_settings_default() {
  $default_settings = array(
    'endpoint' => COMMERCE_HIPAY_TPP_TEST,
    'operation' => COMMERCE_HIPAY_TPP_OPERATION_SALE,
    'supported_currencies' => drupal_map_assoc(array_keys(commerce_hipay_get_enabled_currencies())),
    'currency_code' => commerce_default_currency(),
    'template' => COMMERCE_HIPAY_TPP_TEMPLATE_BASIC,
    'language' => 'en_GB',
    '3ds' => 1,
    'cardonfile' => FALSE,
    'css' => '',
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
 * Payment method callback: settings form.
 *
 * @see hook_commerce_payment_method_info()
 */
function commerce_hipay_tpp_settings_form($settings = array()) {
  // Merge default settings into the stored settings array.
  // Settings are stored in multi-level array, so we need recursive merge. And
  // actually replace instead of merge, to make sure the default settings are
  // correctly overriden by custom user settings (array_merge_recursive() would
  // just add new array elements instead of overriding existing ones).
  $settings = array_replace_recursive(commerce_hipay_tpp_settings_default(), $settings);

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

  $form['operation'] = array(
    '#type' => 'radios',
    '#title' => t('Default transaction type'),
    '#options' => array(
      COMMERCE_HIPAY_TPP_OPERATION_SALE => t('Sale - indicates transaction is sent for authorization, and if approved, is automatically submitted for capture'),
      COMMERCE_HIPAY_TPP_OPERATION_AUTHORIZATION => t('Authorization - indicates this transaction is sent for authorization only. The transaction will not be sent for settlement until the transaction is submitted for capture manually by the Merchant'),
    ),
    '#default_value' => $settings['operation'],
    '#required' => TRUE,
  );

  $form['supported_currencies'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Supported currencies'),
    '#description' => t('Transactions in these currencies will be sent as-is to the gateway, without any prior conversion. This setting should reflect your <em>Settlement currencies</em> configuration of your Hipay Fullservice account.'),
    '#options' => commerce_hipay_get_enabled_currencies(),
    '#multiple' => TRUE,
    '#default_value' => $settings['supported_currencies'],
    '#required' => TRUE,
  );

  $form['currency_code'] = array(
    '#type' => 'select',
    '#title' => t('Default currency'),
    '#description' => t('Transactions in other currencies will be converted to this currency, so multi-currency sites must be configured to use appropriate conversion rates.'),
    '#options' => commerce_hipay_get_enabled_currencies(),
    '#default_value' => $settings['currency_code'],
  );

  $form['template'] = array(
    '#type' => 'radios',
    '#title' => t('Checkout redirect mode'),
    '#options' => array(
      COMMERCE_HIPAY_TPP_TEMPLATE_BASIC => t('Redirect to the hosted checkout page through an automatically submitted form'),
      COMMERCE_HIPAY_TPP_TEMPLATE_IFRAME => t('Stay on this site using an iframe to embed the hosted checkout page'),
    ),
    '#default_value' => $settings['template'],
    '#required' => TRUE,
  );

  $form['language'] = array(
    '#type' => 'select',
    '#title' => t('Default language'),
    '#description' => t("Language to be used by the off-site payment page or iframe if user's language is not supported by the gateway."),
    '#options' => commerce_hipay_get_supported_languages(),
    '#default_value' => $settings['language'],
  );

  $form['3ds'] = array(
    '#type' => 'radios',
    '#title' => t('Should the 3-D Secure authentication be performed for payment transactions'),
    '#options' => array(
      0 => t('Bypass 3-D Secure authentication.'),
      1 => t('3-D Secure authentication if available.'),
      2 => t('3-D Secure authentication mandatory.'),
    ),
    '#default_value' => $settings['3ds'],
    '#required' => TRUE,
  );

  if (module_exists('commerce_cardonfile')) {
    $form['cardonfile'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Card on File functionality with this payment method.'),
      '#description' => t('Stores tokenized value for card data.'),
      '#default_value' => $settings['cardonfile'],
    );
  }
  else {
    $form['cardonfile'] = array(
      '#type' => 'markup',
      '#markup' => t('To enable Card on File functionality download and install the <a href="@cardonfile_url">Card on File</a> module.', array(
        '@cardonfile_url' => url('https://www.drupal.org/project/commerce_cardonfile'),
      )),
    );
  }

  $form['css'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment page style sheet'),
    '#description' => t('Path and filename of the custom style sheet for the hosted payment page, relative to Drupal webroot.')
      . '<br />' . t('Note that Hipay requires HTTPS protocol, and the URL generated will reflect this requirement - make sure that your server configuration supports it.'),
    '#default_value' => $settings['css'],
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
    '#weight' => 10,
  );

  return $form;
}

function commerce_hipay_tpp_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  if (!empty($pane_values['cardonfile']) && $pane_values['cardonfile'] != 'new') {
    // First attempt to load the card on file.
    $card_data = commerce_cardonfile_load($pane_values['cardonfile']);

    // Fail now if it is no longer available or the card is inactive.
    if (empty($card_data) || $card_data->status == 0) {
      drupal_set_message(t('The requested card on file is no longer valid.'), 'error');
      return FALSE;
    }

    $order->data['cardonfile'] = $card_data;
    commerce_order_save($order);
  }
}

/**
 * Payment method callback: redirect form.
 *
 * @see hook_commerce_payment_method_info()
 * @see commerce_hipay_tpp_redirect_form_validate()
 * @see commerce_hipay_tpp_redirect_form_submit()
 */
function commerce_hipay_tpp_redirect_form($form, &$form_state, $order, $payment_method) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_total = $order_wrapper->commerce_order_total->value();
  $api_credentials = commerce_hipay_tpp_get_api_credentials($payment_method, array('currency_code' => $order_total['currency_code']));

  // Return an error if the payment method has not been configured yet.
  if (empty($api_credentials['api_username']) || empty($api_credentials['api_password'])) {
    drupal_set_message(t('Selected payment method has not yet been configured for use.'), 'error');
    // Redirect user to previous checkout page.
    commerce_payment_redirect_pane_previous_page($order);
    drupal_goto(commerce_checkout_order_uri($order));
  }
  elseif (!in_array(arg(3), array('back', 'return'))) {

    // If existing Card on File was selected, we need to use Request a New Order
    // Hipay API resource, instead of Initializing a Hosted Payment Page.
    if (!empty($order->data['cardonfile']) || !empty($order->data['commerce_hipay_tpp_direct_debit'])) {
      commerce_hipay_tpp_api_order_request($order, $payment_method);
    }

    // Initialize a hosted payment page.
    $response = commerce_hipay_tpp_api_initialize($order, $payment_method);

    if (!empty($response['code'])) {
      drupal_set_message(t(COMMERCE_HIPAY_TPP_MESSAGE_PAYMENT_ERROR_GENERAL), 'error');
      // Redirect user to previous checkout page.
      commerce_payment_redirect_pane_previous_page($order);
      drupal_goto(commerce_checkout_order_uri($order));
    }

    // Prepare redirect form.
    $form = array();

    // Store the payment method instance in the form array.
    $form['commerce_hipay_tpp_payment_method'] = array(
      '#type' => 'value',
      '#value' => $payment_method,
    );

    // Determine how to process the redirect based on the payment method
    // settings.
    switch ($payment_method['settings']['template']) {

      // Full-page redirect: go to Hipay URL received in the init call response.
      case COMMERCE_HIPAY_TPP_TEMPLATE_BASIC:
        drupal_goto($response['forwardUrl']);
        break;

      // Embedded iframe: display content of the Hipay URL received in the init
      // call response without leaving the site.
      case COMMERCE_HIPAY_TPP_TEMPLATE_IFRAME:
        // Add the iframe in a markup element.
        $form['iframe'] = array(
          '#markup' => commerce_hipay_tpp_get_iframe($response, $payment_method, $order),
        );
        break;
    }

    return $form;
  }
}

/**
 * Payment method callback: redirect form return validation.
 *
 * @see hook_commerce_payment_method_info()
 * @see commerce_hipay_tpp_redirect_form()
 * @see commerce_hipay_tpp_redirect_form_submit()
 * @see commerce_payment_redirect_pane_checkout_form()
 */
function commerce_hipay_tpp_redirect_form_validate($order, $payment_method) {
  $feedback = commerce_hipay_tpp_get_feedback();

  $result = commerce_hipay_tpp_redirect_process_feedback($feedback, $order, $payment_method);

  // Allow other modules to validate feedback and change the validation result.
  foreach (module_implements('commerce_hipay_tpp_redirect_form_validate') as $module) {
    $function = $module . '_commerce_hipay_tpp_redirect_form_validate';
    $result = $function($feedback, $order, $payment_method) && $result;
  }

  return $result;
}

/**
 * Processes the offsite redirect return feedback.
 *
 * @param array $feedback
 *   An associative array containing the Hipay API call feedback.
 * @param object $order
 *   An order the feedback relates to.
 * @param array $payment_method
 *   A payment method used to pay for the order.
 *
 * @return bool
 *   A boolean indicating whether the processing was successful or not.
 *
 * @see commerce_hipay_tpp_redirect_form_validate()
 */
function commerce_hipay_tpp_redirect_process_feedback($feedback, $order, $payment_method) {
  // Log the Hipay return feedback if response logging is enabled.
  if (!empty($payment_method['settings']['api_logging']['response'])) {
    watchdog('commerce_hipay_tpp', 'Offsite redirect: received feedback: !feedback', array(
      '!feedback' => '<pre>' . var_export($feedback, TRUE) . '</pre>',
    ), WATCHDOG_DEBUG);
  }

  // Make sure that we have actually received the feedback from the gateway.
  if (empty($feedback) || !is_array($feedback)) {
    watchdog('commerce_hipay_tpp', 'Offsite redirect: Missing or invalid feedback received on redirect return: !feedback', array(
      '!feedback' => '<pre>' . var_export($feedback, TRUE) . '</pre>',
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Before we start with any validation, let's first try to load the payment
  // transaction and add received feedback to its payload, so it would be
  // possible later on to check what has gone wrong.
  if (!$transaction = commerce_hipay_tpp_feedback_get_payment_transaction($feedback)) {
    watchdog('commerce_hipay_tpp', 'Offsite redirect: Unable to load payment transaction from response cdata2 (@cdata2)', array(
      '@cdata2' => $feedback['cdata2'],
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Update payment transaction payload with received feedback.
  $transaction->payload['return-' . time()] = $feedback;
  commerce_payment_transaction_save($transaction);

  // Validate the signature either if it was provided by the gateway
  // or when the secret passphrase is configured locally.
  if (!commerce_hipay_tpp_feedback_validate_signature($feedback, $payment_method)) {
    watchdog('commerce_hipay_tpp', 'Offsite redirect: signature hashes do not match.', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  // Verify that the response orderid matches local order_id.
  if (empty($feedback['cdata1']) || $feedback['cdata1'] != $order->order_id) {
    watchdog('commerce_hipay_tpp', 'Offsite redirect: Response order ID in cdata1 (@cdata1) does not match local order_id (@order_id).', array(
      '@cdata1' => $feedback['cdata1'],
      '@order_id' => $order->order_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Verify that the response redirect key matches the one stored in the order.
  if (!commerce_hipay_tpp_feedback_validate_payment_redirect_key($feedback, $order)) {
    watchdog('commerce_hipay_tpp', 'Offsite redirect: Response redirect key in cdata3 (@cdata3) does not match order @order_id redirect key (@order_redirect_key).', array(
      '@cdata3' => $feedback['cdata3'],
      '@order_id' => $order->order_id,
      '@order_redirect_key' => $order->data['payment_redirect_key'],
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Now that we have done some basic feedback validation, let's update
  // the payment transaction with feedback details. We need to do it here
  // instead of commerce_hipay_tpp_redirect_form_submit(), as we want to
  // populate it with selected feedback details, even if the payment was
  // declined (because if the payment is declined we don't go into
  // commerce_hipay_tpp_redirect_form_submit() at all).
  // Also:
  // If transaction status is anything else than 'pending' (the original status
  // with which a payment transaction is created during initialization request)
  // it means that we have already received a server-to-server notification,
  // which already updated its details, so we don't want to change them anymore.
  if ($transaction->status == COMMERCE_PAYMENT_STATUS_PENDING) {
    $transaction->remote_id = $feedback['reference'];
    $transaction->status = COMMERCE_HIPAY_TPP_PAYMENT_STATUS_PENDING;
    $transaction->remote_status = $feedback['status'];
    $transaction->message = t('@message');
    $transaction->message_variables = array(
      '@message' => commerce_hipay_tpp_get_payment_status_message($feedback['status']),
    );
  }
  commerce_payment_transaction_save($transaction);

  // Finally, if payment status is anything but accepted or pending,
  // we need to return the customer to the previous checkout page.
  if (
    $feedback['state'] != COMMERCE_HIPAY_TPP_API_STATUS_COMPLETED
    && $feedback['state'] != COMMERCE_HIPAY_TPP_API_STATUS_PENDING
  ) {
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    commerce_payment_transaction_save($transaction);
    return FALSE;
  }

  return TRUE;
}

/**
 * Payment method callback: redirect form return submission.
 *
 * @see hook_commerce_payment_method_info()
 * @see commerce_hipay_tpp_redirect_form()
 * @see commerce_hipay_tpp_redirect_form_validate()
 * @see commerce_payment_redirect_pane_checkout_form()
 */
function commerce_hipay_tpp_redirect_form_submit($order, $payment_method) {
  $feedback = commerce_hipay_tpp_get_feedback();

  // Allow other modules to do their own submit processing.
  foreach (module_implements('commerce_hipay_tpp_redirect_form_submit') as $module) {
    $function = $module . '_commerce_hipay_tpp_redirect_form_submit';
    $function($feedback, $order, $payment_method);
  }
}

/**
 * Callback page for both gateway responses - redirect back and validate.
 *
 * @see commerce_hipay_tpp_menu().
 */
function commerce_hipay_tpp_callback_notification() {
  // Get Hipay API notification feedback and process it.
  $callback_type = 'notification';
  $feedback = commerce_hipay_tpp_get_feedback();

  if (!empty($feedback['payment_product']) && $feedback['payment_product'] == 'sct') {
    $result = commerce_hipay_tpp_process_sct_notification($feedback);
  }
  else {
    $result = commerce_hipay_tpp_process_notification($feedback, $callback_type);
  }

  // Allow other modules to process the notification.
  foreach (module_implements('commerce_hipay_tpp_process_notification') as $module) {
    $function = $module . '_commerce_hipay_tpp_process_notification';
    $result = $function($feedback, $callback_type, $result) && $result;
  }

  // Add error header if there was a notification processing error at any point.
  if ($result === FALSE) {
    header('HTTP/1.1 400 Bad Request', TRUE, 400);
  }
}

/**
 * Processes the feedback returned by Hipay API.
 *
 * @param array $feedback
 *   An associative array containing the Hipay API call feedback.
 * @param string $callback_type
 *   A string indicating which callback type we are dealing with.
 *   Might be either 'response', when a user is redirected back from Hipay
 *   after completing/cancelling the payment transaction, or 'notification',
 *   when the gateway performs an asynchronous Server-to-Server Notificagtion
 *   to verify transaction details/status.
 *
 * @return bool
 *   Boolean indicating whether the feedback was processed successfully.
 *
 * @see commerce_hipay_tpp_callback_notification()
 * @see commerce_hipay_tpp_refresh()
 */
function commerce_hipay_tpp_process_notification($feedback, $callback_type) {
  $log_type = (!empty($feedback['payment_product']) && $feedback['payment_product'] == 'sdd') ? 'commerce_hipay_tpp_direct_debit' : 'commerce_hipay_tpp';

  // Make sure that we have actually received the feedback from the gateway.
  if (empty($feedback) || !is_array($feedback)) {
    watchdog($log_type, '@callback_type: Missing or invalid feedback received in notification: !feedback', array(
      '@callback_type' => ucfirst($callback_type),
      '!feedback' => '<pre>' . var_export($feedback, TRUE) . '</pre>',
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Before we start with any validation, let's first try to load the payment
  // transaction and add received feedback to its payload, so it would be
  // possible later on to check what has gone wrong.
  if (!$transaction = commerce_hipay_tpp_feedback_get_payment_transaction($feedback)) {
    watchdog($log_type, '@callback_type: Unable to load payment transaction from feedback cdata2 (@cdata2). Feedback received: !feedback', array(
      '@callback_type' => ucfirst($callback_type),
      '@cdata2' => $feedback['cdata2'],
      '!feedback' => '<pre>' . var_export($feedback, TRUE) . '</pre>',
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  $operation_transaction = NULL;
  if (!empty($feedback['operation']['id'])) {
    if (!$operation_transaction = commerce_hipay_tpp_feedback_get_operation_payment_transaction($feedback)) {
      watchdog($log_type, '@callback_type: Unable to load payment transaction from Operation Reference (@last_operation_id). Feedback received: !feedback', array(
        '@callback_type' => ucfirst($callback_type),
        '@last_operation_id' => $feedback['operation']['id'],
        '!feedback' => '<pre>' . var_export($feedback, TRUE) . '</pre>',
      ), WATCHDOG_ERROR);
      return FALSE;
    }
  }
  // Update main payment transaction payload with received feedback.
  else {
    $transaction->payload[$callback_type . '-' . time()] = $feedback;
    commerce_payment_transaction_save($transaction);
  }

  // Log the Hipay return feedback if response logging is enabled.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  if (!empty($payment_method['settings']['api_logging']['response'])) {
    watchdog($log_type, '@callback_type: !feedback', array(
      '@callback_type' => ucfirst($callback_type),
      '!feedback' => '<pre>' . var_export($feedback, TRUE) . '</pre>',
    ), WATCHDOG_DEBUG);
  }

  // Validate the feedback signature.
  if (!commerce_hipay_tpp_feedback_validate_signature($feedback, $payment_method)) {
    watchdog($log_type, '@callback_type: signature hashes do not match.', array(
      '@callback_type' => ucfirst($callback_type),
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Try to load the order.
  if (!$order = commerce_hipay_tpp_feedback_get_order($feedback)) {
    watchdog($log_type, '@callback_type: Unable to load order from response cdata1 (@cdata1).', array(
      '@callback_type' => ucfirst($callback_type),
      '@cdata1' => $feedback['cdata1'],
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Make sure the response redirect key matches the one stored in the order.
  if (!commerce_hipay_tpp_feedback_validate_payment_redirect_key($feedback, $order)) {
    watchdog($log_type, '@callback_type: Response redirect key (@response_redirect_key) does not match order @order_id key (@order_redirect_key).', array(
      '@callback_type' => ucfirst($callback_type),
      '@response_redirect_key' => $feedback['cdata3'],
      '@order_id' => $order->order_id,
      '@order_redirect_key' => $order->data['payment_redirect_key'],
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Update the payment transaction with notification data.
  // Note that this function is used for processing feedback return both by
  // server-to-server notification as well as transaction refresh call.
  // http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap6-ServerToServer.html
  // http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-requestDetails.html
  // In some cases they return different names for the same parameter (for
  // example server-to-server notification returns 'transaction_reference',
  // while refresh call returns 'transactionReference'), which we must account
  // for here - hence the use of commerce_hipay_hpp_get_feedback_value().
  $transaction->data['authorized_amount'] = commerce_currency_decimal_to_amount(commerce_hipay_tpp_get_feedback_value($feedback, 'authorizedAmount'), $feedback['currency']);
  $transaction->data['captured_amount'] = commerce_currency_decimal_to_amount(commerce_hipay_tpp_get_feedback_value($feedback, 'capturedAmount'), $feedback['currency']);
  $transaction->data['refunded_amount'] = commerce_currency_decimal_to_amount(commerce_hipay_tpp_get_feedback_value($feedback, 'refundedAmount'), $feedback['currency']);

  // If we have a notification for a certain transaction operation
  // let's update that first.
  if ($operation_transaction) {
    switch ($feedback['status']) {
      case COMMERCE_HIPAY_TPP_REMOTE_STATUS_CAPTURED:
        $operation_transaction->status = COMMERCE_HIPAY_TPP_PAYMENT_STATUS_CAPTURED;
        break;

      case COMMERCE_HIPAY_TPP_REMOTE_STATUS_CAPTURE_REFUSED:
        $operation_transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
        break;

      case COMMERCE_HIPAY_TPP_REMOTE_STATUS_PARTIALLY_REFUNDED:
        $operation_transaction->status = COMMERCE_HIPAY_TPP_PAYMENT_STATUS_PARTIALLY_REFUNDED;
        break;

      case COMMERCE_HIPAY_TPP_REMOTE_STATUS_REFUNDED:
        $operation_transaction->status = COMMERCE_HIPAY_TPP_PAYMENT_STATUS_REFUNDED;
        break;
    }

    $operation_transaction->remote_status = $feedback['status'];
    $operation_transaction->message = t('@status_message');
    $operation_transaction->message_variables = array(
      '@status_message' => commerce_hipay_tpp_get_payment_status_message($feedback['status']),
    );
    $operation_transaction->payload[$callback_type . '-' . time()] = $feedback;
    $operation_transaction->revision = TRUE;
    $operation_transaction->log = t('Updated from received Hipay notification.');
    commerce_payment_transaction_save($operation_transaction);
  }
  // Otherwise we update the main transaction.
  else {
    $transaction->remote_id = commerce_hipay_tpp_get_feedback_value($feedback, 'transactionReference');

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
  }

  // We want to update the main transaction status only for the very first call
  // (regardless whether it was authorization or capture), which means only when
  // the main transaction still has its 'pending' status.
  if (
    // Initial status for 'sale' transactions.
    $transaction->status == COMMERCE_PAYMENT_STATUS_PENDING
    // Initial status for 'authorization' transactions.
    || $transaction->status == COMMERCE_HIPAY_TPP_PAYMENT_STATUS_PENDING
  ) {
    switch ($feedback['state']) {
      case 'pending':
        // No update in this case.
        break;
      case 'completed':
        // We use different 'success' status for authorization and sale
        // transactions. Note that it is important to use payment transaction
        // status COMMERCE_PAYMENT_STATUS_SUCCESS for sale transactions, as only
        // then the 'commerce_payment_order_paid_in_full' rule will be invoked
        // (see commerce_payment_commerce_payment_transaction_insert()).
        if (
          $transaction->data['operation'] == COMMERCE_HIPAY_TPP_OPERATION_SALE
          && in_array($feedback['status'], commerce_hipay_tpp_get_remote_success_statuses_for_operation_type(COMMERCE_HIPAY_TPP_OPERATION_CAPTURE))
        ) {
          $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;

          // After saving this successful transaction and recalculating order
          // balance, if the order is paid in then it will be updated (see
          // commerce_payment_commerce_payment_transaction_insert()) - let's
          // then make sure that all this will create a new order revision.
          $order->revision = TRUE;
          $order->log = t('Updated from received Hipay notification.');
        }
        elseif (
          $transaction->data['operation'] == COMMERCE_HIPAY_TPP_OPERATION_AUTHORIZATION
          && in_array($feedback['status'], commerce_hipay_tpp_get_remote_success_statuses_for_operation_type(COMMERCE_HIPAY_TPP_OPERATION_AUTHORIZATION))
        ) {
          $transaction->status = COMMERCE_HIPAY_TPP_PAYMENT_STATUS_AUTHORIZED;
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

  // CVC result.
  if (($cvc_result = commerce_hipay_tpp_get_feedback_value($feedback, 'cvcResult')) !== NULL) {
    $transaction->message .= '<br />' . t('CVC: @cvc_message');
    $transaction->message_variables['@cvc_message'] = commerce_hipay_tpp_get_cvc_result_message($cvc_result);
  }

  // AVS result.
  if (($avs_result = commerce_hipay_tpp_get_feedback_value($feedback, 'avsResult')) !== NULL) {
    $transaction->message .= '<br />' . t('AVS: @avs_message');
    $transaction->message_variables['@avs_message'] = commerce_hipay_tpp_get_avs_result_message($avs_result);
  }

  // 3-D Secure result.
  if ($three_d_secure = commerce_hipay_tpp_get_feedback_value($feedback, 'threeDSecure')) {
    $transaction->message .= '<br />' . t('3DS: @3ds_message');
    if ($authentication_message = commerce_hipay_tpp_get_feedback_value($three_d_secure, 'authenticationMessage')) {
      $transaction->message_variables['@3ds_message'] = $authentication_message;
    }
    else {
      $transaction->message_variables['@3ds_message'] = commerce_hipay_tpp_get_feedback_value($three_d_secure, 'enrollmentMessage');
    }
  }

  // Fraud screening result.
  if ($fraud_screening = commerce_hipay_tpp_get_feedback_value($feedback, 'fraudScreening')) {
    $message = array(t('Fraud screening'));
    if (!empty($fraud_screening['scoring'])) {
      $message[] = t('score @fraud_score');
      $transaction->message_variables['@fraud_score'] = $fraud_screening['scoring'];
    }
    if (!empty($fraud_screening['result'])) {
      $message[] = t('@fraud_result');
      $transaction->message_variables['@fraud_result'] = $fraud_screening['result'];
    }
    if (!empty($fraud_screening['review'])) {
      $message[] = t('@fraud_review');
      $transaction->message_variables['@fraud_review'] = $fraud_screening['review'];
    }
    $transaction->message .= '<br />' . implode(': ', $message);
  }

  // Authorized amount.
  if (($authorized_amount = commerce_hipay_tpp_get_feedback_value($feedback, 'authorizedAmount')) != 0) {
    $transaction->message .= '<br />' . t('Authorized amount: @authorized_amount');
    $transaction->message_variables['@authorized_amount'] = commerce_currency_format(commerce_currency_decimal_to_amount($authorized_amount, $transaction->currency_code), $transaction->currency_code);
  }

  // Captured amount.
  if (($captured_amount = commerce_hipay_tpp_get_feedback_value($feedback, 'capturedAmount')) != 0) {
    $transaction->message .= '<br />' . t('Total captured amount: @captured_amount');
    $transaction->message_variables['@captured_amount'] = commerce_currency_format(commerce_currency_decimal_to_amount($captured_amount, $transaction->currency_code), $transaction->currency_code);
  }

  // Refunded amount.
  if (($refunded_amount = commerce_hipay_tpp_get_feedback_value($feedback, 'refundedAmount')) != 0) {
    $transaction->message .= '<br />' . t('Total refunded amount: @refunded_amount');
    $transaction->message_variables['@refunded_amount'] = commerce_currency_format(commerce_currency_decimal_to_amount($refunded_amount, $transaction->currency_code), $transaction->currency_code);
  }

  $transaction->revision = TRUE;
  $transaction->log = t('Updated from received Hipay notification.');
  commerce_payment_transaction_save($transaction);

  // Try to save new Card on File.
  if (!empty($order->data['commerce_hipay_tpp']['cardonfile_store'])) {
    commerce_hipay_tpp_cardonfile_save($feedback, $order, $payment_method);
  }

  return TRUE;
}

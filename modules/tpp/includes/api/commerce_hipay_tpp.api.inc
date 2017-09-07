<?php

/**
 * Performs a call to Hipay TPP API.
 *
 * @param string $url
 *   Hipay API endpoint URL.
 * @param array $request_data
 *   A string of parameters to be sent to Hipay TPP API.
 * @param array $payment_method
 *   The payment method being used.
 * @param array $context
 *   An associative array containing additional information about the request.
 * @param string $type
 *   The category to which watchdog messages should belong.
 *   If empty by default set to 'commerce_hipay_tpp'.
 *
 * @return object
 *   An array of Hipay TPP API response parameters.
 */
function commerce_hipay_tpp_api_request($url, $request_data, $payment_method, $context = array(), $type = 'commerce_hipay_tpp') {
  // Call type description for logging purposes.
  $call_type = !empty($context['call_type']) ? $context['call_type'] : 'API call';
  
  // Log the request data if request logging is enabled.
  if (!empty($payment_method['settings']['api_logging']['request'])) {
    watchdog($type, '@call_type: request to @url: !request_data', array(
      '@call_type' => $call_type,
      '@url' => $url,
      '!request_data' => '<pre>' . var_export($request_data, TRUE) . '</pre>',
    ), WATCHDOG_DEBUG);
  }

  $ch = curl_init($url);

  // Log cURL verbose information to a file.
  if (!empty($payment_method['settings']['api_logging']['request']) || !empty($payment_method['settings']['api_logging']['response'])) {
    $curl_debug_file = file_directory_temp() . '/curl.log';
    $fh = fopen($curl_debug_file, "a+");
    curl_setopt($ch, CURLOPT_STDERR, $fh);
    curl_setopt($ch, CURLOPT_VERBOSE, TRUE);
  }

  $api_credentials = commerce_hipay_tpp_get_api_credentials($payment_method, $context);
  curl_setopt($ch, CURLOPT_USERPWD, $api_credentials['api_username'] . ':' . $api_credentials['api_password']);
  curl_setopt($ch, CURLOPT_HTTPHEADER, array(
    'Accept: application/json',
  ));
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ch, CURLOPT_FAILONERROR, FALSE);
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 10);
  curl_setopt($ch, CURLOPT_HEADER, FALSE);
  curl_setopt($ch, CURLOPT_POST, TRUE);
  curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($request_data));

  $start = microtime(TRUE);

  // Execute the request.
  $result = curl_exec($ch);

  $request_time = microtime(TRUE) - $start;

  // Log the cURL response if response logging is enabled.
  if (!empty($payment_method['settings']['api_logging']['response'])) {
    watchdog($type, '@call_type: cURL response: !response', array(
      '@call_type' => $call_type,
      '!response' => '<pre>' . var_export($result, TRUE) . '</pre>',
    ), WATCHDOG_DEBUG);
  }

  // If there was an error performing cURL request.
  if ($error = curl_error($ch)) {
    // Log cURL error.
    watchdog($type, '@call_type: cURL error: @error', array(
      '@call_type' => $call_type,
      '@error' => $error,
    ), WATCHDOG_ERROR);

    // Create response object in the same format as decoded Hipay API exception
    // response.
    $response = array(
      'code' => curl_errno($ch),
      'message' => 'cURL error',
      'description' => $error,
    );
  }
  // No cURL errors, the request worked fine.
  else {
    // Decode JSON response into array.
    $response = json_decode($result, TRUE);

    // Log the response if response logging is enabled.
    if (!empty($payment_method['settings']['api_logging']['response'])) {
      watchdog($type, '@call_type: response: !response Request time: @request_time', array(
        '@call_type' => $call_type,
        '!response' => '<pre>' . var_export($response, TRUE) . '</pre>',
        '@request_time' => $request_time,
      ), WATCHDOG_DEBUG);
    }
  }

  curl_close($ch);
  if ((!empty($payment_method['settings']['api_logging']['request']) || !empty($payment_method['settings']['api_logging']['response'])) && is_resource($fh)) {
    fclose($fh);
  }

  return $response;
}

/**
 * Performs background call to Hipay API to initialize the hosted payment page.
 *
 * @param object $order
 *   The order object the hosted checkout is for.
 * @param array $payment_method
 *   The payment method instance used to generate the redirect link.
 *
 * @return object
 *   The Hipay initialization call response object.
 *
 * @see http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-initializeHostedPaymentPage.html
 */
function commerce_hipay_tpp_api_initialize($order, $payment_method) {
  // Get generic API request data.
  // The same function is also used for 'Request a New Order' API resource
  // (see commerce_hipay_tpp_api_order_request()).
  $request_data = commerce_hipay_tpp_get_request_data($order, $payment_method);

  // E-commerce with SSL/TLS Encryption.
  if (!isset($request_data['eci'])) {
    $request_data['eci'] = COMMERCE_HIPAY_TPP_ECI_ECOMMERCE;
  }

  // Use either full-page redirect to the hosted payment page
  // or embed it in the iframe.
  $request_data['template'] = $payment_method['settings']['template'];

  // Create payment transaction before we call Hipay API.
  $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->amount = commerce_currency_decimal_to_amount($request_data['amount'], $request_data['currency']);
  $transaction->currency_code = $request_data['currency'];
  $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
  $transaction->message = t('@operation transaction initialized.');
  $transaction->message_variables = array(
    '@operation' => ucfirst($payment_method['settings']['operation']),
  );
  $transaction->data['operation'] = $payment_method['settings']['operation'];
  commerce_payment_transaction_save($transaction);

  $request_data['cdata2'] = $transaction->transaction_id;

  //$request_data['operation_id'] = $transaction->transaction_id;

  if (module_exists('commerce_cardonfile') && !empty($payment_method['settings']['cardonfile'])) {
    $request_data['multi_use'] = 1;
  }

  // Allow other modules to alter the $request_data array.
  drupal_alter('commerce_hipay_tpp_api_initialize', $request_data, $order, $payment_method);

  // Add request data to the transaction payload.
  $transaction->payload['init-request-' . time()] = $request_data;
  commerce_payment_transaction_save($transaction);

  // Perform the call to Hipay TPP API.
  $url = commerce_hipay_tpp_get_server_url($payment_method) . COMMERCE_HIPAY_TPP_RESOURCE_INITIALIZATION;
  $context = array(
    'call_type' => 'Initialization',
    'currency_code' => $request_data['currency'],
  );
  $response = commerce_hipay_tpp_api_request($url, $request_data, $payment_method, $context);

  // Update payment transaction with response details.
  $transaction->payload['init-response-' . time()] = $response;

  // Handle API exceptions.
  if (!empty($response['code'])) {
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->message = t('Error @error_code: @error_message: @error_description');
    $transaction->message_variables = array(
      '@error_code' => $response['code'],
      '@error_message' => $response['message'],
      '@error_description' => $response['description'],
    );
  }

  // Save updated transaction.
  commerce_payment_transaction_save($transaction);

  // Allow other modules to process the initialization response.
  foreach (module_implements('commerce_hipay_tpp_api_initialize_response') as $module) {
    $function = $module . '_commerce_hipay_tpp_api_initialize_response';
    $function($response, $transaction, $order, $payment_method);
  }

  return $response;
}

/**
 * Performs background call to Hipay API to request a new order.
 *
 * @param object $order
 *   The order object the hosted checkout is for.
 * @param array $payment_method
 *   The payment method instance used to request a new Hipay order.
 *
 * @see http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-requestNewOrder.html
 */
function commerce_hipay_tpp_api_order_request($order, $payment_method) {
  // We can request a new order using either a stored credit card...
  if (!empty($order->data['cardonfile'])) {
    $result = commerce_hipay_tpp_cardonfile_charge($payment_method, $order->data['cardonfile'], $order);
  }
  // ...or a stored direct debit mandate.
  elseif (!empty($order->data['commerce_hipay_tpp_direct_debit'])) {
    $result = commerce_hipay_tpp_direct_debit_charge($payment_method, $order);
  }

  if (!empty($result)) {
    commerce_payment_redirect_pane_next_page($order, t('Customer successfully submitted payment at the payment gateway.'));
  }
  else {
    commerce_payment_redirect_pane_previous_page($order, t('Customer payment submission failed at the payment gateway.'));
  }

  // Redirect user to the relevant checkout page.
  drupal_goto(commerce_checkout_order_uri($order));
}

/**
 * Captures an amount on existing authorization transaction.
 *
 * @param object $transaction
 *   An authorization transaction to be captured.
 * @param int $amount
 *   An amount to be captured.
 *
 * @return object
 *   A capture transaction.
 */
function commerce_hipay_tpp_api_capture($transaction, $amount) {
  $order = commerce_order_load($transaction->order_id);
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);

  // Create new transaction for capture.
  $capture_transaction = commerce_payment_transaction_new($payment_method['method_id'], $transaction->order_id);
  $capture_transaction->instance_id = $payment_method['instance_id'];
  $capture_transaction->amount = commerce_currency_decimal_to_amount($amount, $transaction->currency_code);
  $capture_transaction->currency_code = $transaction->currency_code;
  $capture_transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
  $capture_transaction->message = t('Capture transaction initialized.');
  $capture_transaction->data['reference_transaction_id'] = $transaction->transaction_id;
  $capture_transaction->data['operation'] = COMMERCE_HIPAY_TPP_OPERATION_CAPTURE;
  $capture_transaction->remote_id = $transaction->remote_id;
  commerce_payment_transaction_save($capture_transaction);

  // Prepare the request data.
  // See http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-maintenanceOperations.html
  $request_data = array(
    'operation' => COMMERCE_HIPAY_TPP_OPERATION_CAPTURE,
    'amount' => $amount,
    'operation_id' => $capture_transaction->transaction_id,
  );

  // Allow other modules to alter the $request_data array.
  drupal_alter('commerce_hipay_tpp_api_capture', $request_data, $order, $payment_method);

  // Add request data to the transaction's payload.
  $capture_transaction->payload['request-' . time()] = $request_data;
  commerce_payment_transaction_save($capture_transaction);

  // Perform the call to Hipay TPP API.
  $url = commerce_hipay_tpp_get_server_url($payment_method) . COMMERCE_HIPAY_TPP_RESOURCE_MAINTAINANCE . $transaction->remote_id;
  $context = array(
    'call_type' => 'Capture',
    'currency_code' => $transaction->currency_code,
  );
  $response = commerce_hipay_tpp_api_request($url, $request_data, $payment_method, $context);

  // Update payment transaction with response details.
  $capture_transaction->payload['response-' . time()] = $response;
  commerce_payment_transaction_save($capture_transaction);

  commerce_hipay_tpp_process_response($response, $capture_transaction, COMMERCE_HIPAY_TPP_OPERATION_CAPTURE);

  return $capture_transaction;
}

/**
 * Refunds an amount on existing transaction.
 *
 * @param object $capture_transaction
 *   The capture transaction
 * @param int $amount
 *   An amount to be refunded.
 *
 * @return object
 *   A Refund transaction.
 */
function commerce_hipay_tpp_api_refund($capture_transaction, $amount) {
  $order = commerce_order_load($capture_transaction->order_id);
  $payment_method = commerce_payment_method_instance_load($capture_transaction->instance_id);

  // Create new transaction for refund.
  $refund_transaction = commerce_payment_transaction_new($payment_method['method_id'], $capture_transaction->order_id);
  $refund_transaction->instance_id = $payment_method['instance_id'];
  $refund_transaction->amount = -commerce_currency_decimal_to_amount($amount, $capture_transaction->currency_code);
  $refund_transaction->currency_code = $capture_transaction->currency_code;
  $refund_transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
  $refund_transaction->message = t('Refund transaction initialized.');
  $refund_transaction->data['reference_transaction_id'] = $capture_transaction->transaction_id;
  $refund_transaction->data['operation'] = COMMERCE_HIPAY_TPP_OPERATION_REFUND;
  $refund_transaction->remote_id = $capture_transaction->remote_id;

  commerce_payment_transaction_save($refund_transaction);

  // Prepare the request data.
  // See http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-maintenanceOperations.html
  $request_data = array(
    'operation' => COMMERCE_HIPAY_TPP_OPERATION_REFUND,
    'amount' => $amount,
    'operation_id' => $refund_transaction->transaction_id,
  );

  // Allow other modules to alter the $request_data array.
  drupal_alter('commerce_hipay_tpp_api_refund', $request_data, $order, $payment_method);

  // Add request data to the transaction's payload.
  $refund_transaction->payload['request-' . time()] = $request_data;
  commerce_payment_transaction_save($refund_transaction);

  // Perform the call to Hipay TPP API.
  $url = commerce_hipay_tpp_get_server_url($payment_method) . COMMERCE_HIPAY_TPP_RESOURCE_MAINTAINANCE . $capture_transaction->remote_id;
  $context = array(
    'call_type' => 'Refund',
    'currency_code' => $capture_transaction->currency_code,
  );
  $response = commerce_hipay_tpp_api_request($url, $request_data, $payment_method, $context);

  // Update refund transaction with response details.
  $refund_transaction->payload['response-' . time()] = $response;
  commerce_payment_transaction_save($refund_transaction);

  commerce_hipay_tpp_process_response($response, $refund_transaction, 'refund');

  return $refund_transaction;
}

/**
 * Cancels an existing transaction.
 *
 * @param object $transaction
 *   An transaction to cancel.
 */
function commerce_hipay_tpp_api_cancel($transaction) {
  $order = commerce_order_load($transaction->order_id);
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);

  // Prepare the request data.
  // See http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-maintenanceOperations.html
  $request_data = array(
    'operation' => COMMERCE_HIPAY_TPP_OPERATION_CANCEL,
  );

  // Allow other modules to alter the $request_data array.
  drupal_alter('commerce_hipay_tpp_api_cancel', $request_data, $order, $payment_method);

  $transaction->payload['cancel-request-' . time()] = $request_data;
  commerce_payment_transaction_save($transaction);

  // Perform the call to Hipay TPP API.
  $url = commerce_hipay_tpp_get_server_url($payment_method) . COMMERCE_HIPAY_TPP_RESOURCE_MAINTAINANCE . $transaction->remote_id;
  $context = array(
    'call_type' => 'Cancel',
    'currency_code' => $transaction->currency_code,
  );
  $response = commerce_hipay_tpp_api_request($url, $request_data, $payment_method, $context);

  // Update payment transaction with response details.
  $transaction->payload['cancel-response-' . time()] = $response;

  commerce_payment_transaction_save($transaction);

  if (!isset($response['code'])) {
    drupal_set_message(t('Cancel request has been sent to the payment gateway, now waiting for the server-to-server notification to update the transaction details. Refresh this page in a moment to see updated transaction details.'));
  }
  else {
    drupal_set_message(t('There has been an error requesting the Cancel: @error_code: @error_message: @error_description', array(
      '@error_code' => $response['code'],
      '@error_message' => $response['message'],
      '@error_description' => $response['description'],
    )), 'error');
  }
}


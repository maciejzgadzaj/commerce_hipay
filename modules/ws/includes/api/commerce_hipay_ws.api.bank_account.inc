<?php

/**
 * Creates a bank account in Hipay Wallet.
 *
 * @param object $account
 *   A Hipay bank account entity to create an account from.
 * @param array $payment_method_instance
 *   A payment method instance to use when creating an account.
 *
 * @return array
 *   An array of Hipay TPP API response parameters.
 */
function commerce_hipay_ws_api_bank_account_create($account, $payment_method_instance = NULL) {
  $wrapper = entity_metadata_wrapper('commerce_hipay_ws_bank_account', $account);
  $hipay_user_account = $wrapper->hipay_ws_bank_user_account->value();

  // If hipay_account_id value is already set, it means that Hipay User Account
  // was already created, and we cannot create another one.
  if (empty($hipay_user_account->hipay_account_id)) {
    // @TODO: watchdog?
    return FALSE;
  }

  // If no payment method instance was provided in the function parameter,
  // try to load the first available one.
  if (
    !$payment_method_instance
    && ($instances = commerce_hipay_ws_payment_method_instances('commerce_hipay_ws', TRUE))
  ) {
    $payment_method_instance = reset($instances);
  }
  // If still no payment method instance is available, we cannot continue.
  if (empty($payment_method_instance)) {
    // @TODO: watchdog.
    return FALSE;
  }

  $address = $wrapper->hipay_ws_bank_address->value();

  $parameters = array(
    'wsSubAccountId' => $hipay_user_account->hipay_account_id,
    'bankName' => $wrapper->hipay_ws_bank_name->value(),
    'bankAddress' => commerce_hipay_ws_combine_address_lines($address, array('thoroughfare', 'premise')),
    'bankZipCode' => !empty($address['postal_code']) ? $address['postal_code'] : '',
    'bankCity' => !empty($address['locality']) ? $address['locality'] : '',
    'bankCountry' => $address['country'],
    'iban' => $wrapper->hipay_ws_bank_iban->value(),
    'swift' => $wrapper->hipay_ws_bank_swift->value(),
    'acct_num' => $wrapper->hipay_ws_bank_account_number->value(),
    'aba_num' => $wrapper->hipay_ws_bank_aba_number->value(),
    'transit_num' => $wrapper->hipay_ws_bank_transit_number->value(),
  );

  // Do not send empty parameters to the API.
  foreach ($parameters as $key => $value) {
    if (empty($value)) {
      unset($parameters[$key]);
    }
  }

  // Add request parameters array to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_REGISTER . '-request-' . time();
  $account->data['payload'][$payload_key] = $parameters;

  $context = array(
    'commerce_hipay_ws_user_account' => $hipay_user_account,
    'commerce_hipay_ws_bank_account' => $account,
    'currency_code' => commerce_hipay_ws_user_account_get_currency($hipay_user_account),
  );

  // Perform the call to Hipay Wallet API.
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_REGISTER, $parameters, $context);

  // Add the response to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_REGISTER . '-response-' . time();
  $account->data['payload'][$payload_key] = $response;

  // If everything went fine, create new revision of Hipay bank account entity
  // to keep track of the account creation in Hipay Wallet.
  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    $account->hipay_account_id = $hipay_user_account->hipay_account_id;
    $account->log = 'Created bank account in Hipay Wallet.';
  }
  else {
    $account->log = 'Failed creating bank account in Hipay Wallet.';
  }

  // Save the new bank account entity revision.
  $account->revision = TRUE;
  commerce_hipay_ws_bank_account_save($account);

  return $response;
}

/**
 * Processes Hipay Wallet bank account validation notification.
 *
 * @param array $feedback
 *   An array of XML parameters received in the notification.
 *
 * @return bool
 *   An boolean indicating whether the notification was processed successfully.
 *
 * @see commerce_hipay_ws_callback_notification_process_feedback_xml()
 */
function commerce_hipay_ws_api_bank_account_validate_notification($feedback) {
  // Make sure we have received all required parameters.
  $required_parameters = array('account_id', 'status');
  foreach ($required_parameters as $required_parameter) {
    if (empty($feedback[$required_parameter])) {
      watchdog('commerce_hipay_ws', 'Notification: bank_info_validation: !param_name parameter missing or empty.', array('!param_name' => $required_parameter), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // Try to load the Hipay user account from the received parameter.
  if (!$bank_account = commerce_hipay_ws_bank_account_load_by_hipay_account_id($feedback['account_id'])) {
    watchdog('commerce_hipay_ws', 'Notification: bank_info_validation: unable to load a Hipay bank account entity from account_id parameter: @account_id.', array('@account_id' => $feedback['account_id']), WATCHDOG_ERROR);
    return FALSE;
  }

  // Update the Hipay user account entity with the new identification status.
  $bank_account->hipay_status = ($feedback['status'] == COMMERCE_HIPAY_WS_NOTIFICATION_STATUS_OK) ? COMMERCE_HIPAY_WS_ACCOUNT_BANK_INFO_VALIDATED : COMMERCE_HIPAY_WS_ACCOUNT_BANK_INFO_VALIDATION_IN_PROGRESS;

  // Store the full notification in entity data.
  $payload_key = 'notification-' . $feedback['operation'] . '-' . REQUEST_TIME;
  $bank_account->data['payload'][$payload_key] = $feedback;

  // Create new revision and save updated bank account entity.
  $bank_account->revision = TRUE;
  $bank_account->log = 'Updated status from received Hipay Wallet notificaton.';
  commerce_hipay_ws_bank_account_save($bank_account);

  // Add watchdog entry.
  watchdog('commerce_hipay_ws', 'Notification: bank_info_validation: Updated Hipay bank account %account_id validation status to %status.', array(
    '%account_id' => $bank_account->bank_account_id,
    '%status' => $bank_account->hipay_status,
  ), WATCHDOG_NOTICE);

  // Invoke the rules event to allow other modules to react to the notification.
  $update_result = TRUE;
  foreach (rules_invoke_all('commerce_hipay_ws_bank_account_validation', $bank_account, $feedback) as $result) {
    // If any of the hook implementations returns FALSE, alter the function
    // update result respectively.
    $update_result = $update_result && $result;
  }

  return $update_result;
}

/**
 * Get the required bank field needed for an Hipay account.
 *
 * @param string $country
 *   Country code of the bank account.
 * @param array $context
 *   An associative array containing additional information about the request.
 * @param bool $reset
 *   A boolean indicating whether a cached value should be reset or not.
 *
 * @return array
 *   An array of Hipay Wallet API response parameters, where 'fields' key
 *   contains the list of required bank fields, or FALSE on error.
 */
function commerce_hipay_ws_api_bank_account_get_info_fields($country, $context = array(), $reset = FALSE) {
  $parameters = array('country' => $country);
  return commerce_hipay_ws_api_get_cached_response(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_FIELDS, $context, $reset, $parameters);
}

/**
 * Get the bank status information for an account.
 *
 * @param array $parameters
 *   An array of parameters to pass to the API call.
 * @param array $context
 *   An associative array containing additional information about the request.
 *
 * @return array
 *   An array of Hipay Wallet API response parameters.
 */
function commerce_hipay_ws_api_bank_account_get_info_status($parameters = array(), $context = array()) {
  $hipay_user_account_id = commerce_hipay_ws_api_bank_account_get_request_user_account_id($parameters);
  $hipay_user_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($hipay_user_account_id);
  $context['currency_code'] = commerce_hipay_ws_user_account_get_currency($hipay_user_account);

  // Fetch the bank account info status from Hipay Wallet API.
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_STATUS, $parameters, $context);

  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    // If we are checking bank info status for a Hipay bank account entity,
    // and received status differs from the status saved in the entity,
    // let's update it, creating a new revision.
    if (
      !empty($context['commerce_hipay_ws_bank_account'])
      && $context['commerce_hipay_ws_bank_account']->hipay_status != $response['status']
    ) {
      $hipay_bank_account = $context['commerce_hipay_ws_bank_account'];
      $hipay_bank_account->hipay_status = $response['status'];
      $hipay_bank_account->revision = TRUE;
      $hipay_bank_account->log = t('Updated bank account status from %resource API call response.', array('%resource' => COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_STATUS));
      commerce_hipay_ws_bank_account_save($hipay_bank_account);
    }
  }

  return $response;
}

/**
 * Fetches the bank information for an account from Hipay Wallet.
 *
 * @param array $parameters
 *   An array of parameters to pass to the API call.
 * @param array $context
 *   An associative array containing additional information about the request.
 *
 * @return array
 *   An array of Hipay Wallet API response parameters.
 */
function commerce_hipay_ws_api_bank_account_infos_check($parameters = array(), $context = array()) {
  $hipay_user_account_id = commerce_hipay_ws_api_bank_account_get_request_user_account_id($parameters);
  $hipay_user_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($hipay_user_account_id);
  $context['currency_code'] = commerce_hipay_ws_user_account_get_currency($hipay_user_account);

  // Fetch the bank account info status from Hipay Wallet API.
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_CHECK, $parameters, $context);

  return $response;
}

/**
 * Returns Hipay user account ID of the request, or ID of the default one.
 *
 * @param array $parameters
 *   An array of Hipay Wallet API request parameters.
 *
 * @return int
 *   A Hipay user account ID.
 */
function commerce_hipay_ws_api_bank_account_get_request_user_account_id($parameters) {
  // If 'wsSubAccountId' parameter was provided in the request, just return it.
  if (!empty($parameters['wsSubAccountId'])) {
    return $parameters['wsSubAccountId'];
  }
  // Otherwise return ID of the main technical account in the default currency.
  else {
    $payment_method_instance = commerce_hipay_ws_payment_method_instance();
    $default_currency = commerce_default_currency();
    return $payment_method_instance['settings']['accounts'][$default_currency]['account_id'];
  }
}

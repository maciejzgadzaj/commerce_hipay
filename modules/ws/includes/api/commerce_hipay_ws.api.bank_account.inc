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
    'bankZipCode' => $address['postal_code'],
    'bankCity' => $address['locality'],
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

  $context = array(
    'hipay_user_account' => $hipay_user_account,
    'hipay_bank_account' => $account,
  );

  // Perform the call to Hipay Wallet API.
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_REGISTER, $parameters, $context);

  // If everything went fine, create new revision of Hipay bank account entity
  // to keep track of the account creation in Hipay Wallet.
  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    $account->hipay_account_id = $hipay_user_account->hipay_account_id;
    $account->revision = TRUE;
    $account->log = 'Created bank account in Hipay Wallet.';
    commerce_hipay_ws_bank_account_save($account);
  }

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
  $bank_account->revision = TRUE;
  $bank_account->log = 'Updated status from received Hipay Wallet notificaton.';
  commerce_hipay_ws_bank_account_save($bank_account);

  // Add watchdog entry.
  watchdog('commerce_hipay_ws', 'Notification: bank_info_validation: Updated Hipay bank account %account_id validation status to %status.', array(
    '%account_id' => $bank_account->bank_account_id,
    '%status' => $bank_account->hipay_status,
  ), WATCHDOG_NOTICE);

  // Invoke the rules event to allow other modules to react to the notification.
  return rules_invoke_all('commerce_hipay_ws_bank_account_validation', $bank_account, $feedback);
}

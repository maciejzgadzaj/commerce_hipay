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
//  dpm($account, 'create : $account');
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

  $context = array(
    'hipay_user_account' => $hipay_user_account,
    'hipay_bank_account' => $account,
  );

  // Perform the call to Hipay Wallet API.
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_REGISTER, $parameters, $context);

  return $response;
}

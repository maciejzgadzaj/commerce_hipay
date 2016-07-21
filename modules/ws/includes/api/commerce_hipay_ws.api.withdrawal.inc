<?php

/**
 * Executes a withdraw from the wallet account to the registered bank account.
 *
 * @param int $account_id
 *   The account from which we wil withdraw to fill the bank account.
 *
 * @param float $amount
 *   The amount to withdraw.
 *
 * @param string $currency_code
 *   The currency (EUR).
 *
 * @param string $label
 *   A label to distinguish the withdraw.
 *
 * @return int|bool
 *   The transfer id if successful, and false if it failed.
 */
//function commerce_hipay_ws_api_withdrawal($account_id, $amount, $currency_code, $label) {
//  // Make sure the vendor account exists.
//  if (!$account = commerce_hipay_ws_account_load_by_account_id($account_id)) {
//    watchdog('commerce_hipay_ws', 'Withdrawal: unable to load vendor account from ID @account_id', array(
//      '@account_id' => $account_id,
//    ), WATCHDOG_ERROR);
//    return FALSE;
//  }
//
//  // Make sure we have vendor API access credentials.
//  if (
//    empty($account['hipay_vendor_account']['wsLogin'])
//    || empty($account['hipay_vendor_account']['wsPassword'])
//  ) {
//    watchdog('commerce_hipay_ws', 'Withdrawal: missing Hipay API credentials for vendor account ID @account_id', array(
//      '@account_id' => $account_id,
//    ), WATCHDOG_ERROR);
//    return FALSE;
//  }
//
//  // Prepare Hipay API request.
//  $parameters = array(
//    'wsLogin' => $account['hipay_vendor_account']['wsLogin'],
//    'wsPassword' => $account['hipay_vendor_account']['wsPassword'],
//    'amount' => commerce_currency_amount_to_decimal($amount, $currency_code),
//    'label' => substr($label, 0, 32),
//  );
//
//  // Call Hipay API.
//  if (!$response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_WITHDRAWAL, $parameters)) {
//    watchdog('commerce_hipay_ws', 'Withdrawal: error when calling Hipay API', array(), WATCHDOG_ERROR);
//    return FALSE;
//  }
//
//  // Save the withdrawal transaction in the database.
//  // @TODO: Don't we actually want to save the withdrawal request
//  // before calling the API, so it is saved even if API returns an error?
//  // Request details.
//  $withdrawal = commerce_hipay_ws_withdrawal_new();
//  $withdrawal['account_id'] = $account_id;
//  $withdrawal['amount'] = $amount;
//  $withdrawal['currency_code'] = $currency_code;
//  $withdrawal['label'] = $label;
//  $withdrawal['created'] = $withdrawal['changed'] = time();
//  // Response details.
//  $withdrawal['status'] = $response['code'];
//  $withdrawal['status_message'] = $response['description'];
//  $withdrawal['transaction_id'] = $response['transactionPublicId'];
//  commerce_hipay_ws_withdrawal_save($withdrawal);
//
//  // Error code was returned in the response.
//  if (!isset($response['code']) || $response['code'] !== COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
//    watchdog('commerce_hipay_ws', 'Withdrawal: received error @error_code: @error_description', array(
//      '@error_code' => $response['code'],
//      '@error_description' => $response['description'],
//    ), WATCHDOG_ERROR);
//    return FALSE;
//  }
//
//  return $response['transactionPublicId'];
//}

/**
 * Executes a withdrawal of wallet money from Hipay user account to bank account.
 *
 * @param object $withdrawal
 *   A withdrawal entity to execute.
 *
 * @return array|false
 *   An array of Hipay Wallet API response values, or FALSE in case of failure.
 */
function commerce_hipay_ws_api_withdrawal_execute($withdrawal) {
  dpm($withdrawal, '$withdrawal');

  // Validate all withdrawal details.
  if (!commerce_hipay_ws_api_withdrawal_validate($withdrawal)) {
    $withdrawal->revision = TRUE;
    $withdrawal->log = 'Withdrawal execution declined: withdrawal validation failure.';
    commerce_hipay_ws_withdrawal_save($withdrawal);
    return FALSE;
  }

  // Load necessary data.
  $hipay_user_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($withdrawal->user_account_id);
  $payment_method_instance = commerce_hipay_ws_payment_method_instance();

  // Prepare Hipay API request.
  $parameters = array(
    'wsLogin' => $hipay_user_account->hipay_ws_login,
    'wsPassword' => $hipay_user_account->hipay_ws_password,
    'amount' => commerce_currency_amount_to_decimal($withdrawal->amount, $withdrawal->currency_code),
    'label' => substr($withdrawal->label, 0, 255),
  );

  // Call Hipay API.
  if (!$response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_WITHDRAWAL, $parameters)) {
    watchdog('commerce_hipay_ws', 'Withdrawal: error when calling Hipay API', array(), WATCHDOG_ERROR);
    return FALSE;
  }
  dpm($response, '$response');

  // Update the transfer entity with values received in Hipay response,
  // creating new revision.
  $withdrawal->remote_status = $response['code'];
  $withdrawal->description = $response['description'];
  $withdrawal->transaction_id = $response['transactionPublicId'];
  $withdrawal->revision = TRUE;

  // If transfer was executed successfully, update its status to 'executed'.
  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    $withdrawal->status = COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_EXECUTED;
    $withdrawal->log = 'Withdrawal executed successfully.';
  }
  else {
    $withdrawal->log = 'Withdrawal execution failed.';
  }

  // Save new transfer revision.
  commerce_hipay_ws_withdrawal_save($withdrawal);

  return;

  // Create new order revision if required.
  if (variable_get('commerce_order_auto_revision', TRUE)) {
    $order = commerce_order_load($transfer->order_id);
    $order->revision = TRUE;
    $order->log = (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) ? t('Transfer @transfer_id for line item @line_item_id executed successfully.', array('@transfer_id' => $transfer->transfer_id, '@line_item_id' => $transfer->line_item_id)) : t('Transfer @transfer_id execution for line item @line_item_id failed.', array('@transfer_id' => $transfer->transfer_id, '@line_item_id' => $transfer->line_item_id));
    commerce_order_save($order);
  }

  // Error code was returned in the response.
  if (!isset($response['code']) || $response['code'] !== COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    watchdog('commerce_hipay_ws', 'Transfer: received error @error_code: @error_description', array(
      '@error_code' => $response['code'],
      '@error_description' => $response['description'],
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  return $response;
}

/**
 * Validates withdrawal information.
 *
 * @param object $withdrawal
 *   A withdrawal entity to validate.
 *
 * @return bool
 *   A boolean indicating whether the withdrawal validates or not.
 *
 * @see commerce_hipay_ws_api_withdrawal_execute()
 */
function commerce_hipay_ws_api_withdrawal_validate($withdrawal) {
  // Validate the Hipay user account exists.
  if (!$hipay_user_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($withdrawal->user_account_id)) {
    watchdog('commerce_hipay_ws', 'Withdrawal: unable to load Hipay user account from ID @user_account_id for withdrawal ID @withdrawal_id.', array(
      '@user_account_id' => $withdrawal->user_account_id,
      '@withdrawal_id' => $withdrawal->withdrawal_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Validate the Hipay user account API access credentials.
  if (empty($hipay_user_account->hipay_ws_login) || empty($hipay_user_account->hipay_ws_password)) {
    watchdog('commerce_hipay_ws', 'Withdrawal: missing Hipay API credentials for Hipay user account ID @user_account_id for withdrawal ID @withdrawal_id.', array(
      '@user_account_id' => $hipay_user_account->hipay_account_id,
      '@withdrawal_id' => $withdrawal->withdrawal_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Validate the Hipay user account is identified.
  if ($hipay_user_account->hipay_identified == COMMERCE_HIPAY_WS_ACCOUNT_NOT_IDENTIFIED) {
    watchdog('commerce_hipay_ws', 'Withdrawal: Hipay user account ID @user_account_id is not identified for withdrawal ID @withdrawal_id.', array(
      '@user_account_id' => $hipay_user_account->hipay_account_id,
      '@withdrawal_id' => $withdrawal->withdrawal_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  return TRUE;
}

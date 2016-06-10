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
function commerce_hipay_ws_api_withdrawal($account_id, $amount, $currency_code, $label) {
  // Make sure the vendor account exists.
  if (!$account = commerce_hipay_ws_account_load_by_account_id($account_id)) {
    watchdog('commerce_hipay_ws', 'Withdrawal: unable to load vendor account from ID @account_id', array(
      '@account_id' => $account_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Make sure we have vendor API access credentials.
  if (
    empty($account['hipay_vendor_account']['wsLogin'])
    || empty($account['hipay_vendor_account']['wsPassword'])
  ) {
    watchdog('commerce_hipay_ws', 'Withdrawal: missing Hipay API credentials for vendor account ID @account_id', array(
      '@account_id' => $account_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Prepare Hipay API request.
  $parameters = array(
    'wsLogin' => $account['hipay_vendor_account']['wsLogin'],
    'wsPassword' => $account['hipay_vendor_account']['wsPassword'],
    'amount' => commerce_currency_amount_to_decimal($amount, $currency_code),
    'label' => substr($label, 0, 32),
  );

  // Call Hipay API.
  if (!$response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_WITHDRAWAL, $parameters)) {
    watchdog('commerce_hipay_ws', 'Withdrawal: error when calling Hipay API', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  // Save the withdrawal transaction in the database.
  // @TODO: Don't we actually want to save the withdrawal request
  // before calling the API, so it is saved even if API returns an error?
  // Request details.
  $withdrawal = commerce_hipay_ws_withdrawal_new();
  $withdrawal['account_id'] = $account_id;
  $withdrawal['amount'] = $amount;
  $withdrawal['currency_code'] = $currency_code;
  $withdrawal['label'] = $label;
  $withdrawal['created'] = $withdrawal['changed'] = time();
  // Response details.
  $withdrawal['status'] = $response['code'];
  $withdrawal['status_message'] = $response['description'];
  $withdrawal['transaction_id'] = $response['transactionPublicId'];
  commerce_hipay_ws_withdrawal_save($withdrawal);

  // Error code was returned in the response.
  if (!isset($response['code']) || $response['code'] !== COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    watchdog('commerce_hipay_ws', 'Withdrawal: received error @error_code: @error_description', array(
      '@error_code' => $response['code'],
      '@error_description' => $response['description'],
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  return $response['transactionPublicId'];
}

/**
 * Returns an initialized withdrawal array.
 *
 * @return array
 *   An array of initialized Hipay withdrawal details.
 */
function commerce_hipay_ws_withdrawal_new() {
  return array(
    'account_id' => 0,
    'amount' => 0,
    'currency_code' => commerce_default_currency(),
    'label' => '',
    'created' => '',
    'changed' => '',
    'status' => '',
    'status_message' => '',
    'transaction_id' => '',
    'notification' => '',
  );
}

/**
 * Loads Hipay transfers for provided conditions.
 *
 * @param array $conditions
 *   An array of conditions to load Hipay transfers for.
 *
 * @return array
 *   An array of Hipay transfers.
 */
function commerce_hipay_ws_withdrawal_load_multiple($withdrawal_ids = array(), $conditions = array()) {
  $select = db_select('commerce_hipay_ws_withdrawal', 'w')
    ->fields('w');
  if (!empty($withdrawal_ids)) {
    $select->condition('id', $withdrawal_ids, 'IN');
  }
  if (!empty($conditions)) {
    foreach ($conditions as $key => $value) {
      $select->condition($key, $value, '=');
    }
  }
  $result = $select->execute()
    ->fetchAll(PDO::FETCH_ASSOC);

  // Unserialize serialized fields.
  if (!empty($result) && is_array($result)) {
    $schema = drupal_get_schema('commerce_hipay_ws_withdrawal');
    foreach ($result as $key => $transfer) {
      foreach ($schema['fields'] as $field_name => $field) {
        if (!empty($field['serialize'])) {
          $result[$key][$field_name] = unserialize($result[$key][$field_name]);
        }
      }
    }
  }

  return $result;
}

/**
 * Loads Hipay withdrawal details for provided withdrawal transaction ID.
 *
 * @param int $transaction_id
 *   A withdrawal transaction ID to load Hipay withdrawal details for.
 *
 * @return array
 *   An array of Hipay withdrawal details.
 */
function commerce_hipay_ws_withdrawal_load_by_withdrawal_id($transaction_id) {
  $withdrawals = commerce_hipay_ws_withdrawal_load_multiple(array(), array('transaction_id' => $transaction_id));
  return reset($withdrawals);
}

/**
 * Saves Hipay withdrawal details to the database.
 *
 * @param array $withdrawal
 *   An array of Hipay withdrawal details.
 *
 * @return
 *   If the withdrawal insert or update failed, returns FALSE. If it succeeded,
 *   returns SAVED_NEW or SAVED_UPDATED, depending on the operation performed.
 */
function commerce_hipay_ws_withdrawal_save(&$withdrawal) {
  return drupal_write_record('commerce_hipay_ws_withdrawal', $withdrawal, isset($withdrawal['id']) ? 'id' : array());
}

<?php


/**
 * Executes a transfer of wallet money from one account to another.
 *
 * @param int $sender_account_id
 *   The wallet account from which we will remove money.
 *
 * @param int $recipient_account_id
 *   The wallet account to which we will put the money.
 *
 * @param int $amount
 *   The amount to transfer.
 *
 * @param string $currency_code
 *   The currency (EUR).
 *
 * @param string $public_label
 *   A public label for the transaction.
 *
 * @param string $private_label
 *   A private label to identify the transaction.
 *
 * @return int|bool
 *   The transfer id if successful, and false if it failed.
 */
function commerce_hipay_ws_api_transfer($sender_account_id, $recipient_account_id, $amount, $currency_code, $public_label, $private_label) {
  // Make sure the sender account exists.
  if (!$sender_account = commerce_hipay_ws_account_load_by_account_id($sender_account_id)) {
    watchdog('commerce_hipay_ws', 'Transfer: unable to load sender account from ID @sender_account_id', array(
      '@sender_account_id' => $sender_account_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Make sure the recipient account exists.
  if (!$recipient_account = commerce_hipay_ws_account_load_by_account_id($recipient_account_id)) {
    watchdog('commerce_hipay_ws', 'Transfer: unable to load recipient account from ID @recipient_account_id', array(
      '@recipient_account_id' => $recipient_account_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Make sure we have sender API access credentials.
  if (
    empty($sender_account['hipay_vendor_account']['wsLogin'])
    || empty($sender_account['hipay_vendor_account']['wsPassword'])
  ) {
    watchdog('commerce_hipay_ws', 'Transfer: missing Hipay API credentials for sender account ID @sender_account_id', array(
      '@sender_account_id' => $sender_account_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Prepare Hipay API request.
  $parameters = array(
    'wsLogin' => $sender_account['hipay_vendor_account']['wsLogin'],
    'wsPassword' => $sender_account['hipay_vendor_account']['wsPassword'],
    'amount' => commerce_currency_amount_to_decimal($amount, $currency_code),
    'recipientUserId' => $recipient_account_id,
    'publicLabel' => substr($public_label, 0, 32),
    'privateLabel' => substr($private_label, 0, 32),
  );

  // Call Hipay API.
  if (!$response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_TRANSFER, $parameters)) {
    watchdog('commerce_hipay_ws', 'Transfer: error when calling Hipay API', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  // Save the transfer transaction in the database.
  $transfer = commerce_hipay_ws_transfer_new();
  // Request details.
  $transfer['sender_account_id'] = $sender_account_id;
  $transfer['recipient_account_id'] = $parameters['recipientUserId'];
  $transfer['amount'] = commerce_currency_decimal_to_amount($response['amount'], $response['currency']);
  $transfer['currency_code'] = $response['currency'];
  $transfer['public_label'] = $response['publicLabel'];
  $transfer['private_label'] = $response['privateLabel'];
  $transfer['created'] = $transfer['changed'] = time();
  // Response details.
  $transfer['status'] = $response['code'];
  $transfer['status_message'] = $response['description'];
  $transfer['transaction_id'] = $response['transactionId'];
  commerce_hipay_ws_transfer_save($transfer);

  // Error code was returned in the response.
  if (!isset($response['code']) || $response['code'] !== COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    watchdog('commerce_hipay_ws', 'Transfer: received error @error_code: @error_description', array(
      '@error_code' => $response['code'],
      '@error_description' => $response['description'],
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  return $response['transactionId'];
}

/**
 * Returns an initialized transfer array.
 *
 * @return array
 *   An array of initialized Hipay transfer details.
 */
function commerce_hipay_ws_transfer_new() {
  return array(
    'sender_account_id' => 0,
    'recipient_account_id' => 0,
    'amount' => 0,
    'currency_code' => commerce_default_currency(),
    'public_label' => '',
    'private_label' => '',
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
function commerce_hipay_ws_transfer_load_multiple($transfer_ids = array(), $conditions = array()) {
  $select = db_select('commerce_hipay_ws_transfer', 't')
    ->fields('t');
  if (!empty($transfer_ids)) {
    $select->condition('id', $transfer_ids, 'IN');
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
    $schema = drupal_get_schema('commerce_hipay_ws_transfer');
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
 * Loads Hipay transfer details for provided transaction ID.
 *
 * @param int $transaction_id
 *   A transaction ID to load Hipay transfer details for.
 *
 * @return array
 *   An array of Hipay transfer details.
 */
function commerce_hipay_ws_transfer_load_by_transaction_id($transaction_id) {
  $transfers = commerce_hipay_ws_transfer_load_multiple(array(), array('transaction_id' => $transaction_id));
  return reset($transfers);
}

/**
 * Saves Hipay transfer details to the database.
 *
 * @param array $transfer
 *   An array of Hipay transfer details.
 *
 * @return
 *   If the transfer insert or update failed, returns FALSE. If it succeeded,
 *   returns SAVED_NEW or SAVED_UPDATED, depending on the operation performed.
 */
function commerce_hipay_ws_transfer_save(&$transfer) {
  return drupal_write_record('commerce_hipay_ws_transfer', $transfer, isset($transfer['id']) ? 'id' : array());
}

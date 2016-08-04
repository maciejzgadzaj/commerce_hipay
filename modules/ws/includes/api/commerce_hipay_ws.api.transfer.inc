<?php

/**
 * Executes a transfer of wallet money from one account to another.
 *
 * @param object $transfer
 *   A transfer entity to execute.
 *
 * @return array|false
 *   An array of Hipay Wallet API response values, or FALSE in case of failure.
 */
function commerce_hipay_ws_api_transfer_execute($transfer) {
  // Validate all transfer details.
  if (!commerce_hipay_ws_api_transfer_validate($transfer)) {
    $transfer->revision = TRUE;
    $transfer->log = 'Transfer execution declined: transfer validation failure.';
    commerce_hipay_ws_transfer_save($transfer);
    return FALSE;
  }

  // Load necessary data.
  $sender_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($transfer->sender_account_id);
  $recipient_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($transfer->recipient_account_id);
  $payment_method_instance = commerce_hipay_ws_payment_method_instance();

  // Prepare Hipay API request.
  $parameters = array(
    'wsLogin' => $sender_account->hipay_ws_login,
    'wsPassword' => $sender_account->hipay_ws_password,
    'amount' => number_format(commerce_currency_amount_to_decimal($transfer->amount, $transfer->currency_code), 2),
    'recipientUserId' => $recipient_account->hipay_account_id,
    'publicLabel' => substr($transfer->public_label, 0, 255),
    'privateLabel' => substr($transfer->private_label, 0, 255),
    'entity' => $payment_method_instance['settings']['entity'],
    'merchantUniqueId' => $transfer->transfer_id,
  );

  // Add request parameters array to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_WS_RESOURCE_SOAP_TRANSFER . '-request-' . time();
  $transfer->data['payload'][$payload_key] = $parameters;

  // Call Hipay API.
  if (!$response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_TRANSFER, $parameters)) {
    watchdog('commerce_hipay_ws', 'Transfer: error when calling Hipay API', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  // Add the response to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_WS_RESOURCE_SOAP_TRANSFER . '-response-' . time();
  $transfer->data['payload'][$payload_key] = $response;

  // Update the transfer entity with values received in Hipay response,
  // creating new revision.
  $transfer->revision = TRUE;
  $transfer->transaction_id = $response['transactionId'];
  $transfer->remote_status = $response['code'];
  $transfer->description = $response['description'];
  $transfer->amount = commerce_currency_decimal_to_amount($response['amount'], $response['currency']);
  $transfer->currency_code = $response['currency'];
  $transfer->public_label = $response['publicLabel'];
  $transfer->private_label = $response['privateLabel'];

  // If transfer was executed successfully, update its status to 'executed'.
  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    $transfer->status = COMMERCE_HIPAY_WS_TRANSFER_STATUS_EXECUTED;
    $transfer->log = 'Transfer executed successfully.';
  }
  else {
    $transfer->log = 'Transfer execution failed.';
  }

  // Save new transfer revision.
  commerce_hipay_ws_transfer_save($transfer);

  // Error code was returned in the response.
  if (!isset($response['code']) || $response['code'] !== COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    watchdog('commerce_hipay_ws', 'Transfer @transfer_id: received error @error_code: @error_description', array(
      '@transfer_id' => $transfer->transfer_id,
      '@error_code' => $response['code'],
      '@error_description' => $response['description'],
    ), WATCHDOG_ERROR);
  }

  // Create new order revision if required.
  if (variable_get('commerce_order_auto_revision', TRUE)) {
    if (!empty($transfer->hipay_ws_transfer_order)) {
      $transfer_wrapper = entity_metadata_wrapper('commerce_hipay_ws_transfer', $transfer);
      foreach ($transfer_wrapper->hipay_ws_transfer_order as $order_wrapper) {
        $order = $order_wrapper->value();
        $order->revision = TRUE;
        $order->log = (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) ? t('Transfer @transfer_id executed successfully.', array('@transfer_id' => $transfer->transfer_id)) : t('Transfer @transfer_id execution failed.', array('@transfer_id' => $transfer->transfer_id));
        commerce_order_save($order);
      }
    }
  }

  return $response;
}

/**
 * Validates transfer information.
 *
 * @param object $transfer
 *   A transfer entity to validate.
 *
 * @return bool
 *   A boolean indicating whether the transfer validates or not.
 *
 * @see commerce_hipay_ws_api_transfer_execute()
 */
function commerce_hipay_ws_api_transfer_validate($transfer) {
  // Validate the sender account exists.
  if (!$sender_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($transfer->sender_account_id)) {
    watchdog('commerce_hipay_ws', 'Transfer: unable to load sender account from ID @sender_account_id for transfer ID @transfer_id.', array(
      '@sender_account_id' => $transfer->sender_account_id,
      '@transfer_id' => $transfer->transfer_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Validate the recipient account exists.
  if (!$recipient_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($transfer->recipient_account_id)) {
    watchdog('commerce_hipay_ws', 'Transfer: unable to load recipient account from ID @recipient_account_id for transfer ID @transfer_id.', array(
      '@recipient_account_id' => $transfer->recipient_account_id,
      '@transfer_id' => $transfer->transfer_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Validate the sender account API access credentials.
  if (empty($sender_account->hipay_ws_login) || empty($sender_account->hipay_ws_password)) {
    watchdog('commerce_hipay_ws', 'Transfer: missing Hipay API credentials for sender account ID @sender_account_id for transfer ID @transfer_id.', array(
      '@sender_account_id' => $sender_account->hipay_account_id,
      '@transfer_id' => $transfer->transfer_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Validate the sender account is identified.
  if ($sender_account->hipay_identified == COMMERCE_HIPAY_WS_ACCOUNT_NOT_IDENTIFIED) {
    watchdog('commerce_hipay_ws', 'Transfer: sender account ID @sender_account_id is not identified for transfer ID @transfer_id.', array(
      '@sender_account_id' => $sender_account->hipay_account_id,
      '@transfer_id' => $transfer->transfer_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Validate the recipient account is identified.
  if ($recipient_account->hipay_identified == COMMERCE_HIPAY_WS_ACCOUNT_NOT_IDENTIFIED) {
    watchdog('commerce_hipay_ws', 'Transfer: recipient account ID @recipient_account_id is not identified for transfer ID @transfer_id.', array(
      '@recipient_account_id' => $recipient_account->hipay_account_id,
      '@transfer_id' => $transfer->transfer_id,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  return TRUE;
}

/**
 * Executes all transfers in pending status.
 */
function commerce_hipay_ws_api_transfer_execute_all_pending() {
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'commerce_hipay_ws_transfer')
    ->propertyCondition('status', COMMERCE_HIPAY_WS_TRANSFER_STATUS_PENDING, '=')
    ->execute();
  if (!empty($result['commerce_hipay_ws_transfer'])) {
    $transfers = entity_load('commerce_hipay_ws_transfer', array_keys($result['commerce_hipay_ws_transfer']));
    foreach ($transfers as $transfer) {
      commerce_hipay_ws_api_transfer_execute($transfer);
    }
  }
}

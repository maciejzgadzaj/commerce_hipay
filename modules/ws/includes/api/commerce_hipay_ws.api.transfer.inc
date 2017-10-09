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
function commerce_hipay_ws_api_transfer_execute(&$transfer) {
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
    'wsSubAccountId' => $transfer->sender_account_id,
    'amount' => number_format(commerce_currency_amount_to_decimal($transfer->amount, $transfer->currency_code), 2, '.', ''),
    'recipientUserId' => $recipient_account->hipay_account_id,
    'publicLabel' => substr($transfer->public_label, 0, 255),
    'privateLabel' => substr($transfer->private_label, 0, 255),
    'entity' => $payment_method_instance['settings']['entity'],
    'merchantUniqueId' => $transfer->transfer_id,
  );
  $context = array(
    'commerce_hipay_ws_transfer' => $transfer,
    'currency_code' => commerce_hipay_ws_user_account_get_currency($sender_account),
  );

  // Add request parameters array to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_WS_RESOURCE_SOAP_TRANSFER . '-request-' . time();
  $transfer->data['payload'][$payload_key] = $parameters;

  // Call Hipay API.
  if (!$response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_TRANSFER, $parameters, $context)) {
    watchdog('commerce_hipay_ws', 'Transfer: error when calling Hipay API', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  // Add the response to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_WS_RESOURCE_SOAP_TRANSFER . '-response-' . time();
  $transfer->data['payload'][$payload_key] = $response;

  // Update the transfer entity with values received in Hipay response,
  // creating new revision.
  $transfer->revision = TRUE;

  // If transfer was executed successfully, update its status to 'executed'.
  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    $transfer->transaction_id = $response['transactionId'];
    $transfer->remote_status = $response['code'];
    $transfer->description = $response['description'];
    $transfer->amount = commerce_currency_decimal_to_amount($response['amount'], $response['currency']);
    $transfer->currency_code = $response['currency'];
    $transfer->public_label = $response['publicLabel'];
    $transfer->private_label = $response['privateLabel'];
    $transfer->status = COMMERCE_HIPAY_WS_TRANSFER_STATUS_EXECUTED;
    $transfer->executed = time();
    $transfer->log = 'Transfer executed successfully.';
  }
  else {
    if (isset($response['code'])) {
      $transfer->remote_status = $response['code'];
    }
    if (isset($response['description'])) {
      $transfer->description = $response['description'];
    }
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

/**
 * Processes Hipay Wallet transfer validation notification.
 *
 * @param array $feedback
 *   An array of XML parameters received in the notification.
 *
 * @return bool
 *   An boolean indicating whether the notification was processed successfully.
 *
 * @see commerce_hipay_ws_callback_notification_process_feedback_xml()
 */
function commerce_hipay_ws_api_transfer_validate_notification($feedback) {
  // Make sure we have received all required parameters.
  $required_parameters = array('account_id', 'transid', 'status');
  foreach ($required_parameters as $required_parameter) {
    if (empty($feedback[$required_parameter])) {
      watchdog('commerce_hipay_ws', 'Notification: transfer @operation: !param_name parameter missing or empty.', array(
        '@operation' => $feedback['operation'],
        '!param_name' => $required_parameter,
      ), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // It might happen that we receive multiple notifications for the same Hipay
  // transfer at (almost) the same moment (for example "authorization" and
  // "capture" notification), let's then acquire a lock for each update,
  // so they are processed one by one, to avoid later update overwriting
  // previous notification's update.
  $lock_name = commerce_hipay_ws_get_lock_name('commerce_hipay_ws_transfer', $feedback['transid']);
  if (!lock_acquire($lock_name)) {
    lock_wait($lock_name);
    return commerce_hipay_ws_api_transfer_validate_notification($feedback);
  }

  // Try to load the Hipay transfer entity from the received parameter.
  if (!$transfer = commerce_hipay_ws_transfer_load_by_transaction_id($feedback['transid'])) {
    watchdog('commerce_hipay_ws', 'Notification: transfer @operation: unable to load a Hipay transfer entity from transid parameter: @transid.', array(
      '@operation' => $feedback['operation'],
      '@transid' => $feedback['transid'],
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Make sure the notification account_id matches the transfer account_id.
  if ($feedback['account_id'] != $transfer->sender_account_id) {
    watchdog('commerce_hipay_ws', 'Notification: transfer @operation: notification account_id @account_id does not match transfer sender_account_id @sender_account_id for transaction @transid.', array(
      '@operation' => $feedback['operation'],
      '@account_id' => $feedback['account_id'],
      '@sender_account_id' => $transfer->sender_account_id,
      '@transid' => $feedback['transid'],
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Store the full notification in entity data.
  $payload_key = 'notification-' . $feedback['operation'] . '-' . REQUEST_TIME;
  $transfer->data['payload'][$payload_key] = $feedback;

  // Update the Hipay transfer entity with the new status.
  if ($feedback['status'] == COMMERCE_HIPAY_WS_NOTIFICATION_STATUS_OK) {
    if ($feedback['operation'] == 'capture') {
      $transfer->status = COMMERCE_HIPAY_WS_TRANSFER_STATUS_CAPTURED;
    } elseif ($transfer->status != COMMERCE_HIPAY_WS_TRANSFER_STATUS_CAPTURED) {
      $transfer->status = COMMERCE_HIPAY_WS_TRANSFER_STATUS_AUTHORIZED;
    }
  }
  else {
    $transfer->status = COMMERCE_HIPAY_WS_TRANSFER_STATUS_FAILED;
  }
  $transfer->revision = TRUE;
  $transfer->log = 'Updated status from Hipay Wallet notification.';
  commerce_hipay_ws_transfer_save($transfer);

  // Add watchdog entry.
  watchdog('commerce_hipay_ws', 'Notification: transfer @operation: Updated Hipay transfer @transfer_id status to @status.', array(
    '@operation' => $feedback['operation'],
    '@transfer_id' => $transfer->transfer_id,
    '@status' => $transfer->status,
  ), WATCHDOG_NOTICE);

  // Create new revisions of all related orders, if order auto-revisioning
  // is enabled.
  if (variable_get('commerce_order_auto_revision', TRUE)) {
    $transfer_wrapper = entity_metadata_wrapper('commerce_hipay_ws_transfer', $transfer);
    foreach ($transfer_wrapper->hipay_ws_transfer_order as $order_wrapper) {
      $order = $order_wrapper->value();
      $order->revision = TRUE;
      $order->log = ($feedback['status'] == COMMERCE_HIPAY_WS_NOTIFICATION_STATUS_OK) ? t('Transfer @transfer_id @operation has been successful.', array(
        '@transfer_id' => $transfer->transfer_id,
        '@operation' => $feedback['operation'],
      )) : t('Transfer @transfer_id @operation failed.', array(
        '@transfer_id' => $transfer->transfer_id,
        '@operation' => $feedback['operation'],
      ));
      commerce_order_save($order);
    }
  }

  // Invoke the rules event to allow other modules to react to the notification.
  $update_result = TRUE;
  foreach (rules_invoke_all('commerce_hipay_ws_transfer_validation', $transfer, $feedback) as $result) {
    // If any of the hook implementations returns FALSE, alter the function
    // update result respectively.
    $update_result = $update_result && $result;
  }

  // We are done updating, we can release the lock now.
  lock_release($lock_name);

  return $update_result;
}

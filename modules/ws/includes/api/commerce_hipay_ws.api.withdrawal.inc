<?php

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
    'amount' => number_format(commerce_currency_amount_to_decimal($withdrawal->amount, $withdrawal->currency_code), 2),
    'label' => substr($withdrawal->label, 0, 255),
  );

  // Add request parameters array to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_WS_RESOURCE_SOAP_WITHDRAWAL . '-request-' . time();
  $withdrawal->data['payload'][$payload_key] = $parameters;

  // Call Hipay API.
  if (!$response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_WITHDRAWAL, $parameters)) {
    watchdog('commerce_hipay_ws', 'Withdrawal: error when calling Hipay API', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  // Add the response to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_WS_RESOURCE_SOAP_WITHDRAWAL . '-response-' . time();
  $withdrawal->data['payload'][$payload_key] = $response;

  // Update the withdrawal entity with values received in Hipay response,
  // creating new revision.
  $withdrawal->remote_status = $response['code'];
  $withdrawal->description = $response['description'];
  $withdrawal->transaction_id = $response['transactionPublicId'];
  $withdrawal->revision = TRUE;

  // If withdrawal was executed successfully, update its status to 'executed'.
  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    $withdrawal->status = COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_EXECUTED;
    $withdrawal->log = 'Withdrawal executed successfully.';
  }
  else {
    $withdrawal->log = 'Withdrawal execution failed.';
  }

  // Save new withdrawal revision.
  commerce_hipay_ws_withdrawal_save($withdrawal);

  // Error code was returned in the response.
  if (!isset($response['code']) || $response['code'] !== COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    watchdog('commerce_hipay_ws', 'Withdrawal @withdrawal_id: received error @error_code: @error_description', array(
      '@withdrawal_id' => $withdrawal->withdrawal_id,
      '@error_code' => $response['code'],
      '@error_description' => $response['description'],
    ), WATCHDOG_ERROR);
  }

  // Create new revisions of all related transfers and orders if required.
  if (!empty($withdrawal->hipay_ws_withdrawal_transfer)) {
    $withdrawal_wrapper = entity_metadata_wrapper('commerce_hipay_ws_withdrawal', $withdrawal);
    foreach ($withdrawal_wrapper->hipay_ws_withdrawal_transfer as $transfer_wrapper) {
      // Create new transfer revision.
      $transfer = $transfer_wrapper->value();
      $transfer->revision = TRUE;
      if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
        // Update transfer status to 'withdrawal_executed'.
        $transfer->status = COMMERCE_HIPAY_WS_TRANSFER_STATUS_WITHDRAWAL_EXECUTED;
        $transfer->log = t('Executed withdrawal @withdrawal_id.', array('@withdrawal_id' => $withdrawal->withdrawal_id));
      }
      else {
        $transfer->log = t('Withdrawal @withdrawal_id execution failed.', array('@withdrawal_id' => $withdrawal->withdrawal_id));
      }
      commerce_hipay_ws_transfer_save($transfer);

      // Create new revisions of all related orders, if order auto-revisioning
      // is enabled.
      if (variable_get('commerce_order_auto_revision', TRUE) && !empty($transfer->hipay_ws_transfer_order)) {
        foreach ($transfer_wrapper->hipay_ws_transfer_order as $order_wrapper) {
          $order = $order_wrapper->value();
          $order->revision = TRUE;
          $order->log = (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) ? t('Withdrawal @withdrawal_id for transfer @transfer_id executed successfully.', array(
            '@withdrawal_id' => $withdrawal->withdrawal_id,
            '@transfer_id' => $transfer->transfer_id,
          )) : t('Withdrawal @withdrawal_id execution for transfer @transfer_id failed.', array(
            '@withdrawal_id' => $withdrawal->withdrawal_id,
            '@transfer_id' => $transfer->transfer_id,
          ));
          commerce_order_save($order);
        }
      }
    }
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

/**
 * Processes Hipay Wallet withdrawal validation notification.
 *
 * @param array $feedback
 *   An array of XML parameters received in the notification.
 *
 * @return bool
 *   An boolean indicating whether the notification was processed successfully.
 *
 * @see commerce_hipay_ws_callback_notification_process_feedback_xml()
 */
function commerce_hipay_ws_api_withdrawal_validate_notification($feedback) {
  // Make sure we have received all required parameters.
  $required_parameters = array('account_id', 'transid', 'status');
  foreach ($required_parameters as $required_parameter) {
    if (empty($feedback[$required_parameter])) {
      watchdog('commerce_hipay_ws', 'Notification: withdraw_validation: !param_name parameter missing or empty.', array('!param_name' => $required_parameter), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // Try to load the Hipay withdrawal entity from the received parameter.
  if (!$withdrawal = commerce_hipay_ws_withdrawal_load_by_transaction_id($feedback['transid'])) {
    watchdog('commerce_hipay_ws', 'Notification: withdraw_validation: unable to load a Hipay withdrawal entity from transid parameter: @transid.', array('@transid' => $feedback['transid']), WATCHDOG_ERROR);
    return FALSE;
  }

  // Make sure the notification account_id matches the withdrawal account_id.
  if ($feedback['account_id'] != $withdrawal->user_account_id) {
    watchdog('commerce_hipay_ws', 'Notification: withdraw_validation: notification account_id @account_id does not match withdrawal user_account_id @user_account_id for transaction @transid.', array(
      '@account_id' => $feedback['account_id'],
      '@user_account_id' => $withdrawal->user_account_id,
      '@transid' => $feedback['transid'],
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Store the full notification in entity data.
  $payload_key = 'notification-' . $feedback['operation'] . '-' . REQUEST_TIME;
  $withdrawal->data['payload'][$payload_key] = $feedback;

  // Update the Hipay withdrawal entity with the new status.
  $withdrawal->status = ($feedback['status'] == COMMERCE_HIPAY_WS_NOTIFICATION_STATUS_OK) ? COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_VALIDATED : COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_FAILED;
  $withdrawal->revision = TRUE;
  $withdrawal->log = 'Updated status from Hipay Wallet notificaton.';
  commerce_hipay_ws_withdrawal_save($withdrawal);

  // Add watchdog entry.
  watchdog('commerce_hipay_ws', 'Notification: withdraw_validation: Updated Hipay withdrawal @withdrawal_id status to @status.', array(
    '@withdrawal_id' => $withdrawal->withdrawal_id,
    '@status' => $withdrawal->status,
  ), WATCHDOG_NOTICE);

  // Create new revisions of all related transfers and orders if required.
  $withdrawal_wrapper = entity_metadata_wrapper('commerce_hipay_ws_withdrawal', $withdrawal);
  foreach ($withdrawal_wrapper->hipay_ws_withdrawal_transfer as $transfer_wrapper) {
    // Create new transfer revision.
    $transfer = $transfer_wrapper->value();
    $transfer->revision = TRUE;
    if ($feedback['status'] == COMMERCE_HIPAY_WS_NOTIFICATION_STATUS_OK) {
      // Update transfer status to 'withdrawal_validated'.
      $transfer->status = COMMERCE_HIPAY_WS_TRANSFER_STATUS_WITHDRAWAL_VALIDATED;
      $transfer->log = t('Withdrawal @withdrawal_id has been validated.', array('@withdrawal_id' => $withdrawal->withdrawal_id));
    }
    else {
      $transfer->status = COMMERCE_HIPAY_WS_TRANSFER_STATUS_WITHDRAWAL_FAILED;
      $transfer->log = t('Withdrawal @withdrawal_id validation failed.', array('@withdrawal_id' => $withdrawal->withdrawal_id));
    }
    commerce_hipay_ws_transfer_save($transfer);

    // Create new revisions of all related orders, if order auto-revisioning
    // is enabled.
    if (variable_get('commerce_order_auto_revision', TRUE) && !empty($withdrawal->hipay_ws_withdrawal_transfer)) {
      foreach ($transfer_wrapper->hipay_ws_transfer_order as $order_wrapper) {
        $order = $order_wrapper->value();
        $order->revision = TRUE;
        $order->log = ($feedback['status'] == COMMERCE_HIPAY_WS_NOTIFICATION_STATUS_OK) ? t('Withdrawal @withdrawal_id for transfer @transfer_id has been validated.', array(
          '@withdrawal_id' => $withdrawal->withdrawal_id,
          '@transfer_id' => $transfer->transfer_id,
        )) : t('Withdrawal @withdrawal_id validation for transfer @transfer_id failed.', array(
          '@withdrawal_id' => $withdrawal->withdrawal_id,
          '@transfer_id' => $transfer->transfer_id,
        ));
        commerce_order_save($order);
      }
    }
  }

  // Invoke the rules event to allow other modules to react to the notification.
  $update_result = TRUE;
  foreach (rules_invoke_all('commerce_hipay_ws_withdrawal_validation', $withdrawal, $feedback) as $result) {
    // If any of the hook implementations returns FALSE, alter the function
    // update result respectively.
    $update_result = $update_result && $result;
  }

  return $update_result;
}

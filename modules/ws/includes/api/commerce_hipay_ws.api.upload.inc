<?php

/**
 * Uploads a KYC document file to Hipay Wallet.
 *
 * @param object $account
 *   A Hipay user account to upload a KYC document for.
 * @param array $kyc_document
 *   A KYC document file entity to be uploaded to Hipay Wallet.
 * @param array $payment_method_instance
 *   A payment method instance to use when uploading a KYC document.
 *
 * @return array
 *   An array of Hipay Wallet API response parameters.
 */
function commerce_hipay_ws_api_upload($account, $kyc_document, $payment_method_instance = NULL) {
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

  // This requires PHP 5.5 or later.
  $file = new CURLFile(drupal_realpath($kyc_document['uri']), 'application/octet-stream', $kyc_document['filename']);

  $parameters = array(
    'user_space' => $account->hipay_space_id,
    'type' => $kyc_document['kyc_type'],
    'file' => $file,
  );
  if (!empty($kyc_document['kyc_validity'])) {
    $parameters['validity_date'] = date('Y-m-d', $kyc_document['kyc_validity']);
  }

  // Add request parameters array to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_WS_RESOURCE_REST_UPLOAD . '-request-' . time();
  $account->data['payload'][$payload_key] = $parameters;

  $context = array(
    'hipay_user_account' => $account,
    'kyc_document' => $kyc_document,
  );

  // Perform the call to Hipay Wallet API.
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_REST_UPLOAD, $parameters, $context, FALSE);

  // Add the response to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_WS_RESOURCE_REST_UPLOAD . '-response-' . time();
  $account->data['payload'][$payload_key] = $response;

  // If everything went fine, add new uploaded date to the KYC document
  // file field.
  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    // In commerce_hipay_ws_user_account_view_hipay_kyc_upload_form_submit()
    // we added the delta of the file currently being uploaded to $kyc_document
    // array.
    $delta = $kyc_document['delta'];
    $account->hipay_ws_user_kyc_documents[LANGUAGE_NONE][$delta]['kyc_uploaded'] = time();
    commerce_hipay_ws_file_kyc_document_update('commerce_hipay_ws_user_account', $account);

    // Log message for a new revision.
    $account->log = t('Uploaded %filename (@kyc_type) to Hipay Wallet.', array(
      '%filename' => $kyc_document['filename'],
      '@kyc_type' => $kyc_document['kyc_type'],
    ));
  }
  else {
    $account->log = t('Failed uploading %filename (@kyc_type) to Hipay Wallet.', array(
      '%filename' => $kyc_document['filename'],
      '@kyc_type' => $kyc_document['kyc_type'],
    ));
  }

  // Create a new revision to keep track of all uploads.
  $account->revision = TRUE;
  commerce_hipay_ws_user_account_save($account);

  return $response;
}

/**
 * Processes Hipay Wallet document validation notification.
 *
 * @param array $feedback
 *   An array of XML parameters received in the notification.
 *
 * @return bool
 *   An boolean indicating whether the notification was processed successfully.
 *
 * @see commerce_hipay_ws_callback_notification_process_feedback_xml()
 */
function commerce_hipay_ws_api_upload_validate_notification($feedback) {
  // Make sure we have received all required parameters.
  $required_parameters = array('account_id', 'document_type', 'status');
  foreach ($required_parameters as $required_parameter) {
    if (empty($feedback[$required_parameter])) {
      watchdog('commerce_hipay_ws', 'Notification: document_validation: !param_name parameter missing or empty.', array('!param_name' => $required_parameter), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // Try to load the Hipay user account from the received parameter.
  if (!$hipay_user_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($feedback['account_id'])) {
    watchdog('commerce_hipay_ws', 'Notification: document_validation: unable to load a Hipay user account entity from account_id parameter: @account_id.', array('@account_id' => $feedback['account_id']), WATCHDOG_ERROR);
    return FALSE;
  }

  // Store the full notification in entity data.
  $payload_key = 'notification-' . $feedback['operation'] . '-' . REQUEST_TIME;
  $hipay_user_account->data['payload'][$payload_key] = $feedback;

  $update_result = FALSE;
  if (!empty($hipay_user_account->hipay_ws_user_kyc_documents[LANGUAGE_NONE])) {
    foreach ($hipay_user_account->hipay_ws_user_kyc_documents[LANGUAGE_NONE] as $delta => &$kyc_document) {
      if ($kyc_document['kyc_type'] == $feedback['document_type']) {

        // Update custom KYC document file field properties.
        $kyc_document['kyc_status'] = $feedback['status'];
        $kyc_document['kyc_message'] = $feedback['notification_message'];

        // Save new custom KYC document file field properties.
        commerce_hipay_ws_file_kyc_document_update('commerce_hipay_ws_user_account', $hipay_user_account);

        // Create new Hipay user account revision.
        $hipay_user_account->revision = TRUE;
        $hipay_user_account->log = t('Updated KYC document %filename (@kyc_type) status to %kyc_status from received Hipay Wallet notification.', array(
          '%filename' => $kyc_document['filename'],
          '@kyc_type' => $kyc_document['kyc_type'],
          '%kyc_status' => $kyc_document['kyc_status'],
        ));

        // Add watchdog entry.
        watchdog('commerce_hipay_ws', 'Notification: document_validation: Updated Hipay user account @account_id KYC document @document_id status to %kyc_status (@kyc_message).', array(
          '@account_id' => $hipay_user_account->user_account_id,
          '@document_id' => $kyc_document['fid'],
          '%kyc_status' => $feedback['status'],
          '@kyc_message' => $feedback['notification_message'],
        ), WATCHDOG_NOTICE);

        // Mark update as successful.
        $update_result = TRUE;
      }
    }
  }

  // Save new Hipay user account entity revision (if it was created)
  // and notification payload in entity data.
  commerce_hipay_ws_user_account_save($hipay_user_account);

  // If $update_result at this point is still FALSE, it means that we were
  // unable to find a KYC document of the provided kyc_type in this Hipay user
  // account.
  if ($update_result === FALSE) {
    watchdog('commerce_hipay_ws', 'Notification: document_validation: unable to find a KYC document type @kyc_type in Hipay user account @user_account_id (@hipay_account_id).', array(
      '@kyc_type' => $feedback['document_type'],
      '@user_account_id' => $hipay_user_account->user_account_id,
      '@hipay_account_id' => $feedback['account_id'],
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Invoke the rules event to allow other modules to react to the notification.
  foreach (rules_invoke_all('commerce_hipay_ws_document_validation', $hipay_user_account, $feedback) as $result) {
    // If any of the hook implementations returns FALSE, alter the function
    // update result respectively.
    $update_result = $update_result && $result;
  }

  return $update_result;
}

/**
 * Returns types of KYC documents (classification numbers).
 *
 * @return array
 *   An array of types of KYC documents.
 */
function commerce_hipay_ws_api_upload_kyc_document_types() {
  return array(
    t('For individuals') => array(
      1 => t('Identification documents'),
      2 => t('Proof of address'),
      6 => t('Bank account details'),
    ),
    t('For professionals (legal entities)') => array(
      3 => t('Identification document of the legal representative'),
      4 => t('Document certifying company registration (Kbis extract)'),
      5 => t('Signed articles of association with the division of powers'),
      6 => t('Bank account details'),
    ),
    t('For professionals (physical persons)') => array(
      7 => t('Identification document'),
      8 => t('Document certifying registration'),
      9 => t('Document certifying tax status'),
      6 => t('Bank account details'),
    ),
  );
}

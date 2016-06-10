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
  $file = new CURLFile(drupal_realpath($kyc_document['uri']),'application/octet-stream', $kyc_document['filename']);

  $parameters = array(
    'user_space' => $account->hipay_space_id,
    'type' => $kyc_document['kyc_type'],
    'file' => $file,
  );
  if (!empty($kyc_document['kyc_validity'])) {
    $parameters['validity_date'] = date('Y-m-d', $kyc_document['kyc_validity']);
  }

  $context = array(
    'hipay_user_account' => $account,
    'kyc_document' => $kyc_document,
  );

  // Perform the call to Hipay Wallet API.
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_REST_UPLOAD, $parameters, $context, FALSE);

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

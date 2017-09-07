<?php

// https://developer.hipay.com/getting-started/platform-hipay-enterprise/overview/#sepa-direct-debit-sdd
// https://support.hipay.com/hc/en-us/articles/213882649-How-can-I-test-payment-methods


/**
 * Creates a Direct Debit mandate in Hipay TPP.
 *
 * @param object $direct_debit
 *   A Direct Debit entity to create a Direct Debit mandate in Hipay from.
 * @param array $payment_method
 *   A payment method instance to use when creating a Direct Debit.
 * @param array $context
 *   An associative array containing additional information about the request.
 *
 * @return array
 *   An array of Hipay TPP API response parameters.
 */
function commerce_hipay_tpp_api_direct_debit_create($direct_debit, $payment_method = NULL, $context = array()) {
  // If no payment method instance was provided in the function parameter,
  // try to load the first available one.
  if (
    !$payment_method
    && ($instances = commerce_hipay_tpp_payment_method_instances('commerce_hipay_tpp_direct_debit', TRUE))
  ) {
    $payment_method = reset($instances);
  }
  // If still no payment method instance is available, we cannot continue.
  if (empty($payment_method)) {
    watchdog('commerce_hipay_tpp_direct_debit', 'Unable to load @method_id payment method instance.', array('@method_id' => 'commerce_hipay_tpp_direct_debit'), WATCHDOG_ERROR);
    return FALSE;
  }

  $request_data = array(
    'payment_product' => 'sdd',
    'gender' => $direct_debit->gender,
    'firstname' => $direct_debit->first_name,
    'lastname' => $direct_debit->last_name,
    'bank_name' => $direct_debit->bank_name,
    'issuer_bank_id' => $direct_debit->issuer_bank_id,
    'iban' => $direct_debit->iban,
    'authentication_indicator' => 0,
  );

  // Allow other modules to alter the $request_data array.
  drupal_alter('commerce_hipay_tpp_api_direct_debit_create', $request_data, $payment_method);

  // Add request parameters array to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_TPP_RESOURCE_DIRECT_DEBIT_CREATE . '-request-' . time();
  $direct_debit->data['payload'][$payload_key] = $request_data;

  $context += array(
    'call_type' => 'Direct Debit create',
    'commerce_hipay_tpp_direct_debit' => $direct_debit,
  );

  // Perform the call to Hipay Wallet API.
  $url = commerce_hipay_tpp_get_server_url($payment_method, 'v2') . COMMERCE_HIPAY_TPP_RESOURCE_DIRECT_DEBIT_CREATE;
  $response = commerce_hipay_tpp_api_request($url, $request_data, $payment_method, $context, 'commerce_hipay_tpp_direct_debit');

  // Add the response to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_TPP_RESOURCE_DIRECT_DEBIT_CREATE . '-response-' . time();
  $direct_debit->data['payload'][$payload_key] = $response;

  // If everything went fine, create new revision of Virtual IBAN entity
  // to keep track of the account creation in Hipay TPP.
  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_TPP_REST_STATUS_SUCCESS) {
    $direct_debit->debit_agreement_id = $response['debit_agreement']['id'];
    $direct_debit->debit_agreement_status = $response['debit_agreement']['status'];
    $direct_debit->log = 'Created mandate in Hipay.';
  }
  else {
    $direct_debit->log = 'Failed creating mandate in Hipay.';
  }

  // Save the new Virtual IBAN entity revision.
  $direct_debit->revision = TRUE;
  commerce_hipay_tpp_direct_debit_save($direct_debit);

  return $response;
}

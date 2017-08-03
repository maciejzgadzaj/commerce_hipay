<?php

/**
 * Creates a Virtual IBAN in Hipay TPP.
 *
 * @param object $virtual_iban
 *   A Virtual IBAN entity to create an Virtual IBAN account in Hipay from.
 * @param array $payment_method
 *   A payment method instance to use when creating a Virtual IBAN.
 * @param array $context
 *   An associative array containing additional information about the request.
 *
 * @return array
 *   An array of Hipay TPP API response parameters.
 */
function commerce_hipay_tpp_api_virtual_iban_create($virtual_iban, $payment_method = NULL, $context = array()) {
  // If external_reference value is already set, it means that Virtual IBAN
  // was already created, and we cannot create another one.
//  if (!empty($virtual_iban->external_reference)) {
//    watchdog('commerce_hipay_tpp', 'A Virtual IBAN has already been created in Hipay TPP for this Virtual IBAN entity @virtual_iban_id.', array('@virtual_iban_id' => $virtual_iban->virtual_iban_id), WATCHDOG_ERROR);
//    return FALSE;
//  }

  // If no payment method instance was provided in the function parameter,
  // try to load the first available one.
  if (
    !$payment_method
    && ($instances = commerce_hipay_tpp_payment_method_instances('commerce_hipay_tpp_virtual_iban', TRUE))
  ) {
    $payment_method = reset($instances);
  }
  // If still no payment method instance is available, we cannot continue.
  if (empty($payment_method)) {
    watchdog('commerce_hipay_tpp_virtual_iban', 'Unable to load @method_id payment method instance.', array('@method_id' => 'commerce_hipay_tpp_virtual_iban'), WATCHDOG_ERROR);
    return FALSE;
  }

  $request_data = array(
    'external_reference' => !empty($virtual_iban->external_reference) ? $virtual_iban->external_reference : 'VI-' . $virtual_iban->virtual_iban_id,
    'expiry_month' => date('m', $virtual_iban->expires),
    'expiry_year' => date('Y', $virtual_iban->expires),
  );

  // Allow other modules to alter the $request_data array.
  drupal_alter('commerce_hipay_tpp_api_virtual_iban_create', $request_data, $payment_method);

  // Add request parameters array to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_TPP_RESOURCE_VIRTUAL_IBAN_CREATE . '-request-' . time();
  $virtual_iban->data['payload'][$payload_key] = $request_data;

  $context += array(
    'call_type' => 'Virtual IBAN create',
    'commerce_hipay_tpp_virtual_iban' => $virtual_iban,
  );

  // Perform the call to Hipay Wallet API.
  $url = commerce_hipay_tpp_get_server_url($payment_method, 'v2') . COMMERCE_HIPAY_TPP_RESOURCE_VIRTUAL_IBAN_CREATE;
  $response = commerce_hipay_tpp_api_request($url, $request_data, $payment_method, $context, 'commerce_hipay_tpp_virtual_iban');

  // Add the response to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_TPP_RESOURCE_VIRTUAL_IBAN_CREATE . '-response-' . time();
  $virtual_iban->data['payload'][$payload_key] = $response;

  // If everything went fine, create new revision of Virtual IBAN entity
  // to keep track of the account creation in Hipay TPP.
  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_TPP_REST_STATUS_SUCCESS) {
    $virtual_iban->bic = $response['virtual_iban']['bic'];
    $virtual_iban->iban = $response['virtual_iban']['iban'];
    $virtual_iban->external_reference = $response['virtual_iban']['external_reference'];
    $virtual_iban->expires = strtotime($response['virtual_iban']['expiry_date']);
    $virtual_iban->log = 'Created Virtual IBAN in Hipay TPP.';
  }
  else {
    $virtual_iban->log = 'Failed creating Virtual IBAN in Hipay TPP.';
  }

  // Save the new Virtual IBAN entity revision.
  $virtual_iban->revision = TRUE;
  commerce_hipay_tpp_virtual_iban_save($virtual_iban);

  return $response;
}

/**
 * Searches for Virtual IBAN in Hipay TPP.
 *
 * @param string $external_reference
 *   A Virtual IBAN external reference to search for in Hipay TPP.
 * @param array $payment_method
 *   A payment method instance to use when searching for Virtual IBAN
 *   external reference.
 * @param array $context
 *   An associative array containing additional information about the request.
 *
 * @return array
 *   An array of Hipay TPP API response parameters.
 */
function commerce_hipay_tpp_api_virtual_iban_search($external_reference, $payment_method = NULL, $context = array()) {
  // If no payment method instance was provided in the function parameter,
  // try to load the first available one.
  if (
    !$payment_method
    && ($instances = commerce_hipay_tpp_payment_method_instances('commerce_hipay_tpp_virtual_iban', TRUE))
  ) {
    $payment_method = reset($instances);
  }
  // If still no payment method instance is available, we cannot continue.
  if (empty($payment_method)) {
    watchdog('commerce_hipay_tpp_virtual_iban', 'Unable to load @method_id payment method instance.', array('@method_id' => 'commerce_hipay_tpp'), WATCHDOG_ERROR);
    return FALSE;
  }

  $request_data = array(
    'external_reference' => $external_reference,
  );

  // Allow other modules to alter the $request_data array.
  drupal_alter('commerce_hipay_tpp_api_virtual_iban_search', $request_data, $payment_method);

  $context += array(
    'call_type' => 'Virtual IBAN search',
  );

  // Perform the call to Hipay Wallet API.
  $url = commerce_hipay_tpp_get_server_url($payment_method, 'v2') . COMMERCE_HIPAY_TPP_RESOURCE_VIRTUAL_IBAN_SEARCH;
  $response = commerce_hipay_tpp_api_request($url, $request_data, $payment_method, $context, 'commerce_hipay_tpp_virtual_iban');

  return $response;
}

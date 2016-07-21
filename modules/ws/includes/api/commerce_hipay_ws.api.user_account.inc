<?php

/**
 * Creates a user account in Hipay Wallet.
 *
 * @param object $account
 *   A Hipay user account entity to create an account from.
 * @param array $payment_method_instance
 *   A payment method instance to use when creating an account.
 *
 * @return array
 *   An array of Hipay TPP API response parameters.
 */
function commerce_hipay_ws_api_user_account_create($account, $payment_method_instance = NULL) {
  // If hipay_account_id value is already set, it means that Hipay User Account
  // was already created, and we cannot create another one.
  if (!empty($account->hipay_account_id)) {
    // @TODO: watchdog?
    return FALSE;
  }

  $wrapper = entity_metadata_wrapper('commerce_hipay_ws_user_account', $account);
  $address = $wrapper->hipay_ws_user_address->value();

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

  $parameters = array(
    'userAccountBasic' => array(
      'email' => $wrapper->hipay_ws_user_email->value(),
      'title' => $wrapper->hipay_ws_user_title->value(),
      'firstname' => $wrapper->hipay_ws_user_first_name->value(),
      'lastname' => $wrapper->hipay_ws_user_last_name->value(),
      'currency' => $wrapper->hipay_ws_user_currency->value(),
      'locale' => $wrapper->hipay_ws_user_locale->value(),
      'ipAddress' => ip_address(),
      'entity' => $payment_method_instance['settings']['entity'],
    ),
    'userAccountDetails' => array(
      'address' => commerce_hipay_ws_combine_address_lines($address, array('thoroughfare', 'premise')),
      'zipCode' => $address['postal_code'],
      'city' => $address['locality'],
      'country' => $address['country'],
      'timeZone' => $wrapper->hipay_ws_user_time_zone->value(),
      'contactEmail' => $wrapper->hipay_ws_user_contact_email->value(),
      'phoneNumber' => $wrapper->hipay_ws_user_phone_number->value(),
      'termsAgreed' => 1,
      'callbackUrl' => url('commerce-hipay-ws/notify', array('absolute' => TRUE)),
      'legalStatus' => $wrapper->hipay_ws_user_legal_status->value(),
      'structure' => $wrapper->hipay_ws_user_structure->value(),
      'companyName' => $wrapper->hipay_ws_user_company_name->value(),
      'directorRole' => $wrapper->hipay_ws_user_director_role->value(),
      // @TODO: We do not handle Brazilian account data yet.
//      'cpf' => '',
//      'identificationNumberType' => '',
//      'identificationNumber' => '',
      // @TODO: No idea yet what this is for.
//      'state' => '',
      'birthDate' => date('d/m/Y', $wrapper->hipay_ws_user_birth_date->value()),
      'mobilePhoneNumber' => $wrapper->hipay_ws_user_mobile_number->value(),
      'faxNumber' => $wrapper->hipay_ws_user_fax_number->value(),
      'europeanVATNumber' => $wrapper->hipay_ws_user_vat_number->value(),
      'businessId' => $wrapper->hipay_ws_user_business_id->value(),
      'businessLineId' => $wrapper->hipay_ws_user_business_line_id->value(),
      // @TODO: We do not handle these yet either.
//      'antiPhishingKey' => '',
//      'receiveHipayInformation' => 0,
//      'receiveCommercialInformation' => 0,
    ),
  );

  // If we have website data already configured, add it to the call as well.
  if (
    ($business_line_id = variable_get('commerce_hipay_ws_website_business_line_id'))
    && ($topic_id = variable_get('commerce_hipay_ws_website_topic_id'))
  ) {
    $parameters['websites'] = array(
      array(
        'websiteBusinessLineId' => $business_line_id,
        'websiteTopicId' => $topic_id,
        'websiteContactEmail' => variable_get('commerce_hipay_ws_website_contact_email', variable_get('site_mail', ini_get('sendmail_from'))),
        'websiteName' => variable_get('commerce_hipay_ws_website_name', variable_get('site_name', 'Drupal')),
        'websiteUrl' => variable_get('commerce_hipay_ws_website_url', $GLOBALS['base_url']),
        'websiteMerchantPassword' => variable_get('commerce_hipay_ws_website_merchant_password', ''),
      ),
    );
  }

  $context = array(
    'hipay_user_account' => $account,
  );

  // Perform the call to Hipay Wallet API.
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_CREATE_FULL_USER_ACCOUNT, $parameters, $context);

  // If everything went fine, add new Hipay user account data to the entity.
  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    $account->hipay_account_id = $response['userAccountId'];
    $account->hipay_space_id = $response['userSpaceId'];
    $account->hipay_ws_login = $response['wsLogin'];
    $account->hipay_ws_password = $response['wsPassword'];
    $account->revision = TRUE;
    $account->log = 'Created user account in Hipay Wallet.';
    commerce_hipay_ws_user_account_save($account);
  }

  return $response;
}

/**
 * Processes Hipay Wallet user account identification notification.
 *
 * @param array $feedback
 *   An array of XML parameters received in the notification.
 *
 * @return bool
 *   An boolean indicating whether the notification was processed successfully.
 */
function commerce_hipay_ws_api_user_account_validate_notification($feedback) {
  // Make sure we have received all required parameters.
  $required_parameters = array('account_id', 'status');
  foreach ($required_parameters as $required_parameter) {
    if (empty($feedback[$required_parameter])) {
      watchdog('commerce_hipay_ws', 'Notification: identification: !param_name parameter missing or empty.', array('!param_name' => $required_parameter), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // Try to load the Hipay user account from the received parameter.
  if (!$user_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($feedback['account_id'])) {
    watchdog('commerce_hipay_ws', 'Notification: identification: unable to load a Hipay user account entity from account_id parameter: @account_id.', array('@account_id' => $feedback['account_id']), WATCHDOG_ERROR);
    return FALSE;
  }

  // Update the Hipay user account entity with the new identification status.
  $user_account->hipay_identified = ($feedback['status'] == COMMERCE_HIPAY_WS_NOTIFICATION_STATUS_OK) ? COMMERCE_HIPAY_WS_ACCOUNT_IDENTIFIED : COMMERCE_HIPAY_WS_ACCOUNT_NOT_IDENTIFIED;
  $user_account->revision = TRUE;
  $user_account->log = 'Updated identified status from received Hipay Wallet notificaton.';
  commerce_hipay_ws_user_account_save($user_account);

  // Add watchdog entry.
  watchdog('commerce_hipay_ws', 'Notification: identification: Updated Hipay user account %account_id identified status to %status.', array(
    '%account_id' => $user_account->user_account_id,
    '%status' => $user_account->hipay_identified,
  ), WATCHDOG_NOTICE);

  // Invoke the rules event to allow other modules to react to the notification.
  return rules_invoke_all('commerce_hipay_ws_user_account_identification', $user_account, $feedback);
}

/**
 * Get information on an account.
 *
 * @param string $user_account_id
 *   The hipay id of the account we wish to get the information of.
 *
 * @return array
 *   The information or empty array.
 */
function commerce_hipay_ws_api_user_account_get_infos($user_account_id, $context = array()) {
  $parameters = array('accountId' => $user_account_id);
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_ACCOUNT_INFOS, $parameters);

  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    // If we are checking account info status for a Hipay user account entity,
    // and received status differs from the status saved in the entity,
    // let's update it, creating a new revision.
    if (
      !empty($context['commerce_hipay_ws_user_account'])
      && $context['commerce_hipay_ws_user_account']->hipay_identified != $response['identified']
    ) {
      $hipay_user_account = $context['commerce_hipay_ws_user_account'];
      $hipay_user_account->hipay_identified = $response['identified'];
      $hipay_user_account->revision = TRUE;
      $hipay_user_account->log = t('Updated identified value from %resource API call response.', array('%resource' => COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_ACCOUNT_INFOS));
      commerce_hipay_ws_user_account_save($hipay_user_account);
    }
  }

  return $response;
}

/**
 * Check if an account is identified.
 *
 * @param string $user_account_id
 *   The hipay id of the account we wish to check.
 *
 * @return bool
 *   True if the account is identified, false if not.
 */
function commerce_hipay_ws_api_user_account_check_identified($user_account_id) {
  $account_info = commerce_hipay_ws_api_user_account_get_infos($user_account_id);

  if (empty($account_info) || empty($account_info['identified']) || $account_info['identified'] == 'no') {
    return FALSE;
  }

  return TRUE;
}

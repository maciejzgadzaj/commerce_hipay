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
      )
    );
  }

  $context = array(
    'hipay_user_account' => $account,
  );

  // Perform the call to Hipay Wallet API.
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_CREATE_FULL_USER_ACCOUNT, $parameters, $context);

  // If everything went fine, add new Hipay user account data to the entity.
  if (isset($response['code']) && $response['code'] === 0) {
    $account->hipay_account_id = $response['userAccountId'];
    $account->hipay_space_id = $response['userSpaceId'];
    $account->hipay_ws_login = $response['wsLogin'];
    $account->hipay_ws_password = $response['wsPassword'];
    commerce_hipay_ws_user_account_save($account);
  }

  return $response;
}

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
      'legalStatus' => $wrapper->hipay_ws_user_legal_status->value(),
      'structure' => $wrapper->hipay_ws_user_structure->value(),
      'companyName' => $wrapper->hipay_ws_user_company_name->value(),
      'directorRole' => $wrapper->hipay_ws_user_director_role->value(),
      'cpf' => $wrapper->hipay_ws_user_cpf->value(),
      'identificationNumberType' => $wrapper->hipay_ws_user_ident_type->value(),
      'identificationNumber' => $wrapper->hipay_ws_user_ident_number->value(),
      // @TODO: For the moment Hipay doesn't know how to handle states properly.
      // See https://support.hipay.com/hc/en-us/requests/58175
//      'state' => !empty($address['administrative_area']) ? $address['administrative_area'] : '',
      'birthDate' => date('d/m/Y', $wrapper->hipay_ws_user_birth_date->value()),
      'mobilePhoneNumber' => $wrapper->hipay_ws_user_mobile_number->value(),
      'faxNumber' => $wrapper->hipay_ws_user_fax_number->value(),
      'europeanVATNumber' => $wrapper->hipay_ws_user_vat_number->value(),
      'businessId' => $wrapper->hipay_ws_user_business_id->value(),
      'businessLineId' => $wrapper->hipay_ws_user_business_line_id->value(),
      'antiPhishingKey' => $wrapper->hipay_ws_user_antiphishing_key->value(),
      'receiveHipayInformation' => $wrapper->hipay_ws_user_receive_hipay_info->value() ? 1 : 0,
      'receiveCommercialInformation' => $wrapper->hipay_ws_user_receive_com_info->value() ? 1 : 0,
    ),
  );

  if ($payment_method_instance['settings']['api']['user_account']['callback_url'] == COMMERCE_HIPAY_WS_API_CALLBACK_URL_SITE_BASE_URL) {
    $parameters['userAccountDetails']['callbackUrl'] = url('commerce-hipay-ws/notify', array('absolute' => TRUE));
  }
  elseif (
    $payment_method_instance['settings']['api']['user_account']['callback_url'] == COMMERCE_HIPAY_WS_API_CALLBACK_URL_CUSTOM_URL
    && !empty($payment_method_instance['settings']['api']['user_account']['callback_url_custom'])
  ) {
    $parameters['userAccountDetails']['callbackUrl'] = $payment_method_instance['settings']['api']['user_account']['callback_url_custom'];
  }

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

  // Add request parameters array to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_WS_RESOURCE_SOAP_CREATE_FULL_USER_ACCOUNT . '-request-' . time();
  $account->data['payload'][$payload_key] = $parameters;

  $context = array(
    'commerce_hipay_ws_user_account' => $account,
    'currency_code' => $wrapper->hipay_ws_user_currency->value(),
  );

  // Perform the call to Hipay Wallet API.
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_CREATE_FULL_USER_ACCOUNT, $parameters, $context);

  // Add the response to the user account entity payload.
  $payload_key = COMMERCE_HIPAY_WS_RESOURCE_SOAP_CREATE_FULL_USER_ACCOUNT . '-response-' . time();
  $account->data['payload'][$payload_key] = $response;

  // If everything went fine, add new Hipay user account data to the entity.
  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    $account->hipay_account_id = $response['userAccountId'];
    $account->hipay_space_id = $response['userSpaceId'];
    $account->hipay_ws_login = $response['wsLogin'];
    $account->hipay_ws_password = $response['wsPassword'];
    $account->log = 'Created user account in Hipay Wallet.';
  }
  else {
    $account->log = 'Failed creating user account in Hipay Wallet.';
  }

  // Save the new user account entity revision.
  $account->revision = TRUE;
  commerce_hipay_ws_user_account_save($account);

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
 *
 * @see commerce_hipay_ws_callback_notification_process_feedback_xml()
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

  // Store the full notification in entity data.
  $payload_key = 'notification-' . $feedback['operation'] . '-' . REQUEST_TIME;
  $user_account->data['payload'][$payload_key] = $feedback;

  // Create new revision and save updated user account entity.
  $user_account->revision = TRUE;
  $user_account->log = 'Updated identified status from received Hipay Wallet notificaton.';
  commerce_hipay_ws_user_account_save($user_account);

  // Also update statuses of all KYC documents to validated.
  if (!empty($user_account->hipay_ws_user_kyc_documents[LANGUAGE_NONE])) {
    foreach ($user_account->hipay_ws_user_kyc_documents[LANGUAGE_NONE] as $delta => &$kyc_document) {
      // Except these KYC documents that were rejected before.
      if ($kyc_document['kyc_status'] != COMMERCE_HIPAY_WS_KYC_STATUS_NOK) {
        $kyc_document['kyc_status'] = COMMERCE_HIPAY_WS_KYC_STATUS_VALIDATED;
        $kyc_document['kyc_message'] = 'Account identified';
      }
    }
    commerce_hipay_ws_file_kyc_document_update('commerce_hipay_ws_user_account', $user_account);
  }

  // Add watchdog entry.
  watchdog('commerce_hipay_ws', 'Notification: identification: Updated Hipay user account %account_id identified status to %status.', array(
    '%account_id' => $user_account->user_account_id,
    '%status' => $user_account->hipay_identified,
  ), WATCHDOG_NOTICE);

  // Invoke the rules event to allow other modules to react to the notification.
  $update_result = TRUE;
  foreach (rules_invoke_all('commerce_hipay_ws_user_account_identification', $user_account, $feedback) as $result) {
    // If any of the hook implementations returns FALSE, alter the function
    // update result respectively.
    $update_result = $update_result && $result;
  }

  return $update_result;
}

/**
 * Get information on an account.
 *
 * @param string $user_account_id
 *   The hipay id of the account we wish to get the information of.
 * @param array $context
 *   An associative array containing additional information about the request.
 *
 * @return array
 *   The information or empty array.
 */
function commerce_hipay_ws_api_user_account_get_infos($user_account_id, $context = array()) {
  $parameters = array('accountId' => $user_account_id);

  if ($hipay_user_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($user_account_id)) {
    $context['currency_code'] = commerce_hipay_ws_user_account_get_currency($hipay_user_account);
  }

  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_ACCOUNT_INFOS, $parameters, $context);

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
 * Returns balance of the Hipay user account.
 *
 * @param string $user_account_id
 *   An ID of the Hipay user account to get the balance for.
 * @param array $context
 *   An array of request context.
 *
 * @return array|null
 *   An array of Hipay user account balance information.
 */
function commerce_hipay_ws_api_user_account_get_balance($user_account_id, $context = array()) {
  $parameters = array('wsSubAccountId' => $user_account_id);

  $hipay_user_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($user_account_id);
  $context['currency_code'] = commerce_hipay_ws_user_account_get_currency($hipay_user_account);

  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_BALANCE, $parameters, $context);

  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS && !empty($response['balances']['item'])) {
    return $response['balances']['item'];
  }
}

/**
 * Check if an account is identified.
 *
 * @param string $user_account_id
 *   The hipay id of the account we wish to check.
 * @param array $context
 *   An associative array containing additional information about the request.
 *
 * @return bool
 *   True if the account is identified, false if not.
 */
function commerce_hipay_ws_api_user_account_check_identified($user_account_id, $context = array()) {
  $hipay_user_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($user_account_id);
  $context['currency_code'] = commerce_hipay_ws_user_account_get_currency($hipay_user_account);

  $account_info = commerce_hipay_ws_api_user_account_get_infos($user_account_id, $context);

  if (empty($account_info) || empty($account_info['identified']) || $account_info['identified'] == 'no') {
    return FALSE;
  }

  return TRUE;
}

/**
 * Returns a list of transactions for a Hipay user account.
 *
 * @TODO: Refactor it to handle limit/offset/ordering?
 * Currently a call to Hipay Wallet's "getTransactions" API returns only 4
 * transactions per page, and there is no way to change it, meaning that to get
 * all transactions we need to make multiple calls. Also there is no way of
 * ordering the transaction based on any criteria.
 * See http://help.hipay.com/Tickets/Ticket/View/13943
 *
 * @param int $hipay_account_id
 *   A Hipay user account to return the list of transactions for.
 * @param int $start_date
 *   A timestamp which to start fetching transaction at.
 * @param int $end_date
 *   A timestamp which to end fetching transaction at.
 * @param array $context
 *   An associative array containing additional information about the request.
 *
 * @return array
 *   An array of transactions for a Hipay user account.
 */
function commerce_hipay_ws_api_user_account_get_transactions($hipay_account_id, $start_date = NULL, $end_date = NULL, $context = array()) {
  $transactions = array();

  $hipay_user_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($hipay_account_id);

  if (!empty($start_date)) {
    $start_timestamp = commerce_hipay_ws_valid_timestamp($start_date) ? $start_date : strtotime($start_date);
  }
  // Hipay allows to get transactions for max period of 3 months.
  else {
    $three_months_ago = strtotime('-3 months');
    $start_timestamp = ($hipay_user_account->created > $three_months_ago) ? $hipay_user_account->created : $three_months_ago;
  }
  $end_timestamp = !empty($end_date) ? (commerce_hipay_ws_valid_timestamp($end_date) ? $end_date : strtotime($end_date)) : REQUEST_TIME;

  // Hipay servers run on UTC time, so we need to convert our time
  // to match theirs.
  $start_date = format_date($start_timestamp, 'custom', 'Y-m-d H:i:s', 'UTC');
  $end_date = format_date($end_timestamp, 'custom', 'Y-m-d H:i:s', 'UTC');

  $parameters = array(
    'wsSubAccountId' => $hipay_account_id,
    'startDate' => $start_date,
    'endDate' => $end_date,
    'pageNumber' => 1,
  );
  $context['commerce_hipay_ws_user_account'] = $hipay_user_account;
  $context['currency_code'] = commerce_hipay_ws_user_account_get_currency($hipay_user_account);

  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_TRANSACTIONS, $parameters, $context);

  if (
    isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS
    && isset($response['transactions']['item'])
  ) {
    // If there is only one transaction, 'item', we need to put it into
    // a sub-array (as is the case with multiple transactions).
    if (!isset($response['transactions']['item'][0])) {
      $response['transactions']['item'] = array($response['transactions']['item']);
    }

    $transactions = $response['transactions']['item'];

    // Hipay paginates the result, returning only 4 transactions per page.
    // Therefore, if there is more than one page of transactions, we need
    // to fetch all other pages as well.
    if ($response['pageCount'] > 1) {
      for ($page = 2; $page <= $response['pageCount']; $page++) {
        $parameters['pageNumber'] = $page;
        $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_TRANSACTIONS, $parameters, $context);
        if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
          // If there is only one transaction, 'item', we need to put it into
          // a sub-array (as is the case with multiple transactions).
          if (!isset($response['transactions']['item'][0])) {
            $response['transactions']['item'] = array($response['transactions']['item']);
          }

          $transactions = array_merge($transactions, $response['transactions']['item']);
        }
      }
    }
  }

  return array_reverse($transactions);
}

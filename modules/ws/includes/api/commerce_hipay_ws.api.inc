<?php

/**
 * @file
 * Provides integration with Hipay Wallet.
 */

/**
 * Returns Hipay Wallet country code for provided country label.
 *
 * @param string $country_label
 *   A country label to return the country code for.
 *
 * @return string
 *   A country code for provided country label.
 */
function commerce_hipay_ws_get_code_from_country($country_label) {
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_LOCALE_COUNTRIES);

  if (!empty($response['countries']['item']) && is_array($response['countries']['item'])) {
    foreach ($response['countries']['item'] as $item) {
      if ($item['label'] == $country_label) {
        return $item['code'];
      }
    }
  }
}

/**
 * Check the balance of a sender account.
 *
 * @param array $sender_account
 *   The sender account to check the balance of.
 *
 * @param float $amount
 *   The amount the account should have.
 *
 * @param string $item_name
 *   The balance item to check.
 *   Should be either "balance" or "moneyAvailableForWithdrawal"
 *
 * @return bool
 *   True if there is the necessary amount.
 */
function commerce_hipay_ws_check_balance($sender_account, $amount, $item_name = 'balance') {

  static $balances = array();

  $ws_login = $sender_account['hipay_vendor_account']['wsLogin'];

  if (isset($balances[$ws_login])) {
    $response = $balances[$ws_login];
  } else {
    $parameters = array(
      'wsLogin' => $sender_account['hipay_vendor_account']['wsLogin'],
      'wsPassword' => $sender_account['hipay_vendor_account']['wsPassword'],
    );
    $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_BALANCE, $parameters);

    $balances[$ws_login] = $response;
  }

  $balance = 0;
  if (isset($response['code']) && $response['code'] == COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    $balance = $response['balances']['item'][$item_name];
  }

  return $amount < $balance;
}

/**
 * Check the balance to see if it has the funds for the transfer.
 *
 * @param array $sender_account
 *   The sender account to check the balance of.
 *
 * @param float $amount
 *   The amount the account should have.
 *
 * @return bool
 *   True if there is the necessary amount.
 */
function commerce_hipay_ws_check_transfer_balance($sender_account, $amount) {
  return commerce_hipay_ws_check_balance($sender_account, $amount);
}

/**
 * Check the balance to see if it has the funds for the witdraw.
 *
 * @param array $sender_account
 *   The sender account to check the balance of.
 *
 * @param float $amount
 *   The amount the account should have.
 *
 * @return bool
 *   True if there is the necessary amount.
 */
function commerce_hipay_ws_check_withdraw_balance($sender_account, $amount) {
  return commerce_hipay_ws_check_balance($sender_account, $amount, 'moneyAvailableForWithdrawal');
}

/**
 * Get the bank information of an account
 *
 * @param string $ws_login
 *   Login for the account to get the information of.
 *
 * @param string $ws_password
 *   Login for the account to get the information of.
 *
 * @return array|object
 *   Empty if no information, if not bank information.
 */
function commerce_hipay_ws_get_bank_info($ws_login, $ws_password) {
  // Bank account status.
  $parameters = array(
    'wsLogin' => $ws_login,
    'wsPassword' => $ws_password,
  );
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_STATUS, $parameters);
  if (isset($response['code']) && $response['code'] == COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    return $response;
  }

  return array();
}

/**
 * Get the required bank field needed for an Hipay account.
 *
 * @param string $country
 *   Country code of the bank account.
 *
 * @return array
 *   Empty if no information, if not required bank fields information.
 */
function commerce_hipay_ws_get_bank_fields($country) {
  // Bank account status.
  $parameters = array(
    'country' => $country,
  );
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_FIELDS, $parameters);
  if (isset($response['code']) && $response['code'] == COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    return $response;
  }

  return array();
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
function commerce_hipay_ws_get_account_infos($user_account_id) {
  $parameters = array('accountId' => $user_account_id);
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_ACCOUNT_INFOS, $parameters);
  if (isset($response['code']) && $response['code'] == COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    return $response;
  }

  return array();
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
function commerce_hipay_ws_check_account_identified($user_account_id) {
  $account_info = commerce_hipay_ws_get_account_infos($user_account_id);

  if (empty($account_info) || empty($account_info['identified']) || $account_info['identified'] == 'no') {
    return FALSE;
  }

  return TRUE;
}

/**
 * Returns the list of countries supported by Hipay Wallet.
 *
 * @return array|false
 *   An array of Hipay Wallet API response parameters, where 'countries' key
 *   contains the list of countries supported by Hipay Wallet, or FALSE on error.
 */
function commerce_hipay_ws_get_countries() {
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_LOCALE_COUNTRIES);

  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    return $response;
  }

  return FALSE;
}

/**
 * Returns the list of locales supported by Hipay Wallet.
 *
 * @return array|false
 *   An array of Hipay Wallet API response parameters, where 'locales' key
 *   contains the list of locales supported by Hipay Wallet, or FALSE on error.
 */
function commerce_hipay_ws_get_locales() {
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_LOCALE_CODES);

  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    return $response;
  }

  return FALSE;
}

/**
 * Returns the list of timezones supported by Hipay Wallet.
 *
 * @return array|false
 *   An array of Hipay Wallet API response parameters, where 'timezones' key
 *   contains the list of timezones supported by Hipay Wallet, or FALSE on error.
 */
function commerce_hipay_ws_get_timezones() {
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_LOCALE_TIMEZONES);

  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    return $response;
  }

  return FALSE;
}

/**
 * Returns the list of available business lines.
 *
 * @return array|false
 *   An array of Hipay Wallet API response parameters, where 'businessLines' key
 *   contains the list of available business lines, or FALSE on error.
 */
function commerce_hipay_ws_get_website_business_lines() {
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_BUSINESS_LINES);

  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    return $response;
  }

  return FALSE;
}

/**
 * Returns the list of available website topics.
 *
 * @param int $business_line_id
 *   A business line ID to return the list of website topics for.
 *
 * @return array|false
 *   An array of Hipay Wallet API response parameters, where 'websiteTopics' key
 *   contains the list of available website topics, or FALSE on error.
 */
function commerce_hipay_ws_get_website_topics($business_line_id) {
  $parameters = array('businessLineId' => $business_line_id);
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_WEBSITE_TOPICS, $parameters);

  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    return $response;
  }

  return FALSE;
}

/**
 * Returns an array of countries supported by Hipay Wallet as select options.
 *
 * @return array
 *   An array of countries supported by Hipay Wallet formatted as select options.
 */
function commerce_hipay_ws_get_countries_options() {
  $return = array();

  $response = commerce_hipay_ws_get_countries();
  if (!empty($response['countries']['item'])) {
    foreach ($response['countries']['item'] as $item) {
      $return[$item['code']] = t($item['label']);
    }
  }

  return $return;
}

/**
 * Returns an array of locales supported by Hipay Wallet as select options.
 *
 * @return array
 *   An array of locales supported by Hipay Wallet formatted as select options.
 */
function commerce_hipay_ws_get_locales_options() {
  $return = array();

  $response = commerce_hipay_ws_get_locales();
  if (!empty($response['locales']['item'])) {
    foreach ($response['locales']['item'] as $item) {
      $return[$item['code']] = t($item['label']);
    }
  }

  return $return;
}

/**
 * Returns an array of timezones supported by Hipay Wallet as select options.
 *
 * @return array
 *   An array of timezones supported by Hipay Wallet formatted as select options.
 */
function commerce_hipay_ws_get_timezones_options() {
  $return = array();

  $response = commerce_hipay_ws_get_timezones();
  if (!empty($response['timezones']['item'])) {
    foreach ($response['timezones']['item'] as $item) {
      $return[$item['code']] = t($item['label']);
    }
  }

  return $return;
}

/**
 * Returns an array of available business lines formatted as select options.
 *
 * @return array
 *   An array of available business lines formatted as select options.
 */
function commerce_hipay_ws_get_website_business_lines_options() {
  $return = array();

  $response = commerce_hipay_ws_get_website_business_lines();
  if (!empty($response['businessLines']['item'])) {
    foreach ($response['businessLines']['item'] as $item) {
      $return[$item['id']] = t($item['label']);
    }
  }

  return $return;
}

/**
 * Returns an array of available website topics formatted as select options
 * for provided business line ID.
 *
 * @return array
 *   An array of available website topics formatted as select options
 *   for provided business line ID.
 */
function commerce_hipay_ws_get_website_topics_options($business_line_id) {
  $return = array();

  $response = commerce_hipay_ws_get_website_topics($business_line_id);
  if (!empty($response['websiteTopics']['item'])) {
    foreach ($response['websiteTopics']['item'] as $item) {
      $return[$item['id']] = t($item['label']);
    }
  }

  return $return;
}

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
  $countries = array_flip(commerce_hipay_ws_get_countries_options());
  return (isset($countries[$country_label])) ? $countries[$country_label] : FALSE;
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
  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
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
 * Get the bank information for an account.
 *
 * @param array $parameters
 *   An array of parameters to pass to the API call.
 *
 * @return array
 *   An array of Hipay Wallet API response parameters.
 */
function commerce_hipay_ws_api_get_bank_info_status($parameters = array(), $context = array()) {
  // Fetch the bank account info status from Hipay Wallet API.
  $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_STATUS, $parameters, $context);

  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    // If we are checking bank info status for a Hipay bank account entity,
    // and received status differs from the status saved in the entity,
    // let's update it, creating a new revision.
    if (
      !empty($context['commerce_hipay_ws_bank_account'])
      && $context['commerce_hipay_ws_bank_account']->hipay_status != $response['status']
    ) {
      $hipay_bank_account = $context['commerce_hipay_ws_bank_account'];
      $hipay_bank_account->hipay_status = $response['status'];
      $hipay_bank_account->revision = TRUE;
      $hipay_bank_account->log = t('Updated bank account status from %resource API call response.', array('%resource' => COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_STATUS));
      commerce_hipay_ws_bank_account_save($hipay_bank_account);
    }
  }

  return $response;
}

/**
 * Get the required bank field needed for an Hipay account.
 *
 * @param string $country
 *   Country code of the bank account.
 * @param bool $reset
 *   A boolean indicating whether a cached value should be reset or not.
 *
 * @return array
 *   An array of Hipay Wallet API response parameters, where 'fields' key
 *   contains the list of required bank fields, or FALSE on error.
 */
function commerce_hipay_ws_api_get_bank_fields($country, $reset = FALSE) {
  $parameters = array('country' => $country);
  return commerce_hipay_ws_api_get_cached_response(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_FIELDS, $reset, $parameters);
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
  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
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
 * Returns a Hipay Wallet API response, fetched from cache or the API.
 *
 * @param string $resource
 *   A Hipay Wallet API resource to call if cached value was not found.
 * @param bool $reset
 *   A boolean indicating whether a cached value should be reset or not.
 *
 * @return mixed|false
 *   Returns a cached value of Hipay Wallet API response, or false if cached
 *   value was not found and there was an error calling Hipay Wallet API.
 */
function commerce_hipay_ws_api_get_cached_response($resource, $reset = FALSE) {
  $cached_response = &drupal_static(__FUNCTION__);

  // Build cache key from provided function arguments.
  $args = func_get_args();
  $cache_key = sha1(drupal_json_encode($args));

  // Get payment method instance to check if caching is enabled.
  $instance = commerce_hipay_ws_payment_method_instance();

  // If cache reset was requested, or caching is disabled, or the response
  // is not in the local static cache.
  if ($reset || empty($instance['settings']['api']['cache']) || !isset($cached_response[$resource])) {
    // If no cache reset was requested, and caching is enabled, and we have
    // our value in the cache, just add it to local static cache.
    if (!$reset && !empty($instance['settings']['api']['cache']) && $cache = cache_get('commerce_hipay_ws:response:' . $cache_key)) {
      $cached_response[$cache_key] = $cache->data;
    }
    // Otherwise we need to fetch the response from the Hipay Wallet API.
    else {
      array_shift($args);
      array_shift($args);
      array_unshift($args, $resource);
      // Call the function to make a Hipay Wallet API request,
      // passing all parameters we have received here.
      $response = call_user_func_array('commerce_hipay_ws_api_request', $args);

      if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
        // Store the response in static cache for this page execution.
        // This is fine even if caching is disabled, as the next time this
        // function will get called, if caching is disabled the response will
        // be re-fetched from the API anyway.
        $cached_response[$cache_key] = $response;
        // Store it in persistent cache for future page executions
        // (only if caching is enabled).
        if (!empty($instance['settings']['api']['cache'])) {
          cache_set('commerce_hipay_ws:response:' . $cache_key, $response, 'cache', REQUEST_TIME + $instance['settings']['api']['cache_lifetime']);
        }
      }
      // If there was an error fetching response from Hipay Wallet API,
      // just return FALSE, without storing the response anywhere.
      else {
        return FALSE;
      }
    }
  }

  return $cached_response[$cache_key];
}

/**
 * Returns the list of countries supported by Hipay Wallet.
 *
 * @param bool $reset
 *   A boolean indicating whether a cached value should be reset or not.
 *
 * @return array|false
 *   An array of Hipay Wallet API response parameters, where 'countries' key
 *   contains the list of countries supported by Hipay Wallet, or FALSE on error.
 */
function commerce_hipay_ws_api_get_countries($reset = FALSE) {
  return commerce_hipay_ws_api_get_cached_response(COMMERCE_HIPAY_WS_RESOURCE_SOAP_LOCALE_COUNTRIES, $reset);
}

/**
 * Returns the list of locales supported by Hipay Wallet.
 *
 * @param bool $reset
 *   A boolean indicating whether a cached value should be reset or not.
 *
 * @return array|false
 *   An array of Hipay Wallet API response parameters, where 'locales' key
 *   contains the list of locales supported by Hipay Wallet, or FALSE on error.
 */
function commerce_hipay_ws_api_get_locales($reset = FALSE) {
  return commerce_hipay_ws_api_get_cached_response(COMMERCE_HIPAY_WS_RESOURCE_SOAP_LOCALE_CODES, $reset);
}

/**
 * Returns the list of timezones supported by Hipay Wallet.
 *
 * @param bool $reset
 *   A boolean indicating whether a cached value should be reset or not.
 *
 * @return array|false
 *   An array of Hipay Wallet API response parameters, where 'timezones' key
 *   contains the list of timezones supported by Hipay Wallet, or FALSE on error.
 */
function commerce_hipay_ws_api_get_timezones($reset = FALSE) {
  return commerce_hipay_ws_api_get_cached_response(COMMERCE_HIPAY_WS_RESOURCE_SOAP_LOCALE_TIMEZONES, $reset);
}

/**
 * Returns the list of available business lines.
 *
 * @param bool $reset
 *   A boolean indicating whether a cached value should be reset or not.
 *
 * @return array|false
 *   An array of Hipay Wallet API response parameters, where 'businessLines' key
 *   contains the list of available business lines, or FALSE on error.
 */
function commerce_hipay_ws_api_get_website_business_lines($reset = FALSE) {
  return commerce_hipay_ws_api_get_cached_response(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_BUSINESS_LINES, $reset);
}

/**
 * Returns the list of available website topics.
 *
 * @param int $business_line_id
 *   A business line ID to return the list of website topics for.
 * @param bool $reset
 *   A boolean indicating whether a cached value should be reset or not.
 *
 * @return array|false
 *   An array of Hipay Wallet API response parameters, where 'websiteTopics' key
 *   contains the list of available website topics, or FALSE on error.
 */
function commerce_hipay_ws_api_get_website_topics($business_line_id, $reset = FALSE) {
  $parameters = array('businessLineId' => $business_line_id);
  return commerce_hipay_ws_api_get_cached_response(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_WEBSITE_TOPICS, $reset, $parameters);
}

/**
 * Returns an array of countries supported by Hipay Wallet as select options.
 *
 * @return array
 *   An array of countries supported by Hipay Wallet formatted as select options.
 */
function commerce_hipay_ws_get_countries_options() {
  $return = array();

  $response = commerce_hipay_ws_api_get_countries();
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

  $response = commerce_hipay_ws_api_get_locales();
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

  $response = commerce_hipay_ws_api_get_timezones();
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

  $response = commerce_hipay_ws_api_get_website_business_lines();
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

  $response = commerce_hipay_ws_api_get_website_topics($business_line_id);
  if (!empty($response['websiteTopics']['item'])) {
    foreach ($response['websiteTopics']['item'] as $item) {
      $return[$item['id']] = t($item['label']);
    }
  }

  return $return;
}

<?php

/**
 * @file
 * Provides admin functions for Commerce Hipay WS.
 */

/**
 * Returns default settings for the Hipay Wallet payment method.
 *
 * @see commerce_hipay_ws_settings_form()
 */
function commerce_hipay_ws_settings_default() {
  $defaults = array(
    'endpoint' => COMMERCE_HIPAY_WS_TEST,
    'account_id' => '',
    'email' => '',
    'entity' => '',
    'api_username' => '',
    'api_password' => '',
    'locale' => 'en_GB',
    'fees' => array(),
    'api' => array(
      'create_account_immediately' => TRUE,
      'test_mode' => FALSE,
      'logging' => array(
        'request' => FALSE,
        'response' => FALSE,
      ),
      'cache' => FALSE,
      'cache_lifetime' => 0,
      'user_account' => array(
        'callback_url' => 'dont_send',
        'callback_url_custom' => '',
      ),
    ),
    'website' => array(
      'business_line_id' => '',
      'topic_id' => '',
      'contact_email' => variable_get('site_mail'),
      'name' => variable_get('site_name'),
      'url' => $GLOBALS['base_url'],
      'merchant_password' => '',
    ),
    'bank_account' => array(
      'bankName' => '',
      'bankAddress' => '',
      'bankZipCode' => '',
      'bankCity' => '',
      'bankCountry' => '',
      'iban' => '',
      'swift' => '',
      'acct_num' => '',
      'aba_num' => '',
      'transit_num' => '',
    ),
  );

  foreach (array_keys(commerce_currencies(TRUE)) as $currency_code) {
    $defaults['fees'][$currency_code]['account_id'] = '';
    $defaults['fees'][$currency_code]['bank_account'] = $defaults['bank_account'];
  }

  return $defaults;
}

/**
 * Payment method callback: settings form.
 *
 * @see hook_commerce_payment_method_info()
 */
function commerce_hipay_ws_settings_form($settings = array()) {
  // Make sure the PHP SOAP extension is installed.
  if (!class_exists('SoapClient')) {
    drupal_set_message(t('SoapClient class not found! The Hipay Wallet settings form will not be available until PHP <a href="!soap_url">SOAP extension</a> is installed.', array('!soap_url' => 'http://php.net/manual/en/book.soap.php')), 'error');
    return array();
  }

  // Merge default settings into the stored settings array.
  // Settings are stored in multi-level array, so we need recursive merge. And
  // actually replace instead of merge, to make sure the default settings are
  // correctly overriden by custom user settings (array_merge_recursive() would
  // just add new array elements instead of overriding existing ones).
  $settings = array_replace_recursive(commerce_hipay_ws_settings_default(), $settings);

  // When the payment method is configured for the very first time, only part
  // of the settings form is displayed - as for the remaining part we need to
  // have API credentials provided first, to be able to make few API requests
  // to get data required for form element options etc.
  if (empty($settings['api_password'])) {
    drupal_set_message(t('Please note that this is only a part of the Hipay Wallet payment method settings form. After providing the Hipay Wallet API credentials below, the form will be refreshed to show all remaining options.'), 'warning');
  }

  $form = array();

  // Pass the original settings to the submit callback, so that we are know
  // if it is the very first time the payment method is being configured or not.
  $form['settings'] = array(
    '#type' => 'value',
    '#value' => $settings,
  );

  $form['endpoint'] = array(
    '#type' => 'radios',
    '#title' => t('Environment'),
    '#options' => array(
      COMMERCE_HIPAY_WS_TEST => t('Test - process test operations on the test account'),
      COMMERCE_HIPAY_WS_PROD => t('Production - process real operations on the production account'),
    ),
    '#default_value' => $settings['endpoint'],
    '#required' => TRUE,
  );

  $form['account_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Account ID'),
    '#description' => t('Hipay Wallet account ID.'),
    '#default_value' => $settings['account_id'],
    '#required' => TRUE,
  );

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Account email'),
    '#description' => t('Hipay Wallet account email address.'),
    '#default_value' => $settings['email'],
    '#required' => TRUE,
  );

  $form['entity'] = array(
    '#type' => 'textfield',
    '#title' => t('Entity'),
    '#description' => t('Entity of the accounts involved in API operations (provided by Hipay after Hipay Wallet account setup).'),
    '#default_value' => $settings['entity'],
    '#required' => TRUE,
  );

  $form['api_username'] = array(
    '#type' => 'textfield',
    '#title' => t('API username'),
    '#description' => t('The name of the user for accessing Hipay Wallet webservice. This, as well as API password, can be found in your Hipay Wallet Account under <em>Hipay wallet integration / Merchant Tool Kit / API</em>.'),
    '#default_value' => $settings['api_username'],
    '#required' => TRUE,
  );

  $form['api_password'] = array(
    '#type' => 'textfield',
    '#title' => t('API password'),
    '#description' => t('The password for the user specified in the above field.'),
    '#default_value' => $settings['api_password'],
    '#required' => TRUE,
  );

  $form['locale'] = array(
    '#type' => 'select',
    '#title' => t('Locale'),
    '#description' => t('Default API locale.'),
    '#options' => commerce_hipay_ws_get_supported_languages(),
    '#default_value' => $settings['locale'],
    '#required' => TRUE,
  );

  // API settings.
  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('API settings'),
    '#collapsible' => TRUE,
  );

  $form['api']['create_account_immediately'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create Hipay Wallet accounts immediately'),
    '#description' => t('When enabled, after each new Hipay user or bank account entity is created in Drupal, a relevant API will be called immediately to create it in the Hipay Wallet as well.'),
    '#default_value' => $settings['api']['create_account_immediately'],
  );

  $form['api']['test_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Test mode'),
    '#description' => t('When enabled, the email address of each vendor account created in Hipay Wallet will be altered with current timestamp (for example, when creating a user account for <em>email@domain.net</em>, it will be altered to <em>email+1463740832@domain.net</em>). This could be useful when testing Hipay Wallet integration - when creating a user account for the same email address multiple times, or from multiple environments - to avoid Hipay complaining that the user account for a provided email address has already been created.'),
    '#default_value' => $settings['api']['test_mode'],
  );

  $form['api']['logging'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Log the following messages for debugging'),
    '#description' => t('Enabling this option will save full information on each API request and/or response in the dblog.'),
    '#options' => array(
      'request' => t('API request messages'),
      'response' => t('API response messages'),
    ),
    '#default_value' => $settings['api']['logging'],
  );

  // API response caching.
  // Based on system_performance_settings().
  $form['api']['cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache API responses'),
    '#description' => t('Enable caching for generic API responses (countries, currencies, locales, timezones, business lines and website topics).'),
    '#default_value' => $settings['api']['cache'],
  );

  $day = 86400;
  $period = drupal_map_assoc(array($day, $day * 2, $day * 3, $day * 7, $day * 14, $day * 30), 'format_interval');
  $period[0] = '<' . t('none') . '>';
  $form['api']['cache_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Minimum cache lifetime'),
    '#description' => t('Cached API responses will not be re-fetched until at least this much time has elapsed.'),
    '#options' => $period,
    '#default_value' => $settings['api']['cache_lifetime'],
    '#states' => array(
      'visible' => array(
        ':input[name="parameter[payment_method][settings][payment_method][settings][api][cache]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['api']['user_account'] = array(
    '#type' => 'fieldset',
    '#title' => t('User account API settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['api']['user_account']['callback_url'] = array(
    '#type' => 'radios',
    '#title' => t('Callback URL'),
    '#description' => t('Main Hipay Wallet account callback URL is defined by providing it directly to Hipay Wallet support. However, each created Hipay user account could have its own callback URL value.')
      . '<br />' . t('- <em>Do not send at all</em> - will not send the <em>callbackUrl</em> element in the <em>createFullUseraccount</em> API request, and the main Hipay Wallet account callback URL will be used. This is the default value.')
      . '<br />' . t('- <em>Use the base URL of the site</em> - will send the <em>callbackUrl</em> value build from the current <em>base_url</em> value of the site.')
      . '<br />' . t('- <em>Use a custom URL</em> will allow you to provide a custom value for the <em>callbackURL</em> element of the <em>createFullUseraccount</em> API request.'),
    '#options' => array(
      COMMERCE_HIPAY_WS_API_CALLBACK_URL_DONT_SEND => t('Do not send at all'),
      COMMERCE_HIPAY_WS_API_CALLBACK_URL_SITE_BASE_URL => t('Use the base URL of the site'),
      COMMERCE_HIPAY_WS_API_CALLBACK_URL_CUSTOM_URL => t('Use a custom URL'),
    ),
    '#default_value' => $settings['api']['user_account']['callback_url'],
  );

  $form['api']['user_account']['callback_url_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom callback URL'),
    '#default_value' => $settings['api']['user_account']['callback_url_custom'],
    '#states' => array(
      'visible' => array(
        ':input[name="parameter[payment_method][settings][payment_method][settings][api][user_account][callback_url]"]' => array('value' => 'custom_url'),
      ),
    ),
  );

  if (!empty($settings['api_password'])) {

    // Website configuration.
    $form['website'] = array(
      '#type' => 'fieldset',
      '#title' => t('Website settings'),
      '#description' => t('The settings below will be used for the <code>websites</code> item of each <code>createFullUseraccount</code> API call.'),
      '#collapsible' => TRUE,
      '#prefix' => '<div id="website_settings">',
      '#suffix' => '</div>',
    );

    $form['website']['business_line_id'] = array(
      '#type' => 'select',
      '#title' => t('Business line ID'),
      '#description' => t('Main website theme.'),
      '#options' => array('' => t('- select -')) + commerce_hipay_ws_get_website_business_lines_options(),
      '#default_value' => $settings['website']['business_line_id'],
      // Ajax callback to populate the 'Topic ID' select options.
      '#ajax' => array(
        'callback' => 'commerce_hipay_ws_settings_form_ajax_callback',
        'wrapper' => 'website_settings',
      ),
      '#required' => TRUE,
    );

    $topic_options = array('' => t('- select -'));
    if (!empty($settings['website']['business_line_id'])) {
      $topic_options += commerce_hipay_ws_get_website_topics_options($settings['website']['business_line_id']);
    }

    $form['website']['topic_id'] = array(
      '#type' => 'select',
      '#title' => t('Topic ID'),
      '#description' => t('Secondary website theme.'),
      // When the Business Line ID is selected, real options will be added
      // in commerce_hipay_ws_form_rules_ui_edit_element_alter()
      // (as here we don't have access to $form_state values).
      '#options' => $topic_options,
      '#default_value' => $settings['website']['topic_id'],
      '#required' => TRUE,
    );

    $form['website']['contact_email'] = array(
      '#type' => 'textfield',
      '#title' => t('Contact email'),
      '#description' => t('Contact email displayed on payment page.'),
      '#default_value' => $settings['website']['contact_email'],
      '#required' => TRUE,
    );

    $form['website']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Website name'),
      '#default_value' => $settings['website']['name'],
      '#required' => TRUE,
    );

    $form['website']['url'] = array(
      '#type' => 'textfield',
      '#title' => t('Website URL'),
      '#description' => t('Full HTTP URL of website (http://www.merchant.com).'),
      '#default_value' => $settings['website']['url'],
      '#required' => TRUE,
    );

    $form['website']['merchant_password'] = array(
      '#type' => 'textfield',
      '#title' => t('Merchant password'),
      '#description' => t('Merchant password to use with MAPI KIT on payment buttons only.'),
      '#default_value' => $settings['website']['merchant_password'],
      '#required' => TRUE,
    );

    // Vendor account details.
    $form['vendor_account'] = array(
      '#type' => 'fieldset',
      '#title' => t('Vendor account details'),
      '#collapsible' => TRUE,
    );

    $rows = array();

    // Balance.
    $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_BALANCE);
    if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
      $rows[] = array(
        t('Account ID'),
        $response['balances']['item']['userAccountId'],
      );
      $rows[] = array(
        t('Balance'),
        commerce_currency_format(commerce_currency_decimal_to_amount($response['balances']['item']['balance'], $response['balances']['item']['currency']), $response['balances']['item']['currency']),
      );
      $rows[] = array(
        t('Money available for withdrawal'),
        commerce_currency_format(commerce_currency_decimal_to_amount($response['balances']['item']['moneyAvailableForWithdrawal'], $response['balances']['item']['currency']), $response['balances']['item']['currency']),
      );
    }

    $form['vendor_account']['info'] = array(
      '#type' => 'markup',
      '#markup' => theme('table', array('rows' => $rows)),
    );

    // Bank account details.
    $bank_account_description = '<p>' . t('Note that once the bank details are saved, and the bank account gets created in your Hipay Wallet account, they cannot be changed anymore. The only way to change or delete them is to contact Hipay support.') . '</p>'
    . '<p>' . t('To avoid creating the bank account in your Hipay Wallet account after submitting this form you can leave the <em>Bank country</em> field empty - then the Hipay Wallet API will not be called when you submit the form, and the bank account will not be created (even if all other bank-related fields are filled).') . '</p>'
    . '<p>' . t('Note that the required fields are different for different countries. To see which fields are required for your country, please select relevant <em>Bank country</em> value (leaving all other bank fields empty) and submit the form.') . '</p>';

    $form['bank_account'] = array(
      '#type' => 'fieldset',
      '#title' => t('Bank account details'),
      '#description' => $bank_account_description,
      '#collapsible' => TRUE,
    );

    $form['bank_account'] += commerce_hipay_ws_settings_form_bank($settings['account_id'], $settings['bank_account']);

    // Fees accounts.
    $form['fees'] = array(
      '#type' => 'fieldset',
      '#title' => t('Fee accounts'),
      '#collapsible' => TRUE,
    );
    foreach (commerce_currencies(TRUE) as $currency_code => $currency) {
      $form['fees'][$currency_code]['account_id'] = array(
        '#type' => 'textfield',
        '#title' => t('@currency_code fee account ID', array('@currency_code' => $currency_code)),
        '#default_value' => $settings['fees'][$currency_code]['account_id'],
        '#currency_code' => $currency_code,
      );

      // For each fees account add a bank account details form.
      $form['fees'][$currency_code]['bank_account'] = array(
        '#type' => 'fieldset',
        '#title' => t('@currency_code fee bank account', array('@currency_code' => $currency_code)),
        '#description' => $bank_account_description,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['fees'][$currency_code]['bank_account'] += commerce_hipay_ws_settings_form_bank($settings['fees'][$currency_code]['account_id'], $settings['fees'][$currency_code]['bank_account']);
    }

  }

  return $form;
}

/**
 * Returns bank account settings subform for the payment method settings form.
 *
 * This subform might be used multiple times in the main form: for the main
 * technical user account, as well as for all fees accounts.
 *
 * @see commerce_hipay_ws_settings_form()
 */
function commerce_hipay_ws_settings_form_bank($account_id, $settings = array()) {
  $form = array();

  // Check current bank information status only if API username and password
  // have already been provided - otherwise we won't be able to make API call.
  if (!empty($account_id)) {
    $parameters = array('wsSubAccountId' => $account_id);
    $response = commerce_hipay_ws_api_bank_account_get_info_status($parameters);
  }

  if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    // Display current status of bank account information.
    $form['current_status'] = array(
      '#markup' => t('Current Hipay Wallet bank information status: <strong>@status</strong>', array(
        '@status' => $response['status'],
      )),
    );

    // Add it to the form as well so we have access to it in validate
    // and submit callbacks.
    $form['status'] = array(
      '#type' => 'value',
      '#value' => $response['status'],
    );

    // If bank account has been already created in Hipay Wallet, we won't be
    // able to change it anymore through API, so just disable all bank-related
    // form fields, and collapse whole 'Bank account details' fieldset.
    if ($response['status'] != COMMERCE_HIPAY_WS_ACCOUNT_NO_BANK_INFO) {
      $disable_bank_form_elements = TRUE;

      // If bank account details has been validated in Hipay Wallet, let's
      // fetch all the information and display them in the form, overriding
      // whatever settings has been saved locally before.
      if ($response['status'] != COMMERCE_HIPAY_WS_ACCOUNT_BANK_INFO_VALIDATED) {
        $parameters = array('wsSubAccountId' => $account_id);
        $response = commerce_hipay_ws_api_bank_account_infos_check($parameters);

        if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
          // Alter existing payment method instance settings.
          $settings = $response;
          // 'bankInfosCheck' API resource return 'bankCountry' as a label
          // instead of country code (for example, it returns 'FRANCE' instead
          // of 'FR'), but we need country code to set the dropdown default
          // value, so we need another API call to get full country list,
          // from which we can map country label to country code.
          if (!empty($response['bankCountry'])) {
            $settings['bankCountry'] = commerce_hipay_ws_get_code_from_country($response['bankCountry']);
          }
        }
      }
    }
  }

  $form['bankName'] = array(
    '#type' => 'textfield',
    '#title' => t('Bank name'),
    '#default_value' => $settings['bankName'],
    '#disabled' => !empty($disable_bank_form_elements),
  );

  $form['bankAddress'] = array(
    '#type' => 'textfield',
    '#title' => t('Bank address'),
    '#default_value' => $settings['bankAddress'],
    '#disabled' => !empty($disable_bank_form_elements),
  );

  $form['bankZipCode'] = array(
    '#type' => 'textfield',
    '#title' => t('Bank zip code'),
    '#default_value' => $settings['bankZipCode'],
    '#disabled' => !empty($disable_bank_form_elements),
  );

  $form['bankCity'] = array(
    '#type' => 'textfield',
    '#title' => t('Bank city'),
    '#default_value' => $settings['bankCity'],
    '#disabled' => !empty($disable_bank_form_elements),
  );

  require_once DRUPAL_ROOT . '/includes/locale.inc';
  $options = array('' => '- select -') + country_get_list();
  $form['bankCountry'] = array(
    '#type' => 'select',
    '#title' => t('Bank country'),
    '#options' => $options,
    '#default_value' => $settings['bankCountry'],
    '#disabled' => !empty($disable_bank_form_elements),
  );

  $form['iban'] = array(
    '#type' => 'textfield',
    '#title' => t('IBAN'),
    '#default_value' => $settings['iban'],
    '#disabled' => !empty($disable_bank_form_elements),
  );

  $form['swift'] = array(
    '#type' => 'textfield',
    '#title' => t('SWIFT / BIC'),
    '#default_value' => $settings['swift'],
    '#disabled' => !empty($disable_bank_form_elements),
  );

  $form['acct_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Account number'),
    '#default_value' => $settings['acct_num'],
    '#disabled' => !empty($disable_bank_form_elements),
  );

  $form['aba_num'] = array(
    '#type' => 'textfield',
    '#title' => t('ABA number'),
    '#default_value' => $settings['aba_num'],
    '#disabled' => !empty($disable_bank_form_elements),
  );

  $form['transit_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Transit number'),
    '#default_value' => $settings['transit_num'],
    '#disabled' => !empty($disable_bank_form_elements),
  );

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter() for rules_ui_edit_element().
 *
 * Adds custom validation for Hipay Wallet payment method settings form.
 */
function commerce_hipay_ws_form_rules_ui_edit_element_alter(&$form, &$form_state) {
  // Alter the form only for the Hipay Wallet payment method.
  if (
    !empty($form_state['element_settings']['payment_method']['method_id'])
    && $form_state['element_settings']['payment_method']['method_id'] == 'commerce_hipay_ws'
    || !empty($form_state['element_settings']['payment_method'])
    && $form_state['element_settings']['payment_method'] == 'commerce_hipay_ws'
  ) {

    if (!empty($form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings']['website']['business_line_id'])) {
      $form['parameter']['payment_method']['settings']['payment_method']['settings']['website']['topic_id']['#options'] += commerce_hipay_ws_get_website_topics_options($form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings']['website']['business_line_id']);
    }

    // This is just to display a message on the very first submit of the form
    // that there are more settings that need to be provided.
    $form['#submit'][] = 'commerce_hipay_ws_settings_form_submit';

    // Add submit callback to create new Hipay user account entities
    // for provided vendor details, and for the fees accounts.
    $form['#validate'][] = 'commerce_hipay_ws_settings_form_validate_vendor_user_account';
    $form['#validate'][] = 'commerce_hipay_ws_settings_form_validate_fees_user_accounts';
    $form['#submit'][] = 'commerce_hipay_ws_settings_form_submit_create_vendor_user_account';
    $form['#submit'][] = 'commerce_hipay_ws_settings_form_submit_create_fees_user_accounts';

    // Add validation and submit callbacks to create new Hipay bank entities
    // for provided vendor bank details, and for the fees accounts.
    if (!empty($form['parameter']['payment_method']['settings']['payment_method']['settings']['bank_account'])) {
      $form['#validate'][] = 'commerce_hipay_ws_settings_form_validate_vendor_bank_account';
      $form['#validate'][] = 'commerce_hipay_ws_settings_form_validate_fees_bank_accounts';
      $form['#submit'][] = 'commerce_hipay_ws_settings_form_submit_create_vendor_bank_account';
      $form['#submit'][] = 'commerce_hipay_ws_settings_form_submit_create_fees_bank_accounts';
    }
  }
}

/**
 * Form ajax callback for Website section of commerce_hipay_ws_settings_form().
 *
 * @see commerce_hipay_ws_settings_form()
 */
function commerce_hipay_ws_settings_form_ajax_callback($form, $form_state) {
  return $form['parameter']['payment_method']['settings']['payment_method']['settings']['website'];
}

/**
 * Form validation callback for the main technical user account details.
 *
 * @see commerce_hipay_ws_form_rules_ui_edit_element_alter()
 * @see commerce_hipay_ws_settings_form()
 */
function commerce_hipay_ws_settings_form_validate_vendor_user_account(&$form, &$form_state) {
  $submitted_settings = $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings'];
  // This will be used in case we need to set the form error on an element.
  $form_element_parents = 'parameter][payment_method][settings][payment_method][settings][';
  // Validate the user account details.
  commerce_hipay_ws_settings_form_validate_user_account($submitted_settings['account_id'], $form_element_parents);
}

/**
 * Form validation callback for the fees accounts details.
 *
 * @see commerce_hipay_ws_form_rules_ui_edit_element_alter()
 * @see commerce_hipay_ws_settings_form()
 */
function commerce_hipay_ws_settings_form_validate_fees_user_accounts(&$form, &$form_state) {
  $submitted_settings = $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings'];
  if (!empty($submitted_settings['fees'])) {
    foreach ($submitted_settings['fees'] as $currency_code => $fees_account) {
      if (!empty($fees_account['account_id'])) {
        // This will be used in case we need to set the form error on an element.
        $form_element_parents = 'parameter][payment_method][settings][payment_method][settings][fees][' . $currency_code . '][';
        // Validate the user account details.
        commerce_hipay_ws_settings_form_validate_user_account($fees_account['account_id'], $form_element_parents);
      }
    }
  }
}

/**
 * Validates submitted user account details.
 *
 * This is called for both main technical account user details, as well as for
 * fees user accounts details. In both cases the accounts should already exist
 * (as they are created by Hipay team when setting everything up) and be already
 * validated (indentified).
 *
 * @param int $hipay_account_id
 * @param string $form_element_parents
 *   Parents of the user account details form fields.
 */
function commerce_hipay_ws_settings_form_validate_user_account($hipay_account_id, $form_element_parents = '') {
  // Try to fetch the user account information from Hipay Wallet API.
  if (!($response = commerce_hipay_ws_api_user_account_get_infos($hipay_account_id)) || !isset($response['code']) || !isset($response['identified'])) {
    form_set_error($form_element_parents . 'account_id', t('Error calling Hipay Wallet API to fetch account information.'));
  }

  // Make sure we received the user account details from Hipay Wallet.
  if ($response['code'] != COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    form_set_error($form_element_parents . 'account_id', t('Error fetching user account details from Hipay Wallet API: @description.', array('@description' => $response['description'])));
  }

  // Make sure the user account is already validated (identified).
  if ($response['identified'] != COMMERCE_HIPAY_WS_ACCOUNT_IDENTIFIED) {
    form_set_error($form_element_parents . 'account_id', t('The provided user account is not identified in Hipay Wallet.'));
  }
}

/**
 * Form validation callback for the main technical account bank details.
 *
 * @see commerce_hipay_ws_form_rules_ui_edit_element_alter()
 * @see commerce_hipay_ws_settings_form()
 */
function commerce_hipay_ws_settings_form_validate_vendor_bank_account(&$form, &$form_state) {
  $submitted_settings = $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings'];
  // This will be used in case we need to set the form error on an element.
  $form_element_parents = 'parameter][payment_method][settings][payment_method][settings][bank_account][';
  // Validate the bank account details.
  commerce_hipay_ws_settings_form_validate_bank_account($submitted_settings['bank_account'], $form_element_parents);
}

/**
 * Form validation callback for the fees accounts bank details.
 *
 * @see commerce_hipay_ws_form_rules_ui_edit_element_alter()
 * @see commerce_hipay_ws_settings_form()
 */
function commerce_hipay_ws_settings_form_validate_fees_bank_accounts(&$form, &$form_state) {
  $submitted_settings = $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings'];
  if (!empty($submitted_settings['fees'])) {
    foreach ($submitted_settings['fees'] as $currency_code => $fees_account) {
      if (!empty($fees_account['account_id'])) {
        // This will be used in case we need to set the form error on an element.
        $form_element_parents = 'parameter][payment_method][settings][payment_method][settings][fees][' . $currency_code . '][bank_account][';
        // Validate the bank account details.
        commerce_hipay_ws_settings_form_validate_bank_account($fees_account['bank_account'], $form_element_parents);
      }
    }
  }
}

/**
 * Validates submitted bank account details.
 *
 * This is called for both main technical account bank details, as well as for
 * fees accounts bank details.
 *
 * @param array $bank_account
 *   Submitted bank account details.
 * @param string $form_element_parents
 *   Parents of the bank account details form fields.
 *
 * @see commerce_hipay_ws_settings_form_validate_vendor_bank_account()
 * @see commerce_hipay_ws_settings_form_validate_fees_bank_accounts()
 */
function commerce_hipay_ws_settings_form_validate_bank_account($bank_account, $form_element_parents = '') {
  // Run the validation only if bank country was selected.
  if (!empty($bank_account['bankCountry'])) {
    // Get all required fields for selected country from Hipay API.
    $response = commerce_hipay_ws_api_bank_account_get_info_fields($bank_account['bankCountry']);

    // If any of the required fields is empty, set form error.
    if (!empty($response['fields']['item'])) {
      foreach ($response['fields']['item'] as $field) {
        if ($field['code'] != 'bankCountry' && empty($bank_account[$field['code']])) {
          form_set_error($form_element_parents . $field['code'], t('Bank @field_label is required.', array('@field_label' => $field['label'])));
        }
      }
    }
  }
}

/**
 * Custom payment method settings form submit callback.
 *
 * @see commerce_hipay_ws_settings_form()
 * @see  commerce_hipay_ws_form_rules_ui_edit_element_alter().
 */
function commerce_hipay_ws_settings_form_submit(&$form, &$form_state) {
  // If this is the first time that the payment method settings form was
  // submitted, display the message telling the user that there are more
  // settings to be provided.
  $original_settings = $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings']['settings'];
  if (empty($original_settings['api_password'])) {
    drupal_set_message(t("But wait! There's more!"), 'warning');
    $form_state['redirect'] = current_path();
  }
}

/**
 * Creates a new Hipay user account entity for the provided vendor details.
 *
 * @see commerce_hipay_ws_form_rules_ui_edit_element_alter()
 * @see commerce_hipay_ws_settings_form()
 */
function commerce_hipay_ws_settings_form_submit_create_vendor_user_account(&$form, &$form_state) {
  $submitted_settings = $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings'];
  commerce_hipay_ws_settings_form_submit_create_user_account($submitted_settings['account_id'], commerce_default_currency(), $submitted_settings);
}

/**
 * Creates a new Hipay user account entity for each provided fees account.
 *
 * @see commerce_hipay_ws_form_rules_ui_edit_element_alter()
 * @see commerce_hipay_ws_settings_form()
 */
function commerce_hipay_ws_settings_form_submit_create_fees_user_accounts(&$form, &$form_state) {
  $submitted_settings = $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings'];
  if (!empty($submitted_settings['fees'])) {
    foreach ($submitted_settings['fees'] as $currency_code => $fees_account) {
      if (!empty($fees_account['account_id'])) {
        commerce_hipay_ws_settings_form_submit_create_user_account($fees_account['account_id'], $currency_code, $submitted_settings);
      }
    }
  }
}

/**
 * Creates a new Hipay user account entity or updates the existing one.
 *
 * @param int $account_id
 *   Hipay user account ID to create or update.
 * @param string $currency_code
 *   A currency code of the Hipay user account to create or update.
 * @param array $settings
 *   An array of submitted payment method instance settings.
 *
 * @see commerce_hipay_ws_settings_form_submit_create_vendor_account()
 * @see commerce_hipay_ws_settings_form_submit_create_fees_accounts()
 */
function commerce_hipay_ws_settings_form_submit_create_user_account($account_id, $currency_code, $settings) {
  global $user;

  // Try to load the existing Hipay user account entity from provided account_id
  // value, and if it doesn't exist yet, create a new one.
  if (!$user_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($account_id)) {
    $user_account = commerce_hipay_ws_user_account_new(array('type' => 'hipay_system_account'));
    $user_account->uid = $user->uid;
  }

  // Replace existing values with form submitted values.
  // Replace property values.
  $user_account->hipay_account_id = $account_id;
  $user_account->hipay_space_id = '';
  $user_account->hipay_identified = COMMERCE_HIPAY_WS_ACCOUNT_IDENTIFIED;
  $user_account->hipay_ws_login = $settings['api_username'];
  $user_account->hipay_ws_password = $settings['api_password'];

  // Replace field values.
  $user_account_wrapper = entity_metadata_wrapper('commerce_hipay_ws_user_account', $user_account);
  $user_account_wrapper->hipay_ws_user_currency = $currency_code;

  // Create new revision.
  $user_account->revision = TRUE;
  $user_account->log = t('Submitted Hipay Wallet configuration form.');

  // Save updated Hipay user account entity.
  commerce_hipay_ws_user_account_save($user_account);
}

/**
 * Creates a bank account in Hipay Wallet.
 *
 * @param array $bank_account
 *   An array of bank account details.
 *
 * @return bool|void
 */
function commerce_hipay_ws_settings_form_submit_create_vendor_bank_account(&$form, &$form_state) {
  $submitted_settings = $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings'];
  $bank_account = $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings']['bank_account'];
  commerce_hipay_ws_settings_form_submit_create_bank_account($submitted_settings['account_id'], $bank_account, $submitted_settings);
}

function commerce_hipay_ws_settings_form_submit_create_fees_bank_accounts(&$form, &$form_state) {
  $submitted_settings = $form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings'];
  if (!empty($submitted_settings['fees'])) {
    foreach ($submitted_settings['fees'] as $currency_code => $fees_account) {
      if (!empty($fees_account['account_id'])) {
        commerce_hipay_ws_settings_form_submit_create_bank_account($fees_account['account_id'], $fees_account['bank_account'], $submitted_settings);
      }
    }
  }
}

function commerce_hipay_ws_settings_form_submit_create_bank_account($account_id, $bank_account, $settings) {
  global $user;

  // If bank country was not selected, do not try to create the bank account.
  if (empty($bank_account['bankCountry'])) {
    return;
  }

  // Try to load the existing Hipay bank account entity from provided account_id
  // value, and if it doesn't exist yet, create a new one.
  if (!$hipay_bank_account = commerce_hipay_ws_bank_account_load_by_hipay_account_id($account_id)) {
    $hipay_bank_account = commerce_hipay_ws_bank_account_new();
    $hipay_bank_account->uid = $user->uid;
    $hipay_bank_account->hipay_account_id = $settings['account_id'];
    $hipay_bank_account->hipay_status = COMMERCE_HIPAY_WS_ACCOUNT_NO_BANK_INFO;
  }

  // Update Hipay bank account values.
  $hipay_bank_account_wrapper = entity_metadata_wrapper('commerce_hipay_ws_bank_account', $hipay_bank_account);
  $hipay_bank_account_wrapper->hipay_ws_bank_address = array(
    'thoroughfare' => $bank_account['bankAddress'],
    'postal_code' => $bank_account['bankZipCode'],
    'locality' => $bank_account['bankCity'],
    'country' => $bank_account['bankCountry'],
  );
  $hipay_bank_account_wrapper->hipay_ws_bank_name = $bank_account['bankName'];
  $hipay_bank_account_wrapper->hipay_ws_bank_iban = $bank_account['iban'];
  $hipay_bank_account_wrapper->hipay_ws_bank_swift = $bank_account['swift'];
  $hipay_bank_account_wrapper->hipay_ws_bank_account_number = $bank_account['acct_num'];
  $hipay_bank_account_wrapper->hipay_ws_bank_aba_number = $bank_account['aba_num'];
  $hipay_bank_account_wrapper->hipay_ws_bank_transit_number = $bank_account['transit_num'];

  // Make sure the bank account is assigned to the Hipay user account.
  $hipay_user_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($account_id);
  $hipay_bank_account_wrapper->hipay_ws_bank_user_account = $hipay_user_account->user_account_id;

  // Save updated Hipay bank account.
  commerce_hipay_ws_bank_account_save($hipay_bank_account);

  // Re-fetch the bank account infos to update the local Hipay bank account
  // entity status value.
  $parameters = array('wsSubAccountId' => $account_id);
  $context = array('commerce_hipay_ws_bank_account' => $hipay_bank_account);
  $bank_account_info = commerce_hipay_ws_api_bank_account_get_info_status($parameters, $context);

  // If bank account has been already created in Hipay Wallet,
  // we do not want to create it again.
  if (isset($bank_account_info['status']) && $bank_account_info['status'] != COMMERCE_HIPAY_WS_ACCOUNT_NO_BANK_INFO) {
    return;
  }

  // Do the API call.
  if (!$response = commerce_hipay_ws_api_bank_account_create($hipay_bank_account)) {
    drupal_set_message(t('There was an error calling Hipay Wallet API. See log for more details.'), 'error');
    return FALSE;
  }

  // If an error code was returned in the response.
  if (!isset($response['code']) || $response['code'] !== COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    drupal_set_message(t('There was an error creating bank account at Hipay Wallet (@error_code): @error_description', array(
      '@error_code' => $response['code'],
      '@error_description' => $response['description'],
    )), 'error');
    return FALSE;
  }

  drupal_set_message(t('Bank details have been successfully registered in your Hipay Wallet account.'));

  return $response;
}

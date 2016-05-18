<?php

/**
 * Menu callback: admin settings form.
 *
 * @see commerce_hipay_ws_menu()
 */
function commerce_hipay_ws_admin_settings_form($form, $form_state) {
  $form = array();

  $form['commerce_hipay_ws_create_immediately'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create Hipay accounts immediately'),
    '#description' => t('When enabled, the relevant Hipay Wallet API to create a user or bank account will be called immediately after the relevant entity is created in Drupal.'),
    '#default_value' => variable_get('commerce_hipay_ws_create_immediately', COMMERCE_HIPAY_WS_CREATE_ACCOUNTS_IMMEDIATELY),
  );

  return system_settings_form($form);
}

/**
 * Menu callback: admin website settings form.
 *
 * @see commerce_hipay_ws_menu()
 */
function commerce_hipay_ws_website_settings_form($form, $form_state) {
  $form = array(
    '#prefix' => '<div id="hipay_ws_website_settings">',
    '#suffix' => '</div>',
  );

  // If the Hipay Wallet payment method instance has not yet been enabled
  // or configured, display the warning message.
  if ($instances = commerce_hipay_ws_payment_method_instances('commerce_hipay_ws', TRUE)) {
    $payment_method_instance = reset($instances);
  }
  if (empty($payment_method_instance['settings'])) {
    drupal_set_message(t('Please <a href="@url">enable and configure Hipay Wallet payment method</a> before providing the website configuration.', array(
      '@url' => url('admin/commerce/config/payment-methods'),
    )), 'error');
    return;
  }

  $form['commerce_hipay_ws_website_business_line_id'] = array(
    '#type' => 'select',
    '#title' => t('Business line ID'),
    '#description' => t('Main website theme.'),
    '#options' => commerce_hipay_ws_get_website_business_line_options(),
    '#default_value' => variable_get('commerce_hipay_ws_website_business_line_id'),
    '#ajax' => array(
      'callback' => 'commerce_hipay_ws_website_settings_form_ajax_callback',
      'wrapper' => 'hipay_ws_website_settings',
    ),
    '#required' => TRUE,
  );

  $selected_business_line_id = (!empty($form_state['values']['commerce_hipay_ws_website_business_line_id'])) ? $form_state['values']['commerce_hipay_ws_website_business_line_id'] : variable_get('commerce_hipay_ws_website_business_line_id');

  $form['commerce_hipay_ws_website_topic_id'] = array(
    '#type' => 'select',
    '#title' => t('Topic ID'),
    '#description' => t('Secondary website theme.'),
    '#options' => commerce_hipay_ws_get_website_topic_options($selected_business_line_id),
    '#default_value' => variable_get('commerce_hipay_ws_website_topic_id'),
    '#required' => TRUE,
  );

  $form['commerce_hipay_ws_website_contact_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Website contact email'),
    '#description' => t('Contact email displayed on payment page.'),
    '#default_value' => variable_get('commerce_hipay_ws_website_contact_email', variable_get('site_mail')),
    '#required' => TRUE,
  );

  $form['commerce_hipay_ws_website_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Website name'),
    '#default_value' => variable_get('commerce_hipay_ws_website_name', variable_get('site_name')),
    '#required' => TRUE,
  );

  $form['commerce_hipay_ws_website_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Website URL'),
    '#description' => t('Full HTTP URL of website (http://www.merchant.com).'),
    '#default_value' => variable_get('commerce_hipay_ws_website_url', $GLOBALS['base_url']),
    '#required' => TRUE,
  );

  $form['commerce_hipay_ws_website_merchant_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Website merchant password'),
    '#description' => t('Merchant password to use with MAPI KIT on payment buttons only.'),
    '#default_value' => variable_get('commerce_hipay_ws_website_merchant_password'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

function commerce_hipay_ws_website_settings_form_ajax_callback($form, $form_state) {
  return $form;
}

/**
 * Page callback: Displays list of payment methods available for a store.
 *
 * @param $store
 *   The store object for which available payment methods are displayed.
 *
 * @return array
 *   Content for a page listing payment methods available for a store.
 *
 * @see commerce_store_ui_menu()
 */
function commerce_hipay_ws_store_overview($store) {
  $rows = array();

  $account = $store->data['commerce_hipay_ws_account'];

  if (!empty($account['hipay_vendor_account'])) {
    $rows[] = array(
      t('Account ID'),
      $account['hipay_vendor_account']['userAccountId'],
    );
    $rows[] = array(
      t('Account email'),
      $account['email'],
    );
    $rows[] = array(
      t('API login'),
      $account['hipay_vendor_account']['wsLogin'],
    );
    $rows[] = array(
      t('API password'),
      $account['hipay_vendor_account']['wsPassword'],
    );
    $rows[] = array(
      t('Account Identified'),
      commerce_hipay_ws_check_account_identified($account['hipay_vendor_account']['userAccountId']) ? 'Yes' : 'No',
    );

    // Bank account status.
    $parameters = array(
      'wsLogin' => $account['hipay_vendor_account']['wsLogin'],
      'wsPassword' => $account['hipay_vendor_account']['wsPassword'],
      'locale' => 'en_GB',
    );
    $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_BANK_INFOS_STATUS, $parameters);
    if (isset($response['code']) && $response['code'] == COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
      $rows[] = array(
        t('Bank info status'),
        t($response['status']),
      );
    }

    // Bank account details.
    $parameters = array(
      'wsLogin' => $account['hipay_vendor_account']['wsLogin'],
      'wsPassword' => $account['hipay_vendor_account']['wsPassword'],
    );
    $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_BANK_INFOS_CHECK, $parameters);
    if (isset($response['code']) && $response['code'] == COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
      unset($response['code']);
      unset($response['description']);
      foreach ($response as $key => $value) {
        if (!empty($value)) {
          $rows[] = array(
            t($key),
            $value,
          );
        }
      }
    }

    // Balance.
    $parameters = array(
      'wsLogin' => $account['hipay_vendor_account']['wsLogin'],
      'wsPassword' => $account['hipay_vendor_account']['wsPassword'],
    );
    $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_GET_BALANCE, $parameters);
    if (isset($response['code']) && $response['code'] == COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
      $rows[] = array(
        t('Balance'),
        commerce_currency_format(commerce_currency_decimal_to_amount($response['balances']['item']['balance'], $response['balances']['item']['currency']), $response['balances']['item']['currency']),
      );
      $rows[] = array(
        t('Money available for withdrawal'),
        commerce_currency_format(commerce_currency_decimal_to_amount($response['balances']['item']['moneyAvailableForWithdrawal'], $response['balances']['item']['currency']), $response['balances']['item']['currency']),
      );
    }
  }
  else {
    $rows[] = array(t('Hipay Wallet vendor account has not been created yet.'));
  }

  return theme('table', array('rows' => $rows));
}

/**
 * Page callback: Displays list of received transfers for a store.
 *
 * @param $store
 *   The store object for which transfers are displayed.
 *
 * @return array
 *   Content for a page listing transfers for a store.
 *
 * @see commerce_store_ui_menu()
 */
function commerce_hipay_ws_store_transfers_received($store) {
  $rows = array();

  $account = commerce_hipay_ws_account_load_by_store_id($store->store_id);
  $transfers = commerce_hipay_ws_transfer_load_multiple(array(), array('recipient_account_id' => $account['account_id']));

  $header = array(
    t('Date'),
    t('Sender'),
    t('Amount'),
    t('Public label'),
    t('Private label'),
    t('Transaction ID'),
    t('Status'),
  );

  if (!empty($transfers)) {
    foreach ($transfers as $transfer) {
      $sender_account = commerce_hipay_ws_account_load_by_account_id($transfer['sender_account_id']);
      if ($sender_account['store_id'] == 0) {
        $sender = variable_get('site_name');
      }
      else {
        $sender_store = commerce_store_load($sender_account['store_id']);
        $sender = l($sender_store->title, 'store/' . $sender_store->store_id);
      }

      $rows[] = array(
        format_date($transfer['created']),
        $sender,
        commerce_currency_format($transfer['amount'], $transfer['currency_code']),
        $transfer['public_label'],
        $transfer['private_label'],
        $transfer['transaction_id'],
        $transfer['status_message'],
      );
    }
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Page callback: Displays list of sent transfers for a store.
 *
 * @param $store
 *   The store object for which transfers are displayed.
 *
 * @return array
 *   Content for a page listing transfers for a store.
 *
 * @see commerce_store_ui_menu()
 */
function commerce_hipay_ws_store_transfers_sent($store) {
  $rows = array();

  $account = commerce_hipay_ws_account_load_by_store_id($store->store_id);
  $transfers = commerce_hipay_ws_transfer_load_multiple(array(), array('sender_account_id' => $account['account_id']));

  $header = array(
    t('Date'),
    t('Recipient'),
    t('Amount'),
    t('Public label'),
    t('Private label'),
    t('Transaction ID'),
    t('Status'),
  );

  if (!empty($transfers)) {
    foreach ($transfers as $transfer) {
      $recipient_account = commerce_hipay_ws_account_load_by_account_id($transfer['recipient_account_id']);
      if ($recipient_account['store_id'] == 0) {
        $recipient = variable_get('site_name');
      }
      else {
        $recipient_store = commerce_store_load($recipient_account['store_id']);
        $recipient = l($recipient_store->title, 'store/' . $recipient_store->store_id);
      }

      $rows[] = array(
        format_date($transfer['created']),
        $recipient,
        commerce_currency_format($transfer['amount'], $transfer['currency_code']),
        $transfer['public_label'],
        $transfer['private_label'],
        $transfer['transaction_id'],
        $transfer['status_message'],
      );
    }
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Page callback: Displays list of withdrawals for a store.
 *
 * @param $store
 *   The store object for which withdrawals are displayed.
 *
 * @return array
 *   Content for a page listing withdrawals for a store.
 *
 * @see commerce_store_ui_menu()
 */
function commerce_hipay_ws_store_withdrawals($store) {
  $rows = array();

  $account = commerce_hipay_ws_account_load_by_store_id($store->store_id);
  $withdrawals = commerce_hipay_ws_withdrawal_load_multiple(array(), array('account_id' => $account['account_id']));

  $header = array(
    t('Date'),
    t('Amount'),
    t('Label'),
    t('Transaction ID'),
    t('Status'),
  );

  if (!empty($withdrawals)) {
    foreach ($withdrawals as $withdrawal) {
      $rows[] = array(
        format_date($withdrawal['created']),
        commerce_currency_format($withdrawal['amount'], $withdrawal['currency_code']),
        $withdrawal['label'],
        $withdrawal['transaction_id'],
        $withdrawal['status_message'],
      );
    }
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

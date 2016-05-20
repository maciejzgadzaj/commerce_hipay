<?php

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
    $context = array(
      'commerce_store' => $store,
    );
    $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_STATUS, $parameters, $context);
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
    $context = array(
      'commerce_store' => $store,
    );
    $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_CHECK, $parameters, $context);
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
    $context = array(
      'commerce_store' => $store,
    );
    $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_BALANCE, $parameters, $context);
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

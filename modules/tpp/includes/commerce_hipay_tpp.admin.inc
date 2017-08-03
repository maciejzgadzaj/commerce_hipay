<?php

/**
 * @file
 * Provide admin features such as manual refresh, capture, refund, cancel
 * and delete transactions.
 */

/**
 * Returns default settings for the Hipay TPP payment method.
 *
 * @see commerce_hipay_tpp_settings_form()
 */
function commerce_hipay_tpp_settings_default() {
  $default_settings = array(
    'endpoint' => COMMERCE_HIPAY_TPP_TEST,
    'operation' => COMMERCE_HIPAY_TPP_OPERATION_SALE,
    'supported_currencies' => drupal_map_assoc(array_keys(commerce_hipay_get_enabled_currencies())),
    'currency_code' => commerce_default_currency(),
    'template' => COMMERCE_HIPAY_TPP_TEMPLATE_BASIC,
    'language' => 'en_GB',
    '3ds' => 1,
    'cardonfile' => FALSE,
    'css' => '',
    'api_logging' => array(
      'request' => FALSE,
      'response' => FALSE,
    ),
  );
  foreach (array_keys(commerce_currencies(TRUE)) as $currency_code) {
    $default_settings['accounts'][$currency_code] = array(
      'api_username' => '',
      'api_password' => '',
      'secret_passphrase' => '',
    );
  }
  return $default_settings;
}

/**
 * Payment method callback: settings form.
 *
 * @see hook_commerce_payment_method_info()
 */
function commerce_hipay_tpp_settings_form($settings = array()) {
  // Merge default settings into the stored settings array.
  // Settings are stored in multi-level array, so we need recursive merge. And
  // actually replace instead of merge, to make sure the default settings are
  // correctly overriden by custom user settings (array_merge_recursive() would
  // just add new array elements instead of overriding existing ones).
  $settings = array_replace_recursive(commerce_hipay_tpp_settings_default(), $settings);

  $default_currency = commerce_default_currency();
  $enabled_currencies = commerce_currencies(TRUE);

  $form = array();

  $form['endpoint'] = array(
    '#type' => 'radios',
    '#title' => t('Environment'),
    '#options' => array(
      COMMERCE_HIPAY_TPP_TEST => t('Test - process test transactions to a test account'),
      COMMERCE_HIPAY_TPP_PRODUCTION => t('Production - process real transactions to a production account'),
    ),
    '#default_value' => $settings['endpoint'],
    '#required' => TRUE,
  );

  foreach ($enabled_currencies as $currency_code => $currency) {
    $form['accounts'][$currency_code] = array(
      '#type' => 'fieldset',
      '#title' => t('@currency_code account', array('@currency_code' => $currency_code)),
      '#collapsible' => TRUE,
      '#collapsed' => !($currency_code == commerce_default_currency()) && empty($settings['accounts'][$currency_code]['api_username']) && empty($settings['accounts'][$currency_code]['api_password']),
      '#group' => 'accounts',
    );

    $form['accounts'][$currency_code]['api_username'] = array(
      '#type' => 'textfield',
      '#title' => t('API username'),
      '#description' => t('The name of the user for accessing Hipay TPP webservice. This, as well as API password, can be found in your Hipay Fullservice Account under <em>Integration / Security Settings</em>.'),
      '#default_value' => $settings['accounts'][$currency_code]['api_username'],
      '#required' => $currency_code == commerce_default_currency(),
    );

    $form['accounts'][$currency_code]['api_password'] = array(
      '#type' => 'textfield',
      '#title' => t('API password'),
      '#description' => t('The password for the user specified in the above field.'),
      '#default_value' => $settings['accounts'][$currency_code]['api_password'],
      '#required' => $currency_code == commerce_default_currency(),
    );

    $form['accounts'][$currency_code]['secret_passphrase'] = array(
      '#type' => 'textfield',
      '#title' => t('Secret Passphrase'),
      '#description' => t('This secret passphrase is used to generate a unique character string (signature) hashed with SHA algorithm. It should be the same as the value provided in <em>Secret Passphrase</em> field in your Hipay Fullservice Account configuration in <em>Integration » Security Settings</em>.'),
      '#default_value' => $settings['accounts'][$currency_code]['secret_passphrase'],
      '#required' => $currency_code == commerce_default_currency(),
    );
  }

  $form['operation'] = array(
    '#type' => 'radios',
    '#title' => t('Default transaction type'),
    '#options' => array(
      COMMERCE_HIPAY_TPP_OPERATION_SALE => t('Sale - indicates transaction is sent for authorization, and if approved, is automatically submitted for capture'),
      COMMERCE_HIPAY_TPP_OPERATION_AUTHORIZATION => t('Authorization - indicates this transaction is sent for authorization only. The transaction will not be sent for settlement until the transaction is submitted for capture manually by the Merchant'),
    ),
    '#default_value' => $settings['operation'],
    '#required' => TRUE,
  );

  $form['supported_currencies'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Supported currencies'),
    '#description' => t('Transactions in these currencies will be sent as-is to the gateway, without any prior conversion. This setting should reflect your <em>Settlement currencies</em> configuration of your Hipay Fullservice account.'),
    '#options' => commerce_hipay_get_enabled_currencies(),
    '#multiple' => TRUE,
    '#default_value' => $settings['supported_currencies'],
    '#required' => TRUE,
  );

  $form['currency_code'] = array(
    '#type' => 'select',
    '#title' => t('Default currency'),
    '#description' => t('Transactions in other currencies will be converted to this currency, so multi-currency sites must be configured to use appropriate conversion rates.'),
    '#options' => commerce_hipay_get_enabled_currencies(),
    '#default_value' => $settings['currency_code'],
  );

  $form['template'] = array(
    '#type' => 'radios',
    '#title' => t('Checkout redirect mode'),
    '#options' => array(
      COMMERCE_HIPAY_TPP_TEMPLATE_BASIC => t('Redirect to the hosted checkout page through an automatically submitted form'),
      COMMERCE_HIPAY_TPP_TEMPLATE_IFRAME => t('Stay on this site using an iframe to embed the hosted checkout page'),
    ),
    '#default_value' => $settings['template'],
    '#required' => TRUE,
  );

  $form['language'] = array(
    '#type' => 'select',
    '#title' => t('Default language'),
    '#description' => t("Language to be used by the off-site payment page or iframe if user's language is not supported by the gateway."),
    '#options' => commerce_hipay_get_supported_languages(),
    '#default_value' => $settings['language'],
  );

  $form['3ds'] = array(
    '#type' => 'radios',
    '#title' => t('Should the 3-D Secure authentication be performed for payment transactions'),
    '#options' => array(
      0 => t('Bypass 3-D Secure authentication.'),
      1 => t('3-D Secure authentication if available.'),
      2 => t('3-D Secure authentication mandatory.'),
    ),
    '#default_value' => $settings['3ds'],
    '#required' => TRUE,
  );

  if (module_exists('commerce_cardonfile')) {
    $form['cardonfile'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Card on File functionality with this payment method.'),
      '#description' => t('Stores tokenized value for card data.'),
      '#default_value' => $settings['cardonfile'],
    );
  }
  else {
    $form['cardonfile'] = array(
      '#type' => 'markup',
      '#markup' => t('To enable Card on File functionality download and install the <a href="@cardonfile_url">Card on File</a> module.', array(
        '@cardonfile_url' => url('https://www.drupal.org/project/commerce_cardonfile'),
      )),
    );
  }

  $form['css'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment page style sheet'),
    '#description' => t('Path and filename of the custom style sheet for the hosted payment page, relative to Drupal webroot.')
      . '<br />' . t('Note that Hipay requires HTTPS protocol, and the URL generated will reflect this requirement - make sure that your server configuration supports it.'),
    '#default_value' => $settings['css'],
  );

  $form['api_logging'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Log the following messages for debugging'),
    '#description' => t('Basic transaction-related request and response data will be saved in transaction <em>payload</em> property regardless of this setting. Enabling this option will save additional information on each transaction request and response in the dblog, and save cURL verbose information to @curllog.', array('@curllog' => file_directory_temp() . '/curl.log')),
    '#options' => array(
      'request' => t('API request messages'),
      'response' => t('API response messages'),
    ),
    '#default_value' => $settings['api_logging'],
  );

  return $form;
}

/**
 * Returns default settings for the Hipay TPP Virtual IBAN payment method.
 *
 * @see commerce_hipay_tpp_virtual_iban_settings_form()
 */
function commerce_hipay_tpp_virtual_iban_settings_default() {
  $default_settings = array(
    'endpoint' => COMMERCE_HIPAY_TPP_TEST,
    'creation_mode' => 'order',
    'expiry_date' => '+1 year',
    'completion_message' => '<p>To complete your order, please transfer <b>[commerce-order:commerce-order-total:amount_decimal] [commerce-order:commerce-order-total:currency_code]</b> to the following bank account:</p>
<p>BIC: <b>[commerce-order:payment-method:virtual-iban:bic]</b>
<br />IBAN: <b>[commerce-order:payment-method:virtual-iban:iban]</b></p>
<p>using reference: <b>[commerce-order:payment-method:virtual-iban:external-reference]</b></p>',
    'api_logging' => array(
      'request' => FALSE,
      'response' => FALSE,
    ),
  );
  foreach (array_keys(commerce_currencies(TRUE)) as $currency_code) {
    $default_settings['accounts'][$currency_code] = array(
      'api_username' => '',
      'api_password' => '',
      'secret_passphrase' => '',
    );
  }
  return $default_settings;
}

/**
 * Payment method callback: Virtual IBAN settings form.
 *
 * @see hook_commerce_payment_method_info()
 */
function commerce_hipay_tpp_virtual_iban_settings_form($settings = array()) {
  // Merge default settings into the stored settings array.
  // Settings are stored in multi-level array, so we need recursive merge. And
  // actually replace instead of merge, to make sure the default settings are
  // correctly overriden by custom user settings (array_merge_recursive() would
  // just add new array elements instead of overriding existing ones).
  $settings = array_replace_recursive(commerce_hipay_tpp_virtual_iban_settings_default(), $settings);

  $default_currency = commerce_default_currency();
  $enabled_currencies = commerce_currencies(TRUE);

  $form = array();

  $form['endpoint'] = array(
    '#type' => 'radios',
    '#title' => t('Environment'),
    '#options' => array(
      COMMERCE_HIPAY_TPP_TEST => t('Test - process test transactions to a test account'),
      COMMERCE_HIPAY_TPP_PRODUCTION => t('Production - process real transactions to a production account'),
    ),
    '#default_value' => $settings['endpoint'],
    '#required' => TRUE,
  );

  foreach ($enabled_currencies as $currency_code => $currency) {
    $form['accounts'][$currency_code] = array(
      '#type' => 'fieldset',
      '#title' => t('@currency_code account', array('@currency_code' => $currency_code)),
      '#collapsible' => TRUE,
      '#collapsed' => !($currency_code == commerce_default_currency()) && empty($settings['accounts'][$currency_code]['api_username']) && empty($settings['accounts'][$currency_code]['api_password']),
      '#group' => 'accounts',
    );

    $form['accounts'][$currency_code]['api_username'] = array(
      '#type' => 'textfield',
      '#title' => t('API username'),
      '#description' => t('The name of the user for accessing Hipay TPP webservice. This, as well as API password, can be found in your Hipay Fullservice Account under <em>Integration / Security Settings</em>.'),
      '#default_value' => $settings['accounts'][$currency_code]['api_username'],
      '#required' => $currency_code == commerce_default_currency(),
    );

    $form['accounts'][$currency_code]['api_password'] = array(
      '#type' => 'textfield',
      '#title' => t('API password'),
      '#description' => t('The password for the user specified in the above field.'),
      '#default_value' => $settings['accounts'][$currency_code]['api_password'],
      '#required' => $currency_code == commerce_default_currency(),
    );

    $form['accounts'][$currency_code]['secret_passphrase'] = array(
      '#type' => 'textfield',
      '#title' => t('Secret Passphrase'),
      '#description' => t('This secret passphrase is used to generate a unique character string (signature) hashed with SHA algorithm. It should be the same as the value provided in <em>Secret Passphrase</em> field in your Hipay Fullservice Account configuration in <em>Integration » Security Settings</em>.'),
      '#default_value' => $settings['accounts'][$currency_code]['secret_passphrase'],
      '#required' => $currency_code == commerce_default_currency(),
    );
  }

  $form['creation_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Creation mode'),
    '#options' => array(
      'order' => t('new Virtual IBAN for each order'),
      'user' => t('one Virtual IBAN for each user'),
    ),
    '#default_value' => $settings['creation_mode'],
    '#required' => TRUE,
  );

  $form['expiry_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiration date'),
    '#description' => t('<a href="@relative_url">Relative format</a> parsable by <a href="@strtotime_url"><code>strtotime()</code></a>, for example: <em>+1 year</em>', array(
      '@relative_url' => url('http://php.net/manual/datetime.formats.relative.php'),
      '@strtotime_url' => url('http://php.net/manual/function.strtotime.php')
    )),
    '#default_value' => $settings['expiry_date'],
    '#required' => TRUE,
  );

  $form['completion_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Completion message'),
    '#description' => t('Message to be displayed on the completion step of the checkout process.'),
    '#default_value' => $settings['completion_message'],
  );

  $form['api_logging'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Log the following messages for debugging'),
    '#description' => t('Basic transaction-related request and response data will be saved in transaction <em>payload</em> property regardless of this setting. Enabling this option will save additional information on each transaction request and response in the dblog, and save cURL verbose information to @curllog.', array('@curllog' => file_directory_temp() . '/curl.log')),
    '#options' => array(
      'request' => t('API request messages'),
      'response' => t('API response messages'),
    ),
    '#default_value' => $settings['api_logging'],
  );

  return $form;
}

/**
 * Menu callback: refresh transaction details from Hipay.
 *
 * @see commerce_hipay_tpp_menu()
 * @see commerce_hipay_tpp_access_refresh()
 */
function commerce_hipay_tpp_refresh($order, $transaction) {
  $request_data = array(
    'orderid' => $order->order_id,
  );

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);

  // Allow other modules to alter the $request_data array.
  drupal_alter('commerce_hipay_tpp_api_refresh', $request_data, $order, $transaction, $payment_method);

  // Perform the call to Hipay TPP API.
  $url = commerce_hipay_tpp_get_server_url($payment_method) . COMMERCE_HIPAY_TPP_RESOURCE_TRANSACTION . $transaction->remote_id;
  $context = array(
    'call_type' => 'Refresh',
    'currency_code' => $transaction->currency_code,
  );
  $response = commerce_hipay_tpp_api_request($url, $request_data, $payment_method, $context);

  $callback_type = 'refresh';
  $feedback = $response['transaction'];
  $result = commerce_hipay_tpp_process_notification($feedback, $callback_type);

  // Allow other modules to process the response feedback.
  foreach (module_implements('commerce_hipay_tpp_process_notification') as $module) {
    $function = $module . '_commerce_hipay_tpp_process_notification';
    $result = $result && $function($feedback, $callback_type, $result);
  }

  if ($result) {
    drupal_set_message(t('Transaction details has been successfully refreshed.'));
  }

  drupal_goto('admin/commerce/orders/' . $order->order_id . '/payment');
}

/**
 * Menu callback: capture an authorization transaction.
 *
 * @see commerce_hipay_tpp_menu()
 * @see commerce_hipay_tpp_access_capture()
 * @see commerce_hipay_tpp_capture_form_validate()
 * @see commerce_hipay_tpp_capture_form_submit()
 */
function commerce_hipay_tpp_capture_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $totals = commerce_hipay_tpp_get_order_operation_totals($order);
  $total_captured = commerce_hipay_tpp_get_order_total_captured($totals);
  $balance = commerce_payment_order_balance($order);

  $default_amount = $totals[COMMERCE_HIPAY_TPP_OPERATION_AUTHORIZATION] - $total_captured;
  if ($balance['amount'] < $default_amount) {
    $default_amount = $balance['amount'];
  }

  $default_amount = number_format(commerce_currency_amount_to_decimal($default_amount, $transaction->currency_code), 2, '.', '');

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Capture amount'),
    '#description' => commerce_hipay_tpp_get_order_transaction_summary($order),
    '#default_value' => $default_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('What amount do you want to capture?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Capture'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}


/**
 * Capture form validation handler: ensure a valid amount is given.
 *
 * @see commerce_hipay_tpp_capture_form()
 * @see commerce_hipay_tpp_capture_form_submit()
 */
function commerce_hipay_tpp_capture_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to capture.'));
  }

  // Ensure the amount is less than or equal to the authorisation amount.
  $totals = commerce_hipay_tpp_get_order_operation_totals($form_state['order']);
  $total_captured = commerce_hipay_tpp_get_order_total_captured($totals);

  $max_capture_amount = $totals[COMMERCE_HIPAY_TPP_OPERATION_AUTHORIZATION] - $total_captured;
  if ($amount > commerce_currency_amount_to_decimal($max_capture_amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot capture more than you authorised through Hipay.'));
  }

  // Ensure the amount is less than or equal to current order balance.
  $balance = commerce_payment_order_balance($form_state['order']);
  if ($amount > commerce_currency_amount_to_decimal($balance['amount'], $balance['currency_code'])) {
    form_set_error('amount', t('You cannot capture more than current order balance amount.'));
  }
}

/**
 * Capture form submit handler: process capture transaction with Hipay.
 *
 * @see commerce_hipay_tpp_capture_form()
 * @see commerce_hipay_tpp_capture_form_validate()
 */
function commerce_hipay_tpp_capture_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $order = commerce_order_load($transaction->order_id);

  commerce_hipay_tpp_api_capture($transaction, $form_state['values']['amount']);

  $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
}

/**
 * Menu callback: refund an capture transaction.
 *
 * @see commerce_hipay_tpp_menu()
 * @see commerce_hipay_tpp_access_refund()
 * @see commerce_hipay_tpp_refund_form_validate()
 * @see commerce_hipay_tpp_refund_form_submit()
 */
function commerce_hipay_tpp_refund_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $totals = commerce_hipay_tpp_get_order_operation_totals($order);
  $total_captured = commerce_hipay_tpp_get_order_total_captured($totals);

  $default_amount = $total_captured;
  if (!empty($totals[COMMERCE_HIPAY_TPP_OPERATION_REFUND])) {
    $default_amount -= abs($totals[COMMERCE_HIPAY_TPP_OPERATION_REFUND]);
  }
  $default_amount = number_format(commerce_currency_amount_to_decimal($default_amount, $transaction->currency_code), 2, '.', '');

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Refund amount'),
    '#description' => commerce_hipay_tpp_get_order_transaction_summary($order),
    '#default_value' => $default_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('What amount do you want to refund?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Refund'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}


/**
 * Refund form validation handler: ensure a valid amount is given.
 *
 * @see commerce_hipay_tpp_refund_form()
 * @see commerce_hipay_tpp_refund_form_submit()
 */
function commerce_hipay_tpp_refund_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to refund.'));
  }

  // Ensure the amount is less than or equal to the authorisation amount.
  $totals = commerce_hipay_tpp_get_order_operation_totals($form_state['order']);
  $total_captured = commerce_hipay_tpp_get_order_total_captured($totals);
  $max_capture_amount = $total_captured;
  if (!empty($totals[COMMERCE_HIPAY_TPP_OPERATION_REFUND])) {
    $max_capture_amount -= abs($totals[COMMERCE_HIPAY_TPP_OPERATION_REFUND]);
  }

  if ($amount > commerce_currency_amount_to_decimal($max_capture_amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot refund more than you captured through Hipay.'));
  }
}

/**
 * Refund form submit handler: process refund transaction with Hipay.
 *
 * @see commerce_hipay_tpp_refund_form()
 * @see commerce_hipay_tpp_refund_form_validate()
 */
function commerce_hipay_tpp_refund_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $order = commerce_order_load($transaction->order_id);

  $form_state['refund_transaction'] = commerce_hipay_tpp_api_refund($transaction, $form_state['values']['amount']);

  $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
}

/**
 * Menu callback: cancel an authorized transaction.
 *
 * @see commerce_hipay_tpp_menu()
 * @see commerce_hipay_tpp_access_cancel()
 * @see commerce_hipay_tpp_cancel_form_submit()
 */
function commerce_hipay_tpp_cancel_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $form = confirm_form($form,
    t('Are you sure you want to cancel this transaction?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Cancel'),
    t('Back'),
    'confirm'
  );

  return $form;
}

/**
 * Cancel form submit handler: process cancel transaction with Hipay.
 *
 * @see commerce_hipay_tpp_cancel_form()
 * @see commerce_hipay_tpp_cancel_form_validate()
 */
function commerce_hipay_tpp_cancel_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $order = commerce_order_load($transaction->order_id);

  commerce_hipay_tpp_api_cancel($transaction);

  $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
}

/**
 * Returns a summary of existing order transactions.
 *
 * @param object $order
 *   An order to return the summary for.
 *
 * @return string
 *   A summary of existing order transactions
 */
function commerce_hipay_tpp_get_order_transaction_summary($order) {
  $balance = commerce_payment_order_balance($order);
  $return = array(t('Order balance: @balance', array(
    '@balance' => commerce_currency_format($balance['amount'], $balance['currency_code']),
  )));

  $totals = commerce_hipay_tpp_get_order_operation_totals($order);
  foreach ($totals as $label => $value) {
    $return[] = t('@label amount: @value', array(
      '@label' => ucfirst(str_replace('_', ' ', $label)),
      '@value' => commerce_currency_format(abs($value), $balance['currency_code']),
    ));
  }

  return implode('<br />', $return);
}

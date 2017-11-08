<?php

/**
 * @file
 * Provide admin features such as manual refresh, capture, refund, cancel
 * and delete transactions.
 */

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
    $result = $function($feedback, $callback_type, $result) && $result;
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

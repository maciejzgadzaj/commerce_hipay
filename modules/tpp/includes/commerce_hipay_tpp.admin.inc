<?php

/**
 * @file
 * Provide admin features such as manual capture, refund, cancel and delete
 * transactions.
 */

/**
 * Menu callback: refresh transaction details from Hipay.
 *
 * @see commerce_hipay_tpp_menu()
 */
function commerce_hipay_tpp_refresh($order, $transaction) {
  $request_data = array(
    'orderid' => $order->order_id,
  );

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);

  // Allow other modules to alter the $request_data array.
  drupal_alter('commerce_hipay_tpp_refresh', $request_data, $order, $transaction, $payment_method);

  // Perform the call to Hipay TPP API.
  $url = commerce_hipay_tpp_get_server_url($payment_method) . COMMERCE_HIPAY_TPP_RESOURCE_TRANSACTION . $transaction->remote_id;
  $response = commerce_hipay_tpp_api_request($url, $request_data, $payment_method, 'Refresh');

  if (commerce_hipay_tpp_process_feedback($response['transaction'], 'refresh')) {
    drupal_set_message(t('Transaction details has been successfully refreshed.'));
  }

  drupal_goto('admin/commerce/orders/' . $order->order_id . '/payment');
}

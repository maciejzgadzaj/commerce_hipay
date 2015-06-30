<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Allows other modules to alter hosted payment page initialization request
 * parameters before sending them to Hipay TPP API.
 *
 * @param array $request_data
 *   An array of parameters being sent to Hipay TPP API.
 * @param object $order
 *   An order object being paid for.
 * @param array $payment_method
 *   The payment method instance used for the payment transaction.
 */
function hook_commerce_hipay_tpp_initialize_alter(&$request_data, $order, $payment_method) {
  // No example.
}

/**
 * Allows other modules to alter capture request parameters before sending them
 * to Hipay TPP API.
 *
 * @param array $request_data
 *   An array of parameters being sent to Hipay TPP API.
 * @param object $order
 *   An order object being paid for.
 * @param array $payment_method
 *   The payment method instance used for the payment transaction.
 */
function hook_commerce_hipay_tpp_capture_alter(&$request_data, $order, $payment_method) {
  // No example.
}

/**
 * Allows other modules to alter transaction refresh request parameters
 * before sending them to Hipay TPP API.
 *
 * @param array $request_data
 *   An array of parameters being sent to Hipay TPP API.
 * @param object $order
 *   An order object being paid for.
 * @param object $transaction
 *   Payment transaction to refresh.
 */
function hook_commerce_hipay_tpp_refresh_alter(&$request_data, $order, $transaction, $payment_method) {
  // No example.
}

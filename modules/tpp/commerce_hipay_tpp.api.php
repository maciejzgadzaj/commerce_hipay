<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Allows other modules to alter Initialize a Hosted Payment Page API request
 * parameters before sending them to Hipay TPP API.
 *
 * @param array $request_data
 *   An array of parameters being sent to Hipay TPP API.
 * @param object $order
 *   An order object being paid for.
 * @param array $payment_method
 *   The payment method instance used for the payment transaction.
 *
 * @see http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-initializeHostedPaymentPage.html
 */
function hook_commerce_hipay_tpp_api_initialize_alter(&$request_data, $order, $payment_method) {
  // No example.
}

/**
 * Allows other modules to alter Request a New Order API request parameters
 * before sending them to Hipay TPP API.
 *
 * @param array $request_data
 *   An array of parameters being sent to Hipay TPP API.
 * @param object $order
 *   An order object being paid for.
 * @param array $payment_method
 *   The payment method instance used for the payment transaction.
 *
 * @see http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-requestNewOrder.html
 */
function hook_commerce_hipay_tpp_api_order_request_alter(&$request_data, $order, $payment_method) {
  // No example.
}

/**
 * Allow other modules to validate redirect feedback and change validation
 * result.
 *
 * @param array $feedback
 *   An associative array containing the Hipay API call feedback.
 * @param object $order
 *   An order object being paid for.
 * @param array $payment_method
 *   The payment method instance used for the payment transaction.
 *
 * @return bool
 *   A boolean indicating whether the processing was successful or not.
 *   Note that based on this value the order being processed will be moved
 *   to either next (if TRUE) or previous (if FALSE) checkout step.
 *   Also, if the validation callback returns FALSE, the redirect form
 *   submission callback will not be called at all.
 *
 * @see commerce_hipay_tpp_redirect_form_validate()
 * @see commerce_payment_redirect_pane_checkout_form()
 */
function hook_commerce_hipay_tpp_redirect_form_validate($feedback, $order, $payment_method) {
  // No example.
}

/**
 * Allow other modules to perform any additional submit functions if necessary.
 *
 * Note that this hook implementations will not be called if the validation
 * result of commerce_hipay_tpp_redirect_form_validate() was FALSE.
 *
 * @param array $feedback
 *   An associative array containing the Hipay API call feedback.
 * @param object $order
 *   An order object being paid for.
 * @param array $payment_method
 *   The payment method instance used for the payment transaction.
 *
 * @see commerce_hipay_tpp_redirect_form_submit()
 * @see commerce_payment_redirect_pane_checkout_form()
 */
function hook_commerce_hipay_tpp_redirect_form_submit($feedback, $order, $payment_method) {
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
 * @param array $payment_method
 *   The payment method instance used for the payment transaction.
 */
function hook_commerce_hipay_tpp_api_refresh_alter(&$request_data, $order, $transaction, $payment_method) {
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
function hook_commerce_hipay_tpp_api_capture_alter(&$request_data, $order, $payment_method) {
  // No example.
}

/**
 * Allows other modules to alter refund request parameters before sending them
 * to Hipay TPP API.
 *
 * @param array $request_data
 *   An array of parameters being sent to Hipay TPP API.
 * @param object $order
 *   An order object being paid for.
 * @param array $payment_method
 *   The payment method instance used for the payment transaction.
 */
function hook_commerce_hipay_tpp_api_refund_alter(&$request_data, $order, $payment_method) {
  // No example.
}

/**
 * Allows other modules to alter cancel request parameters before sending them
 * to Hipay TPP API.
 *
 * @param array $request_data
 *   An array of parameters being sent to Hipay TPP API.
 * @param object $order
 *   An order object being paid for.
 * @param array $payment_method
 *   The payment method instance used for the payment transaction.
 */
function hook_commerce_hipay_tpp_api_cancel_alter(&$request_data, $order, $payment_method) {
  // No example.
}

/**
 * Allows other modules to alter Virtual IBAN creation request parameters
 * before sending them to Hipay TPP API.
 *
 * @param array $request_data
 *   An array of parameters being sent to Hipay TPP API.
 * @param array $payment_method
 *   The payment method instance used for the Virtual IBAN creation.
 */
function hook_commerce_hipay_tpp_api_virtual_iban_create_alter(&$request_data, $payment_method) {
  // No example.
}

/**
 * Allows other modules to alter Virtual IBAN search request parameters
 * before sending them to Hipay TPP API.
 *
 * @param array $request_data
 *   An array of parameters being sent to Hipay TPP API.
 * @param array $payment_method
 *   The payment method instance used for the Virtual IBAN search.
 */
function hook_commerce_hipay_tpp_api_virtual_iban_search_alter($request_data, $payment_method) {
  // No example.
}

/**
 * Allows other modules to process Hipay server-to-server notification.
 *
 * @param array $feedback
 *   An associative array containing the Hipay server-to-server notification
 *   feedback, or 'refresh' API call response feedback.
 * @param string $callback_type
 *   A callback type which the feedback is being processed for.
 *   This could be either 'notification' (when processing asynchronous
 *   server-to-server notification) or 'refresh' (when processing the response
 *   of the 'refresh' API call).
 * @param bool $processing_result
 *   A boolean indicating whether the feedback was processed successfully by
 *   commerce_hipay_tpp_process_notification() and other earlier implementations
 *   of this hook.
 *
 * @return bool
 *   A boolean indicating whether the processing was successful or not.
 *
 * @see commerce_hipay_tpp_callback_notification()
 * @see commerce_hipay_tpp_refresh()
 */
function hook_commerce_hipay_tpp_process_notification($feedback, $callback_type, $processing_result) {
  // No example.
}

/**
 * Allows other modules to process Hipay SCP server-to-server notification.
 *
 * Hipay SCT notifications are notifications sent on receiving a Virtual IBAN
 * payment.
 *
 * @param array $feedback
 *   An associative array containing the Hipay SCP server-to-server notification
 *   feedback.
 * @param object $transaction
 *   A payment transaction entity related to the notification.
 * @param object|null $order
 *   An order entity related to the notification (if found).
 * @param bool $processing_result
 *   A boolean indicating whether the feedback was processed successfully by
 *   commerce_hipay_tpp_process_notification() and other earlier implementations
 *   of this hook.
 *
 * @return bool
 *   A boolean indicating whether the processing was successful or not.
 *
 * @see commerce_hipay_tpp_process_sct_notification()
 */
function hook_commerce_hipay_tpp_process_sct_notification($feedback, $transaction, $order, $processing_result) {
  // No example.
}

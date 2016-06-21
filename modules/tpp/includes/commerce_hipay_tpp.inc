<?php

/**
 * @file
 * Miscellaneous helper functions.
 */

/**
 * Returns an order from the received feedback.
 *
 * @param array $feedback
 *   An associative array containing the Hipay API call feedback.
 *
 * @return object|false
 *   An order object, or FALSE if failed to load.
 */
function commerce_hipay_tpp_feedback_get_order($feedback) {
  return (!empty($feedback['cdata1'])) ? commerce_order_load((int) $feedback['cdata1']) : FALSE;
}

/**
 * Returns a payment transaction from the received feedback.
 *
 * @param array $feedback
 *   An associative array containing the Hipay API call feedback.
 *
 * @return object|false
 *   A payment transaction object, or FALSE if failed to load.
 */
function commerce_hipay_tpp_feedback_get_payment_transaction($feedback) {
  return (!empty($feedback['cdata2'])) ? commerce_payment_transaction_load((int) $feedback['cdata2']) : FALSE;
}

/**
 * Returns an operation payment transaction from the received feedback.
 *
 * @param array $feedback
 *   An associative array containing the Hipay API call feedback.
 *
 * @return object|false
 *   A payment transaction object, or FALSE if failed to load.
 */
function commerce_hipay_tpp_feedback_get_operation_payment_transaction($feedback) {
  return (!empty($feedback['operation']['id'])) ? commerce_payment_transaction_load((int) $feedback['operation']['id']) : FALSE;
}

/**
 * Validates payment redirect key value received in the feedback
 * .
 * @param array $feedback
 *   An associative array containing the Hipay API call feedback.
 * @param object $order
 *   An order object being paid for.
 *
 * @return bool
 *   A boolean indicating whether the payment redirect key value matches
 *   the relevant values saved in the order.
 */
function commerce_hipay_tpp_feedback_validate_payment_redirect_key($feedback, $order) {
  return (empty($feedback['cdata3']) || $feedback['cdata3'] != $order->data['payment_redirect_key']) ? FALSE : TRUE;
}

/**
 * Validates the feedback signature.
 *
 * @param array $feedback
 *   An associative array containing the Hipay API call feedback.
 * @param array $payment_method
 *   The payment method instance used for the payment transaction.
 *
 * @return bool
 *   A boolean indicating whether the signature was correctly validated or not.
 *   Note that it also returns TRUE if the validation was not possible: in case
 *   when the signature was not provided by the gateway, or the passphrase is
 *   not configured locally.
 */
function commerce_hipay_tpp_feedback_validate_signature($feedback, $payment_method) {
  // Validate the signature either if it was provided by the gateway
  // or when the secret passphrase is configured locally.
  if (
    !empty($payment_method['settings']['secret_passphrase'])
    // Offsite redirect return will send the signature in the 'hash' feedback
    // parameter, while asynchronous server-to-server notification will use
    // the 'HTTP_X_ALLOPASS_SIGNATURE' header.
    && (!empty($_SERVER['HTTP_X_ALLOPASS_SIGNATURE']) || !empty($feedback['hash']))
  ) {
    return commerce_hipay_tpp_validate_signature($feedback, $payment_method);
  }

  // If no signature was provided by the gateway, or the passphrase is not
  // configured locally, pretend everything is fine.
  return TRUE;
}

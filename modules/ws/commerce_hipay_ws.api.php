<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Allows other modules to alter Hipay Wallet SOAP API request parameters
 * before sending them to the API.
 *
 * @param array $parameters
 *   An array of parameters being sent to Hipay Wallet API.
 * @param string $resource
 *   An API resource which the request is being sent to.
 * @param array $payment_method_instance
 *   The payment method instance used for the operation.
 */
function hook_commerce_hipay_ws_api_request_alter(&$parameters, $resource, $payment_method_instance) {
  // No example.
}

/**
 * Allows other modules to do custom processing of Hipay Wallet API response.
 *
 * @param array $response
 *   An array of response parameters returned by Hipay Wallet API.
 * @param array $parameters
 *   An array of parameters being sent to Hipay Wallet API.
 * @param string $resource
 *   An API resource which the request is being sent to.
 * @param array $payment_method_instance
 *   The payment method instance used for the operation.
 */
function hook_commerce_hipay_ws_api_response($response, $parameters, $resource, $payment_method_instance) {
  // No example.
}

/**
 * Allows other modules to do custom processing of Hipay Wallet Server to Server
 * notifications.
 *
 * @param array $feedback
 *   The request received.
 */
function commerce_hipay_ws_api_notification($feedback) {
  // No example.
}

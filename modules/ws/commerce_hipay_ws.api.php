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
 * @param array $context
 *   An associative array containing additional information about the request.
 *
 * @see commerce_hipay_ws_api_request()
 */
function hook_commerce_hipay_ws_api_request_alter(&$parameters, $resource, $context) {
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
 * @param array $context
 *   An associative array containing additional information about the request.
 *
 * @see commerce_hipay_ws_api_request()
 */
function hook_commerce_hipay_ws_api_response($response, $parameters, $resource, $context) {
  // No example.
}

/**
 * Allows other modules to do custom processing of Hipay Wallet Server to Server
 * notifications.
 *
 * @param array $feedback
 *   A full array of feedback values received in Hipay Wallet notification.
 *
 * @see commerce_hipay_ws_callback_notification()
 */
function hook_commerce_hipay_ws_api_notification($feedback) {
  // No example.
}

/**
 * Allows other modules to respond to the Hipay user account identification.
 *
 * @param object $hipay_user_account
 *   A Hipay user account entity that has just been validated.
 * @param array $feedback
 *   An array of XML feedback values received in Hipay Wallet notification.
 *
 * @see commerce_hipay_ws_api_user_account_validate_notification()
 */
function hook_commerce_hipay_ws_user_account_identification($hipay_user_account, $feedback) {
  // No example.
}

/**
 * Allows other modules to respond to the Hipay bank account validation.
 *
 * @param object $hipay_bank_account
 *   A Hipay bank account entity that has just been validated.
 * @param array $feedback
 *   An array of XML feedback values received in Hipay Wallet notification.
 *
 * @see commerce_hipay_ws_api_bank_account_validate_notification()
 */
function hook_commerce_hipay_ws_bank_account_validation($hipay_bank_account, $feedback) {
  // No example.
}

/**
 * Allows other modules to alter the list of available Hipay transfer statuses.
 *
 * @param array $statuses
 *   An array of all defined Hipay transfer status values.
 */
function hook_commerce_hipay_ws_transfer_statuses_alter(&$statuses) {
  $statuses['pending_execution'] = t('Pending execution');
}

/**
 * Allows other modules to alter the list of available Hipay withdrawal statuses.
 *
 * @param array $statuses
 *   An array of all defined Hipay withdrawal status values.
 */
function hook_commerce_hipay_ws_withdrawal_statuses_alter(&$statuses) {
  $statuses['pending_execution'] = t('Pending execution');
}

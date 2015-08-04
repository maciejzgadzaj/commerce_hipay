<?php

/**
 * @file
 * Provides API response code functions for Commerce Hipay TPP.
 */

/**
 * Returns the meaning of the response error code.
 *
 * See 'Chapter 5. APPENDIX' of the Hipay API Integration Guide:
 * Cash-out approach.
 *
 * @param string $error_code
 *   The error code returned by the gateway.
 *
 * @return string
 *   The descriptive meaning of the received error code.
 */
function commerce_hipay_ws_get_error_message($error_code) {
  switch ($error_code) {
    case COMMERCE_HIPAY_WS_STATUS_ERROR_AUTHENTICATION_FAILED:
      return t('Authentication failed.');
    case COMMERCE_HIPAY_WS_STATUS_ERROR_MISSING_PARAMETER:
      return t('Missing parameter.');
    case COMMERCE_HIPAY_WS_STATUS_ERROR_PARAMETER_NOT_VALID:
      return t('Parameter not valid.');
    case COMMERCE_HIPAY_WS_STATUS_ERROR_UNAUTHORIZED_METHOD:
      return t('Unauthorized method.');
    case COMMERCE_HIPAY_WS_STATUS_ERROR_OBJECT_NOT_FOUND:
      return t('Object not found.');
    case COMMERCE_HIPAY_WS_STATUS_ERROR_TECHNICAL_ERROR:
      return t('Technical error.');
    case COMMERCE_HIPAY_WS_STATUS_ERROR_WRONG_BANK_STATUS:
      return t('Wrong bank status.');
    default:
      return t('Unknown error code: @error_code.', array(
        '@error_code' => $error_code,
      ));
  }
}

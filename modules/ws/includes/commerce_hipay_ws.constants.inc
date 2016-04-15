<?php

/**
 * @file
 * Provides constants for Commerce Hipay Wallet module.
 */

define('COMMERCE_HIPAY_WS_PRODUCTION', 'prod');
define('COMMERCE_HIPAY_WS_TEST', 'test');

// Cash-out SOAP API endpoints.
define('COMMERCE_HIPAY_WS_ENDPOINT_PRODUCTION', 'https://ws.hipay.com/soap/');
define('COMMERCE_HIPAY_WS_ENDPOINT_TEST', 'https://test-ws.hipay.com/soap/');

// SOAP API resource types.
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_ACCOUNT', 'user-account-v2/');
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_TRANSFER', 'transfer/');
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_WITHDRAWAL', 'withdrawal/');
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_LOCALE', 'locale/');
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_BUSINESS_LINES', 'business-lines-v2/');
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_WEBSITE_TOPICS', 'website-topics-v2/');

// SOAP API resources.
// Account creation.
define('COMMERCE_HIPAY_WS_RESOURCE_IS_AVAILABLE', 'isAvailable');
define('COMMERCE_HIPAY_WS_RESOURCE_CREATE_FULL_USER_ACCOUNT', 'createFullUserAccount');
// Account management.
define('COMMERCE_HIPAY_WS_RESOURCE_BANK_INFOS_CHECK', 'bankInfosCheck');
define('COMMERCE_HIPAY_WS_RESOURCE_BANK_INFOS_STATUS', 'bankInfosStatus');
define('COMMERCE_HIPAY_WS_RESOURCE_BANK_INFOS_FIELDS', 'bankInfosFields');
define('COMMERCE_HIPAY_WS_RESOURCE_BANK_INFOS_REGISTER', 'bankInfosRegister');
define('COMMERCE_HIPAY_WS_RESOURCE_GET_ACCOUNT_INFOS', 'getAccountInfos');
define('COMMERCE_HIPAY_WS_RESOURCE_GET_BALANCE', 'getBalance');
define('COMMERCE_HIPAY_WS_RESOURCE_GET_TRANSACTIONS', 'getTransactions');
define('COMMERCE_HIPAY_WS_RESOURCE_CREATE_SUBACCOUNT', 'createSubaccount');
// Transfer funds operations.
define('COMMERCE_HIPAY_WS_RESOURCE_TRANSFER', 'direct');
define('COMMERCE_HIPAY_WS_RESOURCE_WITHDRAWAL', 'create');
// 'Locale' webservice.
// See https://test-www.hipaywallet.com/dl/hipay-ws-locale-1-1-alpha.pdf
define('COMMERCE_HIPAY_WS_RESOURCE_LOCALE_CODES', 'codes');
define('COMMERCE_HIPAY_WS_RESOURCE_LOCALE_TIMEZONES', 'timezones');
define('COMMERCE_HIPAY_WS_RESOURCE_LOCALE_COUNTRIES', 'countries');
// 'Business lines' webservice.
define('COMMERCE_HIPAY_WS_RESOURCE_GET_BUSINESS_LINES', 'business-lines-v2/get');
// 'Website topics' webservice.
define('COMMERCE_HIPAY_WS_RESOURCE_GET_WEBSITE_TOPICS', 'website-topics-v2/get');

// Hipay Wallet error codes.
define('COMMERCE_HIPAY_WS_STATUS_SUCCESS', 0);
define('COMMERCE_HIPAY_WS_STATUS_ERROR_AUTHENTICATION_FAILED', 1);
define('COMMERCE_HIPAY_WS_STATUS_ERROR_MISSING_PARAMETER', 2);
define('COMMERCE_HIPAY_WS_STATUS_ERROR_PARAMETER_NOT_VALID', 3);
define('COMMERCE_HIPAY_WS_STATUS_ERROR_UNAUTHORIZED_METHOD', 4);
define('COMMERCE_HIPAY_WS_STATUS_ERROR_OBJECT_NOT_FOUND', 7);
define('COMMERCE_HIPAY_WS_STATUS_ERROR_TECHNICAL_ERROR', 13);
define('COMMERCE_HIPAY_WS_STATUS_ERROR_WRONG_BANK_STATUS', 23);

// Hipay Wallet bankInfosStatus response 'status' values.
define('COMMERCE_HIPAY_WS_ACCOUNT_NO_BANK_INFO', 'No bank information');
define('COMMERCE_HIPAY_WS_ACCOUNT_BANK_INFO_VALIDATED', 'Validated');

// Hipay Wallet accountInfosIdentified response 'status' values.
define('COMMERCE_HIPAY_WS_ACCOUNT_NOT_IDENTIFIED', 'no');
define('COMMERCE_HIPAY_WS_ACCOUNT_IDENTIFIED', 'yes');

// Hipay Wallet transfer transaction types.
define('COMMERCE_HIPAY_WS_TRANSFER_TYPE_TRANSFER', 'Envoi');
define('COMMERCE_HIPAY_WS_TRANSFER_TYPE_OTHER', ' Autre');

// Hipay Wallet transfer statuses.
define('COMMERCE_HIPAY_WS_TRANSFER_STATUS_CAPTURED', 'CAPTURED');

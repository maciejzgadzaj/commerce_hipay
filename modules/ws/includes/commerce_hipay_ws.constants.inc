<?php

/**
 * @file
 * Provides constants for Commerce Hipay Wallet module.
 */

define('COMMERCE_HIPAY_WS_PROD', 'prod');
define('COMMERCE_HIPAY_WS_TEST', 'test');

// Two different API types.
define('COMMERCE_HIPAY_WS_API_TYPE_SOAP', 'SOAP');
define('COMMERCE_HIPAY_WS_API_TYPE_REST', 'REST');

// Cash-out SOAP API endpoints.
define('COMMERCE_HIPAY_WS_ENDPOINT_SOAP_PROD', 'https://ws.hipay.com/soap/');
define('COMMERCE_HIPAY_WS_ENDPOINT_SOAP_TEST', 'https://test-ws.hipay.com/soap/');
// Cash-out REST API endpoints.
define('COMMERCE_HIPAY_WS_ENDPOINT_REST_PROD', 'https://merchant.hipaywallet.com/api/');
define('COMMERCE_HIPAY_WS_ENDPOINT_REST_TEST', 'https://test-merchant.hipaywallet.com/api/');

// SOAP API resource types.
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_SOAP_ACCOUNT', 'user-account-v2/');
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_SOAP_TRANSFER', 'transfer/');
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_SOAP_WITHDRAWAL', 'withdrawal/');
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_SOAP_LOCALE', 'locale/');
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_SOAP_BUSINESS_LINES', 'business-lines-v2/');
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_SOAP_WEBSITE_TOPICS', 'website-topics-v2/');

// SOAP API resources.
// Account creation.
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_IS_AVAILABLE', 'isAvailable');
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_CREATE_FULL_USER_ACCOUNT', 'createFullUserAccount');
// Account management.
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_CHECK', 'bankInfosCheck');
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_STATUS', 'bankInfosStatus');
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_FIELDS', 'bankInfosFields');
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_REGISTER', 'bankInfosRegister');
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_ACCOUNT_INFOS', 'getAccountInfos');
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_BALANCE', 'getBalance');
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_TRANSACTIONS', 'getTransactions');
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_CREATE_SUBACCOUNT', 'createSubaccount');
// Transfer funds operations.
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_TRANSFER', 'direct');
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_WITHDRAWAL', 'create');
// 'Locale' webservice.
// See https://test-www.hipaywallet.com/dl/hipay-ws-locale-1-1-alpha.pdf
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_LOCALE_CODES', 'codes');
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_LOCALE_TIMEZONES', 'timezones');
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_LOCALE_COUNTRIES', 'countries');
// 'Business lines' webservice.
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_BUSINESS_LINES', 'business-lines-v2/get');
// 'Website topics' webservice.
define('COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_WEBSITE_TOPICS', 'website-topics-v2/get');

// REST API resource types.
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_REST_TOOLS', 'tools/');
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_REST_LOCALE', 'tools/locale/');
define('COMMERCE_HIPAY_WS_RESOURCE_TYPE_REST_IDENTIFICATION', 'identification/');

// REST API resources.
define('COMMERCE_HIPAY_WS_RESOURCE_REST_BUSINESS_LINES', 'business-lines.json');
define('COMMERCE_HIPAY_WS_RESOURCE_REST_WEBSITE_TOPICS', 'website-topics.json');
define('COMMERCE_HIPAY_WS_RESOURCE_REST_LOCALE_CODES', 'codes.json');
define('COMMERCE_HIPAY_WS_RESOURCE_REST_LOCALE_COUNTRIES', 'countries.json');
define('COMMERCE_HIPAY_WS_RESOURCE_REST_LOCALE_TIMEZONES', 'timezones.json');
define('COMMERCE_HIPAY_WS_RESOURCE_REST_UPLOAD', 'upload');

define('COMMERCE_HIPAY_WS_API_CALLBACK_URL_DONT_SEND', 'dont_send');
define('COMMERCE_HIPAY_WS_API_CALLBACK_URL_SITE_BASE_URL', 'site_base_url');
define('COMMERCE_HIPAY_WS_API_CALLBACK_URL_CUSTOM_URL', 'custom_url');

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
define('COMMERCE_HIPAY_WS_ACCOUNT_BANK_INFO_VALIDATION_IN_PROGRESS', 'Validation in progress');
define('COMMERCE_HIPAY_WS_ACCOUNT_BANK_INFO_VALIDATED', 'Validated');

// Hipay Wallet accountInfosIdentified response 'status' values.
define('COMMERCE_HIPAY_WS_ACCOUNT_NOT_IDENTIFIED', 'no');
define('COMMERCE_HIPAY_WS_ACCOUNT_IDENTIFIED', 'yes');

// Hipay Wallet transfer transaction types.
define('COMMERCE_HIPAY_WS_TRANSFER_TYPE_TRANSFER', 'Envoi');
define('COMMERCE_HIPAY_WS_TRANSFER_TYPE_OTHER', ' Autre');

// Hipay Wallet transfer statuses.
define('COMMERCE_HIPAY_WS_TRANSFER_STATUS_CAPTURED', 'CAPTURED');

// Hipay Wallet 'createFullUseraccount' API values.
define('COMMERCE_HIPAY_WS_LEGAL_STATUS_PERSONAL', 0);
define('COMMERCE_HIPAY_WS_LEGAL_STATUS_BUSINESS', 1);
define('COMMERCE_HIPAY_WS_TITLE_MR', 1);
define('COMMERCE_HIPAY_WS_TITLE_MRS', 2);
define('COMMERCE_HIPAY_WS_TITLE_MISS', 3);

// Hipay Wallet server-to-server notification statuses.
define('COMMERCE_HIPAY_WS_NOTIFICATION_STATUS_OK', 'ok');
define('COMMERCE_HIPAY_WS_NOTIFICATION_STATUS_NOK', 'nok');

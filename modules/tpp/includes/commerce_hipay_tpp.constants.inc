<?php

/**
 * @file
 * Provides constants for Commerce Hipay TPP module.
 */

define('COMMERCE_HIPAY_TPP_PRODUCTION', 'prod');
define('COMMERCE_HIPAY_TPP_TEST', 'test');

// Service endpoints.
// http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap2-IntegrationGuidelines.html
define('COMMERCE_HIPAY_TPP_ENDPOINT_PRODUCTION', 'https://secure-gateway.hipay-tpp.com/rest/v1/');
define('COMMERCE_HIPAY_TPP_ENDPOINT_TEST', 'https://stage-secure-gateway.hipay-tpp.com/rest/v1/');

// REST API resources.
// http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources.html
define('COMMERCE_HIPAY_TPP_RESOURCE_INITIALIZATION', 'hpayment');
define('COMMERCE_HIPAY_TPP_RESOURCE_MAINTAINANCE', 'maintenance/transaction/');
define('COMMERCE_HIPAY_TPP_RESOURCE_TRANSACTION', 'transaction/');

// Hipay transaction types.
// http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-initializeHostedPaymentPage.html
// http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-maintenanceOperations.html
define('COMMERCE_HIPAY_TPP_OPERATION_SALE', 'sale');
define('COMMERCE_HIPAY_TPP_OPERATION_AUTHORIZATION', 'authorization');
define('COMMERCE_HIPAY_TPP_OPERATION_CAPTURE', 'capture');
define('COMMERCE_HIPAY_TPP_OPERATION_REFUND', 'refund');
define('COMMERCE_HIPAY_TPP_OPERATION_CANCEL', 'cancel');

// Checkout redirect modes.
// http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-initializeHostedPaymentPage.html
define('COMMERCE_HIPAY_TPP_TEMPLATE_BASIC', 'basic-js');
define('COMMERCE_HIPAY_TPP_TEMPLATE_IFRAME', 'iframe-js');

// Hipay TPP API transaction states.
// http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap6-ServerToServer.html#transaction-workflow
define('COMMERCE_HIPAY_TPP_API_STATUS_COMPLETED', 'completed');
define('COMMERCE_HIPAY_TPP_API_STATUS_PENDING', 'pending');
define('COMMERCE_HIPAY_TPP_API_STATUS_DECLINED', 'declined');
define('COMMERCE_HIPAY_TPP_API_STATUS_ERROR', 'error');

// Commerce payment transaction statuses.
define('COMMERCE_HIPAY_TPP_PAYMENT_STATUS_AUTHORIZED', 'hipay_authorized');
define('COMMERCE_HIPAY_TPP_PAYMENT_STATUS_CAPTURED', 'hipay_captured');
define('COMMERCE_HIPAY_TPP_PAYMENT_STATUS_REFUNDED', 'hipay_refunded');
define('COMMERCE_HIPAY_TPP_PAYMENT_STATUS_CANCELLED', 'hipay_cancelled');
define('COMMERCE_HIPAY_TPP_PAYMENT_STATUS_PENDING', 'hipay_pending');

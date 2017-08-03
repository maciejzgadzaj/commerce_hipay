<?php

/**
 * @file
 * Provides constants for Commerce Hipay TPP module.
 */

define('COMMERCE_HIPAY_TPP_PRODUCTION', 'prod');
define('COMMERCE_HIPAY_TPP_TEST', 'test');

// TPP REST API endpoints.
// http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap2-IntegrationGuidelines.html
define('COMMERCE_HIPAY_TPP_ENDPOINT_PRODUCTION', 'https://secure-gateway.hipay-tpp.com/rest/');
define('COMMERCE_HIPAY_TPP_ENDPOINT_TEST', 'https://stage-secure-gateway.hipay-tpp.com/rest/');
// Secure Vault Tokenization API endpoints.
define('COMMERCE_HIPAY_TPP_SECURE_VAULT_ENDPOINT_PRODUCTION', 'https://secure-vault.hipay-tpp.com/rest/v1/');
define('COMMERCE_HIPAY_TPP_SECURE_VAULT_ENDPOINT_TEST', 'https://stage-secure-vault.hipay-tpp.com/rest/v1/');

// TPP REST API resources.
// http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources.html
define('COMMERCE_HIPAY_TPP_RESOURCE_INITIALIZATION', 'hpayment');
define('COMMERCE_HIPAY_TPP_RESOURCE_ORDER', 'order');
define('COMMERCE_HIPAY_TPP_RESOURCE_MAINTAINANCE', 'maintenance/transaction/');
define('COMMERCE_HIPAY_TPP_RESOURCE_TRANSACTION', 'transaction/');
define('COMMERCE_HIPAY_TPP_RESOURCE_VIRTUAL_IBAN_CREATE', 'virtual-iban');
define('COMMERCE_HIPAY_TPP_RESOURCE_VIRTUAL_IBAN_SEARCH', 'virtual-iban/search');
// Secure Vault Tokenization API resources.
// We are not using Tokenization API token lookup or token generation resources.
// In our case tokens are generated during initializing a hosted payment page,
// and lookup is not needed anywhere.
define('COMMERCE_HIPAY_TPP_RESOURCE_TOKEN_UPDATE', 'token/update');

// Hipay transaction types.
// http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-initializeHostedPaymentPage.html
// http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-maintenanceOperations.html
define('COMMERCE_HIPAY_TPP_OPERATION_SALE', 'sale');
define('COMMERCE_HIPAY_TPP_OPERATION_AUTHORIZATION', 'authorization');
define('COMMERCE_HIPAY_TPP_OPERATION_CAPTURE', 'capture');
define('COMMERCE_HIPAY_TPP_OPERATION_REFUND', 'refund');
define('COMMERCE_HIPAY_TPP_OPERATION_CANCEL', 'cancel');

// Hipay Electronic Commerce Indicator (ECI).
// http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-initializeHostedPaymentPage.html
// http://hipay-tpp-gateway-api.readthedocs.org/en/latest/Chap3-RESTAPIResources-requestNewOrder.html
define('COMMERCE_HIPAY_TPP_ECI_ECOMMERCE', 7);
define('COMMERCE_HIPAY_TPP_ECI_RECURRING_ECOMMERCE', 9);

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

// Commerce order statuses.
define('COMMERCE_HIPAY_TPP_ORDER_STATUS_PENDING_BANK_TRANSFER', 'pending_bank_transfer');

// Commerce payment transaction statuses.
// No custom status for capture transactions, as for this we need to use
// Drupal Commerce's standard COMMERCE_PAYMENT_STATUS_SUCCESS, as only then
// the 'commerce_payment_order_paid_in_full' rule will be invoked
// (see commerce_payment_commerce_payment_transaction_insert()).
define('COMMERCE_HIPAY_TPP_PAYMENT_STATUS_AUTHORIZED', 'hipay_authorized');
define('COMMERCE_HIPAY_TPP_PAYMENT_STATUS_CAPTURED', 'hipay_captured');
define('COMMERCE_HIPAY_TPP_PAYMENT_STATUS_PARTIALLY_REFUNDED', 'hipay_partially_refunded');
define('COMMERCE_HIPAY_TPP_PAYMENT_STATUS_REFUNDED', 'hipay_refunded');
define('COMMERCE_HIPAY_TPP_PAYMENT_STATUS_CANCELLED', 'hipay_cancelled');
define('COMMERCE_HIPAY_TPP_PAYMENT_STATUS_PENDING', 'hipay_pending');

// Hipay remote status.
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_CREATED', 101);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_CARDHOLDER_ENROLLED', 103);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_CARDHOLDER_NOT_ENROLLED', 104);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_UNABLE_TO_AUTH', 105);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_CARTHOLDER_AUTH', 106);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_AUTH_ATTEMPTED', 107);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_COULD_NOT_AUTH', 108);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_AUTH_FAILED', 109);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_BLOCKED', 110);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_DENIED', 111);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_AUTHORIZED_AND_PENDING', 112);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_REFUSED', 113);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_EXPIRED', 114);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_CANCELLED', 115);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_AUTHORIZED', 116);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_CAPTURE_REQUESTED', 117);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_CAPTURED', 118);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_PARTIALLY_CAPTURED', 119);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_COLLECTED', 120);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_PARTIALLY_COLLECTED', 121);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_SETTLED', 122);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_PARTIALLY_SETTLED', 123);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_REFUND_REQUESTED', 124);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_REFUNDED', 125);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_PARTIALLY_REFUNDED', 126);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_CHARGED_BACK', 129);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_DEBITED', 131);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_PARTIALLY_DEBITED', 132);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_AUTH_REQUEST', 140);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_AUTHENTIFICATED', 141);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_AUTHORIZATION_REQUESTED', 142);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_ACQUIRE_FOUND', 150);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_ACQUIRE_NOT_FOUND', 151);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_CARDHOLDER_ENROLLMENT_UNKNOWN', 160);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_RISK_ACCEPTED', 161);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_AUTHORIZATION_REFUSED', 163);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_CAPTURE_REFUSED', 173);
define('COMMERCE_HIPAY_TPP_REMOTE_STATUS_PENDING_PAYMENT', 200);

// Hipay TPP REST response status codes.
define('COMMERCE_HIPAY_TPP_REST_STATUS_SUCCESS', 0);

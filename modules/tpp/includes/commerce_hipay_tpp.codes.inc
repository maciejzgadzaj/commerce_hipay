<?php

/**
 * @file
 * Provides API response code functions for Commerce Hipay TPP.
 */

/**
 * Returns the meaning of the response payment status code.
 *
 * See 'Appendix B. Payment Status Definitions' of the Hipay Fullservice
 * Payment Gateway API Integration Guide.
 *
 * @param string $payment_status_code
 *   The payment status code returned by the gateway.
 * @return string
 *   The descriptive meaning of the received payment status code.
 */
function commerce_hipay_tpp_get_payment_status_message($payment_status_code) {
  switch ($payment_status_code) {
    case 101: return t('Created: The payment attempt was created.');
    case 103: return t('Cardholder Enrolled: Card is enrolled in the 3-D Secure program. The merchant has to forward the cardholder to the authentication pages of the card issuer.');
    case 104: return t('Cardholder Not Enrolled: Card is not enrolled in 3-D Secure program.');
    case 104: return t('Cardholder Not Enrolled: Card is not enrolled in 3-D Secure program.');
    case 105: return t('Unable to Authenticate: Unable to complete the authentication request.');
    case 106: return t('Cardholder Authenticated: Cardholder was successfully authenticated in the 3-D Secure program.');
    case 107: return t('Authentication Attempted: The Merchant has attempted to authenticate the cardholder in the 3-D Secure program and either the Issuer or cardholder is not enrolled.');
    case 108: return t('Could Not Authenticate: The Issuer is not able to complete the authentication request.');
    case 109: return t('Authentication Failed: Cardholder authentication failed. Authorization request should not be submitted.');
    case 110: return t('Blocked: The transaction has been rejected for reasons of suspected fraud.');
    case 111: return t('Denied: Merchant denied the payment attempt. After reviewing the fraud screening result, the merchant decided to decline the payment.');
    case 112: return t('Authorized and Pending: The payment was challenged by the fraud rule set and is pending.');
    case 113: return t('Refused: The financial institution refused to authorize the payment.');
    case 114: return t('Expired: The validity period of the payment authorization has expired.');
    case 115: return t('Cancelled: Merchant cancelled the payment attempt.');
    case 116: return t('Authorized: The financial institution has approved the payment.');
    case 117: return t('Capture Requested: A capture request has been sent to the financial institution.');
    case 118: return t('Captured: The financial institution has processed the payment.');
    case 119: return t('Partially Captured: The financial institution has processed part of the payment.');
    case 120: return t('Collected: The funds have been made available for remittance to the merchant.');
    case 121: return t('Partially Collected: A part of the transaction has been collected.');
    case 122: return t('Settled: The financial operations linked to this transaction are closed. Funds have been debited or credited from your merchant account at Hipay.');
    case 123: return t('Partially Settled: A part of the financial operations linked to this transaction is closed.');
    case 124: return t('Refund Requested: A refund request has been sent to the financial institution.');
    case 125: return t('Refunded: The payment was refunded.');
    case 126: return t('Partially Refunded: A part of the transaction has been refunded.');
    case 129: return t('Charged Back: The cardholder reversed a capture processed by their bank or credit card company.');
    case 131: return t('Debited: The acquirer has informed us that a debit linked to the transaction is going to be applied.');
    case 132: return t('Partially Debited: The acquirer has informed us that a partial debit linked to the transaction is going to be applied.');
    case 140: return t('Authentication Requested: The payment method used requires authentication, authentication request was send and system is waiting for a customers’ action.');
    case 141: return t('Authenticated: The payment method used requires authentication and it was successfull.');
    case 142: return t('Authorization Requested: The payment method used requires an authorization request; the request was send and the system is waiting for the financial institution approval.');
    case 150: return t('Acquirer Found: The acquirer payment route has been found.');
    case 151: return t('Acquirer not Found: The acquirer payment route has not been found.');
    case 160: return t('Cardholder Enrollment Unknown: Unable to verify if the card is enrolled in the 3-D Secure program.');
    case 161: return t('Risk Accepted: The payment has been accepted by the fraud rule set.');
    case 163: return t('Authorization Refused: The authorization was refused by the financial institution.');
    case 173: return t('Capture Refused: The capture was refused by the financial institution.');
    case 200: return t('Pending Payment: The transaction request was submitted to the acquirer but response is not yet available.');
  }
}

/**
 * Returns the meaning of the AVS response code.
 *
 * See 'Appendix D. Address Verification Service' of the Hipay Fullservice
 * Payment Gateway API Integration Guide.
 *
 * @param string $avs_code
 *   The AVS code returned by the gateway.
 * @return string
 *   The descriptive meaning of the received AVS code.
 */
function commerce_hipay_tpp_get_avs_result_message($avs_code) {
  switch ($avs_code) {
    case '':  return t('Not applicable: No AVS response was obtained.');
    case 'Y': return t('Exact Match: Street addresses and postal codes match.');
    case 'A': return t('Partial Match: The street addresses match; the postal codes do not match.');
    case 'P': return t('Partial Match: The postal codes match; the street addresses do not match.');
    case 'N': return t('No Match: Neither the street addresses nor the postal codes match.');
    case 'C': return t('Not Compatible: Street addresses and postal codes not verified due to incompatible formats.');
    case 'E': return t('Not Allowed: AVS data is invalid or AVS is not allowed for this card type.');
    case 'U': return t('Unavailable: Address information is unavailable for that account number, or the card issuer does not support AVS.');
    case 'R': return t('Retry: Issuer authorization system is unavailable, try again later.');
    case 'S': return t('Not Supported: Card issuer does not support AVS.');
    default:  return t('Unknown AVS code: @avs_code.', array('@avs_code' => $avs_code));
  }
}

/**
 * Returns the meaning of the CVC response code.
 *
 * See 'Appendix E. Card Verification Code' of the Hipay Fullservice
 * Payment Gateway API Integration Guide.
 *
 * @param string $cvc_code
 *   The CVC code returned by the gateway.
 * @return string
 *   The descriptive meaning of the received CVC code.
 */
function commerce_hipay_tpp_get_cvc_result_message($cvc_code) {
  switch ($cvc_code) {
    case '':  return t('CVC check was not possible.');
    case 'M': return t('CVC match.');
    case 'N': return t('CVC does not match.');
    case 'P': return t('CVC request not processed.');
    case 'S': return t("Missing: CVC should be on the card, but the cardholder has reported that it isn't.");
    case 'U': return t('Card issuer does not support CVC.');
    default:  return t('Unknown CVC code: @cvc_code.', array('@cvc_code' => $cvc_code));
  }
}

/**
 * Returns the meaning of the error code.
 *
 * See 'Appendix F. Decline Reasons and Error Codes' of the Hipay Fullservice
 * Payment Gateway API Integration Guide.
 *
 * @param string $error_code
 *   The error code returned by the gateway.
 * @return string
 *   The descriptive meaning of the received error code.
 */
function commerce_hipay_tpp_get_error_message($cvc_code) {
  switch ($cvc_code) {
    case '1000001':  return t('Username and/or password is incorrect.');
    case '1000002':  return t('The signature that was sent does not match the requested format.');
    case '1000003':  return t('Account is inactive.');
    case '1000004':  return t('Account is locked.');
    case '1000005':  return t('You do not have permission to make this API call.');
    case '1000006':  return t('API access is disabled for this account.');
    case '1000007':  return t('API version is not supported.');
    case '1000008':  return t('The gateway is temporarily unavailable.');
    case '1000009':  return t('The request was rejected due to IP restriction.');
    // Configuration errors.
    case '1010001':  return t('The specified HTTP Method is not allowed for this API call.');
    case '1010101':  return t('A required parameter is missing.');
    case '1010201':  return t('A parameter is in an invalid format.');
    case '1010202':  return t('The parameter value exceeds the maximum number of characters allowed.');
    case '1010203':  return t('Parameter contains non alphabetic characters.');
    case '1010204':  return t('Parameter contains non numeric characters.');
    case '1010205':  return t('The specified parameter is expected to be in decimal format, but does not appear to be a valid decimal.');
    case '1010206':  return t('The specified parameter does not seem to be a valid date.');
    case '1010207':  return t('The specified parameter does not seem to be a valid time.');
    case '1010208':  return t('The merchant entered an IP address that was in an invalid format. The IP address must be in a format such as 123.456.123.456.');
    case '1010209':  return t('The merchant entered an email address that was in an invalid format.');
    case '1010301':  return t('The soft descriptor contains invalid characters.');
    // Validation errors.
    case '1020001':  return t('The requested payment product is not configured for your account.');
    case '1020002':  return t('The specified ECI is not supported by the gateway.');
    case '1020003':  return t('The specified payment product is not valid.');
    case '3000001':  return t('Order not found.');
    case '3000002':  return t('Transaction not found.');
    case '3000003':  return t('Merchant account does not exist.');
    case '3000101':  return t('Unsupported Operation.');
    case '3000102':  return t('The IP address cannot be detected. Transaction cannot be processed without a valid IP address.');
    case '3000201':  return t('The transaction has been rejected by the financial institution for reasons of suspected fraud.');
    case '3030001':  return t('The transaction has been rejected by HiPay for reasons of suspected fraud.');
    case '3040001':  return t('The specified token was not found in the secure vault.');
    case '3010001':  return t('Currency is not supported.');
    case '3010002':  return t('The amount exceeds the maximum amount allowed for a single transaction.');
    case '3010003':  return t('You have exceeded the maximum number of payment attempts for this order.');
    case '3010004':  return t('Order was already processed.');
    case '3010005':  return t('This session has expired. Order is no longer valid.');
    case '3010006':  return t('Order has already been completed.');
    case '3010007':  return t('Order has expired.');
    case '3010008':  return t('Order is voided.');
    case '3020001':  return t('Authorization has expired.');
    case '3020002':  return t('Amount specified exceeds allowable limit.');
    case '3020101':  return t('Capture feature is not enabled for the merchant.');
    case '3020102':  return t('You cannot capture this type of transaction.');
    case '3020103':  return t('You cannot partially capture this type of transaction.');
    case '3020104':  return t('You do not have permission to capture this transaction.');
    case '3020105':  return t('Currency of capture must be the same as currency of authorization.');
    case '3020106':  return t('Authorization has already been completed.');
    case '3020107':  return t('Maximum number of allowable captures has been reached.');
    case '3020108':  return t('Invalid Amount: The capture amount must be a positive amount.');
    case '3020109':  return t('The capture amount must be less than or equal to the original transaction amount.');
    case '3020110':  return t('The partial capture amount must be less than or equal to the remaining amount.');
    case '3020111':  return t('Operation Not Permitted: The transaction is closed.');
    case '3020112':  return t('Operation Not Permitted: This transaction cannot be processed because it has been denied by the fraud rule set.');
    case '3020201':  return t('Refund feature is not enabled for the merchant.');
    case '3020202':  return t('You cannot refund this type of transaction.');
    case '3020203':  return t('You cannot partially refund this type of transaction.');
    case '3020204':  return t('You do not have permission to refund this transaction.');
    case '3020205':  return t('The refund must be the same currency as the original transaction.');
    case '3020206':  return t('This transaction has already been fully refunded.');
    case '3020207':  return t('Maximum number of allowable refunds has been reached. No more refund for the transaction.');
    case '3020208':  return t('The refund amount must be a positive amount.');
    case '3020209':  return t('The refund amount must be less than or equal to the original transaction amount.');
    case '3020210':  return t('The partial refund amount must be less than or equal to the remaining amount.');
    case '3020211':  return t('Operation Not Permitted: The transaction is closed.');
    case '3020212':  return t('Too Late: You are over the time limit to perform a refund on this transaction.');
    case '3020301':  return t('Reauthorization feature is not enabled for the merchant.');
    case '3020302':  return t('Reauthorization is not allowed for this type of transaction.');
    case '3020303':  return t('You can reauthorize only the original authorization, not a reauthorization.');
    case '3020304':  return t('Maximum number of reauthorization allowed for the authorization is reached.');
    case '3020401':  return t('You cannot void this type of transaction.');
    case '3020402':  return t('You can void only the original authorization, not a reauthorization.');
    case '3020403':  return t('Authorization has already been voided.');
    case '4000001':  return t('The transaction was declined by the acquirer.');
    case '4000002':  return t('Payment was refused by the financial institution.');
    case '4000003':  return t("The shopper's account does not have sufficient funds.");
    case '4000004':  return t('There was a technical problem processing this transaction.');
    case '4000005':  return t('Communication Failure: This transaction cannot be processed.');
    case '4000006':  return t('This transaction cannot be processed because the acquirer is temporarily unavailable.');
    case '4000007':  return t('Duplicate Transaction: Transaction was already processed.');
    case '4000008':  return t('Transaction was cancelled by the customer.');
    case '4000009':  return t('Transaction type not valid.');
    case '4000010':  return t('An issue occurred with your acquirer, please contact HiPay.');
    case '4000011':  return t('The authentication requested by the payment method was failed. Please retry or cancel.');
    case '4000012':  return t('No UID configured for this operation: The payment method used for this transaction is not supported on actual account configuration.');
    case '4010101':  return t('Transaction declined by the card issuer with no explanation given as to the reason.');
    case '4010102':  return t('The authorization centre of the card issuer is not operational at this time.');
    case '4010103':  return t('The cardholder does not have enough credit to make this payment.');
    case '4010201':  return t('Transaction not permitted for this type of card.');
    case '4010202':  return t('The transaction failed as a result of an invalid credit card number.');
    case '4010203':  return t('The type of card is not supported or is unknown.');
    case '4010204':  return t('Transaction declined because the expiry date on the card used for payment has already passed.');
    case '4010205':  return t('Transaction declined because the expiry date of the card used for payment does not correspond with the correct date.');
    case '4010206':  return t('This transaction cannot be processed because no Card Verification Code was provided.');
    case '4010207':  return t('The transaction was declined because the CVC entered does not match the credit card.');
    case '4010301':  return t('This transaction was refused because the AVS response returned the value of N, and the merchant account is not able to accept such transactions.');
    case '4010302':  return t('The bank placed a hold on purchases due to issue with the cardholder account.');
    case '4010303':  return t('Card blocked by the card issuer because the cardholder has reported it as being lost or stolen (potential fraud).');
    case '4010304':  return t('The credit card is blacklisted by card association.');
    case '4010305':  return t('The transaction would exceed card monthly limit.');
    case '4010306':  return t('The card was rejected by the bank’s fraud system.');
    case '4010307':  return t('The country IP address used is not authorized.');
    case '4010309':  return t('The credit card number is not in an authorised cards database.');
  }
}

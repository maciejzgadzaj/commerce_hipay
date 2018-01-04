<?php

namespace Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway;

use Drupal\commerce_payment\Entity\PaymentInterface;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\OffsitePaymentGatewayInterface;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\SupportsAuthorizationsInterface;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\SupportsRefundsInterface;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\SupportsStoredPaymentMethodsInterface;

/**
 * Interface CreditCardInterface
 *
 * Provides the interface for Hipay TPP Credit Card payment method.
 *
 * @package Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway
 *
 * @see \Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway\CreditCard
 */
interface CreditCardInterface extends OffsitePaymentGatewayInterface, SupportsStoredPaymentMethodsInterface, SupportsAuthorizationsInterface, SupportsRefundsInterface {

  /**
   * Initializes a Hipay secure hosted payment page API request.
   *
   * Builds the data for the request and make the request.
   *
   * @param \Drupal\commerce_payment\Entity\PaymentInterface $payment
   *   The payment.
   * @param array $extra
   *   Extra data needed for this request.
   *
   * @return array
   *   Hipay response data.
   *
   * @see https://developer.hipay.com/doc-api/enterprise/gateway/#!/payments/generateHostedPaymentPage
   */
  public function initializeHostedPaymentPage(PaymentInterface $payment, array $extra);

  /**
   * Creates a Hipay order and a transaction based on payment details API request.
   *
   * Builds the data for the request and make the request.
   *
   * @param \Drupal\commerce_payment\Entity\PaymentInterface $payment
   *   The payment.
   * @param array $extra
   *   Extra data needed for this request.
   *
   * @return array
   *   Hipay response data.
   *
   * @see https://developer.hipay.com/doc-api/enterprise/gateway/#!/payments/requestNewOrder
   */
  public function requestNewOrder(PaymentInterface $payment, array $extra);

  /**
   * Performs a Hipay Hosted Payment Page API request.
   *
   * @return \HiPay\Fullservice\Gateway\Model\HostedPaymentPage
   *   Hipay response data.
   *
   * @see https://developer.hipay.com/doc-api/enterprise/gateway/#!/payments/generateHostedPaymentPage
   */
  public function doHostedPaymentPageRequest();

  /**
   * Performs a Hipay Order API request.
   *
   * @return \HiPay\Fullservice\Gateway\Model\Transaction
   *   Hipay response data.
   *
   * @see https://developer.hipay.com/doc-api/enterprise/gateway/#!/payments/requestNewOrder
   */
  public function doOrderRequest();

}

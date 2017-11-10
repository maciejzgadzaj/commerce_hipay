<?php

namespace Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway;

use Drupal\commerce_payment\Entity\PaymentInterface;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\OffsitePaymentGatewayInterface;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\SupportsAuthorizationsInterface;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\SupportsRefundsInterface;
use HiPay\Fullservice\Gateway\Request\Order\HostedPaymentPageRequest;

/**
 * Provides the interface for Hipay TPP Credit Card payment method.
 */
interface CreditCardInterface extends OffsitePaymentGatewayInterface, SupportsAuthorizationsInterface, SupportsRefundsInterface {

  /**
   * Initialize hosted payment page API Operation request.
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
   * @see https://developer.paypal.com/docs/classic/api/merchant/SetExpressCheckout_API_Operation_NVP/
   */
  public function initializeHostedPaymentPage(PaymentInterface $payment, array $extra);

  /**
   * Performs a Hipay Hosted Payment Page API request.
   *
   * @param HostedPaymentPageRequest $hosted_payment_page_request
   *   The NVP API data array as documented.
   *
   * @return \HiPay\Fullservice\Gateway\Model\HostedPaymentPage
   *   Hipay response data.
   *
   * @see https://developer.paypal.com/docs/classic/api/#express-checkout
   */
  public function doHostedPaymentPageRequest(HostedPaymentPageRequest $hosted_payment_page_request);

}

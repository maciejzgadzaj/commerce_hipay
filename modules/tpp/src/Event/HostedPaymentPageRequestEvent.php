<?php

namespace Drupal\commerce_hipay_tpp\Event;

use Drupal\commerce_payment\Entity\PaymentInterface;
use HiPay\Fullservice\Gateway\Request\Order\HostedPaymentPageRequest;
use Symfony\Component\EventDispatcher\Event;

/**
 * Class HostedPaymentPageRequestEvent
 *
 * Defines the Hosted Payment Page request event.
 *
 * @package Drupal\commerce_hipay_tpp\Event
 *
 * @see \Drupal\commerce_hipay_tpp\Event\HipayEvents
 */
class HostedPaymentPageRequestEvent extends Event {

  /**
   * The Hosted Payment Page API request data object.
   *
   * @var HostedPaymentPageRequest
   */
  protected $hostedPaymentPageRequest;

  /**
   * The payment transaction.
   *
   * @var PaymentInterface
   */
  protected $payment;

  /**
   * Constructs a new HostedPaymentPageRequestEvent object.
   *
   * @param HostedPaymentPageRequest $hosted_payment_page_request
   *   The Hosted Payment Page API request data object as documented.
   * @param PaymentInterface $payment
   *   The payment entity, or null.
   */
  public function __construct(HostedPaymentPageRequest $hosted_payment_page_request, PaymentInterface $payment = NULL) {
    $this->hostedPaymentPageRequest = $hosted_payment_page_request;
    $this->payment = $payment;
  }

  /**
   * Gets the payment.
   *
   * @return PaymentInterface|null
   *   The payment, or NULL if unknown.
   */
  public function getPayment() {
    return $this->payment;
  }

  /**
   * Gets the Hosted Payment Page API request data object.
   *
   * @return HostedPaymentPageRequest
   *   The Hosted Payment Page API request data object.
   */
  public function getHostedPaymentPageRequest() {
    return $this->hostedPaymentPageRequest;
  }

  /**
   * Sets the Hosted Payment Page API request data object.
   *
   * @param HostedPaymentPageRequest $hosted_payment_page_request
   *   The Hosted Payment Page API request data object as documented.
   *
   * @return $this
   */
  public function setHostedPaymentPageRequest(HostedPaymentPageRequest $hosted_payment_page_request) {
    $this->hostedPaymentPageRequest = $hosted_payment_page_request;
    return $this;
  }

}

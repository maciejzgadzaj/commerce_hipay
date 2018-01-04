<?php

namespace Drupal\commerce_hipay_tpp\Event;

use Drupal\commerce_payment\Entity\PaymentInterface;
use HiPay\Fullservice\Gateway\Model\HostedPaymentPage;
use Symfony\Component\EventDispatcher\Event;

/**
 * Class HostedPaymentPageResponseEvent
 *
 * Defines the Hosted Payment Page response event.
 *
 * @package Drupal\commerce_hipay_tpp\Event
 *
 * @see \Drupal\commerce_hipay_tpp\Event\HipayEvents
 */
class HostedPaymentPageResponseEvent extends Event {

  /**
   * The Hosted Payment Page API response data object.
   *
   * @var HostedPaymentPage
   */
  protected $hostedPaymentPage;

  /**
   * The payment transaction.
   *
   * @var PaymentInterface
   */
  protected $payment;

  /**
   * Constructs a new HostedPaymentPageResponseEvent object.
   *
   * @param HostedPaymentPage $hosted_payment_page
   *   The Hosted Payment Page API response data object as documented.
   * @param PaymentInterface $payment
   *   The payment entity, or null.
   */
  public function __construct(HostedPaymentPage $hosted_payment_page, PaymentInterface $payment = NULL) {
    $this->hostedPaymentPage = $hosted_payment_page;
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
   * Gets the Hosted Payment Page API response data object.
   *
   * @return HostedPaymentPage
   *   The Hosted Payment Page API response data object.
   */
  public function getHostedPaymentPage() {
    return $this->hostedPaymentPage;
  }

}

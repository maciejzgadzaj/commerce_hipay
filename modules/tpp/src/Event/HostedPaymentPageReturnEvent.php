<?php

namespace Drupal\commerce_hipay_tpp\Event;

use Drupal\commerce_payment\Entity\PaymentInterface;
use Symfony\Component\EventDispatcher\Event;

/**
 * Class HostedPaymentPageReturnEvent
 *
 * Defines the Hosted Payment Page return event.
 *
 * @package Drupal\commerce_hipay_tpp\Event
 *
 * @see \Drupal\commerce_hipay_tpp\Event\HipayEvents
 */
class HostedPaymentPageReturnEvent extends Event {

  /**
   * The Hosted Payment Page API return data array.
   *
   * @var array
   */
  protected $returnData;

  /**
   * The payment transaction.
   *
   * @var PaymentInterface
   */
  protected $payment;

  /**
   * Constructs a new HostedPaymentPageReturnEvent object.
   *
   * @param array $return_data
   *   The Hosted Payment Page API return data array.
   * @param PaymentInterface $payment
   *   The payment entity, or null.
   */
  public function __construct(array $return_data, PaymentInterface $payment = NULL) {
    $this->returnData = $return_data;
    $this->payment = $payment;
  }

  /**
   * Gets the Hosted Payment Page API return data array.
   *
   * @return array
   *   The Hosted Payment Page API return data array.
   */
  public function getReturnData() {
    return $this->returnData;
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

}

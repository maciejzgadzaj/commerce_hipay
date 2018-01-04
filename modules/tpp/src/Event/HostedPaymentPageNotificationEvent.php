<?php

namespace Drupal\commerce_hipay_tpp\Event;

use Drupal\commerce_payment\Entity\PaymentInterface;
use Symfony\Component\EventDispatcher\Event;

/**
 * Class HostedPaymentPageNotificationEvent
 *
 * Defines the Hosted Payment Page notification event.
 *
 * @package Drupal\commerce_hipay_tpp\Event
 *
 * @see \Drupal\commerce_hipay_tpp\Event\HipayEvents
 */
class HostedPaymentPageNotificationEvent extends Event {

  /**
   * The Hosted Payment Page API notification data array.
   *
   * @var array
   */
  protected $notificationData;

  /**
   * The payment transaction.
   *
   * @var PaymentInterface
   */
  protected $payment;

  /**
   * Constructs a new HostedPaymentPageNotificationEvent object.
   *
   * @param array $notification_data
   *   The Hosted Payment Page API notification data array.
   * @param PaymentInterface $payment
   *   The payment entity, or null.
   */
  public function __construct(array $notification_data, PaymentInterface $payment = NULL) {
    $this->notificationData = $notification_data;
    $this->payment = $payment;
  }

  /**
   * Gets the Hosted Payment Page API notification data array.
   *
   * @return array
   *   The Hosted Payment Page API notification data array.
   */
  public function getNotificationData() {
    return $this->notificationData;
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

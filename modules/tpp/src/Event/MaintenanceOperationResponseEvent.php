<?php

namespace Drupal\commerce_hipay_tpp\Event;

use Drupal\commerce_payment\Entity\PaymentInterface;
use Symfony\Component\EventDispatcher\Event;

/**
 * Class MaintenanceOperationResponseEvent
 *
 * Defines the Maintenance Operation response event.
 *
 * @package Drupal\commerce_hipay_tpp\Event
 *
 * @see \Drupal\commerce_hipay_tpp\Event\HipayEvents
 */
class MaintenanceOperationResponseEvent extends Event {

  /**
   * The Maintenance Operation response data array.
   *
   * @var array
   */
  protected $responseData;

  /**
   * The payment transaction.
   *
   * @var PaymentInterface
   */
  protected $payment;

  /**
   * Constructs a new MaintenanceOperationResponseEvent object.
   *
   * @param array $response_data
   *   The Maintenance Operation response data array.
   * @param PaymentInterface $payment
   *   The payment entity, or null.
   */
  public function __construct(array $response_data, PaymentInterface $payment = NULL) {
    $this->responseData = $response_data;
    $this->payment = $payment;
  }

  /**
   * Gets the Maintenance Operation response data array.
   *
   * @return array
   *   The Maintenance Operation response data array.
   */
  public function getResponseData() {
    return $this->responseData;
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

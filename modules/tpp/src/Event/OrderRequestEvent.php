<?php

namespace Drupal\commerce_hipay_tpp\Event;

use Drupal\commerce_payment\Entity\PaymentInterface;
use HiPay\Fullservice\Gateway\Request\Order\OrderRequest;
use Symfony\Component\EventDispatcher\Event;

/**
 * Class OrderRequestEvent
 *
 * Defines the Order request event.
 *
 * @package Drupal\commerce_hipay_tpp\Event
 *
 * @see \Drupal\commerce_hipay_tpp\Event\HipayEvents
 */
class OrderRequestEvent extends Event {

  /**
   * The Order API request data object.
   *
   * @var OrderRequest
   */
  protected $orderRequest;

  /**
   * The payment transaction.
   *
   * @var PaymentInterface
   */
  protected $payment;

  /**
   * Constructs a new OrderRequestEvent object.
   *
   * @param OrderRequest $order_request
   *   The Order API request data object as documented.
   * @param PaymentInterface $payment
   *   The payment entity, or null.
   */
  public function __construct(OrderRequest $order_request, PaymentInterface $payment = NULL) {
    $this->orderRequest = $order_request;
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
   * Gets the Order API request data object.
   *
   * @return OrderRequest
   *   The Order API request data object.
   */
  public function getOrderRequest() {
    return $this->orderRequest;
  }

  /**
   * Sets the Order API request data object.
   *
   * @param OrderRequest $order_request
   *   The Order API request data object as documented.
   *
   * @return $this
   */
  public function setOrderRequest(OrderRequest $order_request) {
    $this->orderRequest = $order_request;
    return $this;
  }

}

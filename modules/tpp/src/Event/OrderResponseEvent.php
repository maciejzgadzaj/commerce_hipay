<?php

namespace Drupal\commerce_hipay_tpp\Event;

use Drupal\commerce_payment\Entity\PaymentInterface;
use HiPay\Fullservice\Gateway\Model\Transaction;
use Symfony\Component\EventDispatcher\Event;

/**
 * Class OrderResponseEvent
 *
 * Defines the Order response event.
 *
 * @package Drupal\commerce_hipay_tpp\Event
 *
 * @see \Drupal\commerce_hipay_tpp\Event\HipayEvents
 */
class OrderResponseEvent extends Event {

  /**
   * The Order response data object.
   *
   * @var Transaction
   */
  protected $transaction;

  /**
   * The payment transaction.
   *
   * @var PaymentInterface
   */
  protected $payment;

  /**
   * Constructs a new OrderResponseEvent object.
   *
   * @param Transaction $transaction
   *   The Order response data object as documented.
   * @param PaymentInterface $payment
   *   The payment entity, or null.
   */
  public function __construct(Transaction $transaction, PaymentInterface $payment = NULL) {
    $this->transaction = $transaction;
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
   * Gets the Order response data object.
   *
   * @return Transaction
   *   The Order response data object.
   */
  public function getTransaction() {
    return $this->transaction;
  }

}

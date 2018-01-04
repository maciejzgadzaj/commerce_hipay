<?php

namespace Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway;

use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\OnsitePaymentGatewayInterface;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\SupportsAuthorizationsInterface;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\SupportsRefundsInterface;

/**
 * Interface DirectDebitInterface
 *
 * Provides the interface for Hipay TPP Direct Debit payment method.
 *
 * @package Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway
 *
 * @see \Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway\DirectDebit
 */
interface DirectDebitInterface extends OnsitePaymentGatewayInterface, SupportsAuthorizationsInterface, SupportsRefundsInterface {

}

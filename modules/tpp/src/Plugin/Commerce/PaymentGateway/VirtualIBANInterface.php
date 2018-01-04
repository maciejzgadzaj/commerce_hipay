<?php

namespace Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway;

use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\OnsitePaymentGatewayInterface;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\SupportsRefundsInterface;

/**
 * Interface VirtualIBANInterface
 *
 * Provides the interface for Hipay TPP Virtual IBAN payment method.
 *
 * @package Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway
 *
 * @see \Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway\VirtualIBAN
 */
interface VirtualIBANInterface extends OnsitePaymentGatewayInterface, SupportsRefundsInterface {

}

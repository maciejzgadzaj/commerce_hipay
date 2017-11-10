<?php

namespace Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway;

use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\OnsitePaymentGatewayInterface;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\SupportsRefundsInterface;

/**
 * Provides the interface for Hipay TPP Virtual IBAN payment method.
 */
interface VirtualIBANInterface extends OnsitePaymentGatewayInterface, SupportsRefundsInterface {

}

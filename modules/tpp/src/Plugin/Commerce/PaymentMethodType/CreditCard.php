<?php

namespace Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentMethodType;

use Drupal\commerce_payment\Entity\PaymentMethodInterface;

/**
 * Class CreditCard
 *
 * Extends default credit card payment method type for Hipay.
 *
 * @package Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentMethodType
 *
 * @see commerce_hipay_tpp_commerce_payment_method_type_info_alter()
 */
class CreditCard extends \Drupal\commerce_payment\Plugin\Commerce\PaymentMethodType\CreditCard {

  /**
   * {@inheritdoc}
   */
  public function buildLabel(PaymentMethodInterface $payment_method) {
    // Display "New credit card" instead of "<> ending in <>" for already saved
    // Hipay credit card payment methods for which the offsite redirect has not
    // yet been done and which do not have credit card type/number/remote_id
    // (credit card token) stored in them yet.
    if ($payment_method->getPaymentGatewayId() == 'hipay_tpp_credit_card' && !$payment_method->getRemoteId()) {
      return $this->t('New credit card');
    }

    // Use standard label for everything else.
    return parent::buildLabel($payment_method);
  }

}

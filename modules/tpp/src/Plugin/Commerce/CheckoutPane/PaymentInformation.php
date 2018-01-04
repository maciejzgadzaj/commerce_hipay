<?php

namespace Drupal\commerce_hipay_tpp\Plugin\Commerce\CheckoutPane;

/**
 * Class PaymentInformation
 *
 * Overrides default payment information pane.
 *
 * @package Drupal\commerce_hipay_tpp\Plugin\Commerce\CheckoutPane
 *
 * @see commerce_hipay_tpp_commerce_checkout_pane_info_alter()
 */
class PaymentInformation extends \Drupal\commerce_payment\Plugin\Commerce\CheckoutPane\PaymentInformation {

  /**
   * A boolean indicating whether temporary Hipay payment method created before
   * offsite Hipay redirect has been deleted from the order when going back in
   * the checkout process.
   *
   * @var bool
   */
  private $orderPaymentMethodDeleted = FALSE;

  /**
   * {@inheritdoc}
   */
  protected function buildPaymentMethodOptions(array $payment_gateways) {
    $options = parent::buildPaymentMethodOptions($payment_gateways);

    // When going back in the checkout process from "Order review" to "Order
    // information" page, the "Payment method" form shows all available stored
    // reusable payment methods, including a new empty one that was just created
    // for this order if "New credit card" was originally selected. This results
    // in "New credit card" option being displayed twice - one for real new
    // credit card (which shows billing details subform), and second one for
    // the not yet defined but already saved payment method stored int he order.
    // We do not want to show it twice, therefore let's remove this option from
    // the form, and also remove it from the order and delete its entity.

    /** @var \Drupal\commerce_payment\Entity\PaymentMethod $payment_method */
    if ($payment_method = $this->order->get('payment_method')->entity) {
      if ($payment_method->getPaymentGatewayId() == 'hipay_tpp_credit_card' && !$payment_method->getRemoteId()) {
        if (isset($options[$payment_method->id()])) {
          unset($options[$payment_method->id()]);
          $this->orderPaymentMethodDeleted = TRUE;

          // Delete payment method entity and billing profile as new ones will
          // be created when submitting this "Order information" form anyway,
          // and we don't want to leave any db polluting non-used entities.
          $payment_method->delete();
          /** @var \Drupal\profile\Entity\Profile $billing_profile */
          if ($billing_profile = $this->order->getBillingProfile()) {
            $billing_profile->delete();
          }
        }
      }
    }

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  protected function getDefaultPaymentMethodOption(array $options) {
    $default_option = parent::getDefaultPaymentMethodOption($options);

    // Select "New credit card" by default when going back in the checkout
    // process from "Order review" to "Order information" page, if previously
    // "New credit card" was selected.
    if ($this->orderPaymentMethodDeleted && isset($options['new--credit_card--hipay_tpp_credit_card'])) {
      $default_option = 'new--credit_card--hipay_tpp_credit_card';
    }

    return $default_option;
  }

}

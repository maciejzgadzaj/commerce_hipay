<?php

namespace Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway;

use Drupal\commerce_payment\Entity\PaymentInterface;
use Drupal\commerce_payment\Entity\PaymentMethodInterface;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\OnsitePaymentGatewayBase;
use Drupal\commerce_price\Price;
use Drupal\Core\Form\FormStateInterface;

/**
 * Class DirectDebit
 *
 * Provides Hipay TPP SEPA Direct Debit payment gateway.
 *
 * @CommercePaymentGateway(
 *   id = "hipay_tpp_direct_debit",
 *   label = "Hipay Enterprise TPP: SEPA Direct Debit (on-site)",
 *   display_label = "Direct debit",
 *   forms = {
 *     "offsite-payment" = "Drupal\commerce_hipay_tpp\PluginForm\PaymentOffsiteForm\DirectDebitPaymentOffsiteForm",
 *   }
 * )
 *
 * @package Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway
 *
 * @see \Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway\DirectDebitInterface
 */
class DirectDebit extends OnsitePaymentGatewayBase implements DirectDebitInterface {

  use ConfigurationTrait;

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return parent::defaultConfiguration()
      + self::defaultCommonConfiguration();
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    return parent::buildConfigurationForm($form, $form_state)
      + self::buildCommonConfigurationForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function validateConfigurationForm(array &$form, FormStateInterface $form_state) {
    parent::validateConfigurationForm($form, $form_state);
    self::validateCommonConfigurationForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    parent::submitConfigurationForm($form, $form_state);
    self::submitCommonConfigurationForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function createPaymentMethod(PaymentMethodInterface $payment_method, array $payment_details) {

  }

  /**
   * {@inheritdoc}
   */
  public function deletePaymentMethod(PaymentMethodInterface $payment_method) {

  }

  /**
   * {@inheritdoc}
   */
  public function createPayment(PaymentInterface $payment, $capture = TRUE) {

  }

  /**
   * {@inheritdoc}
   */
  public function capturePayment(PaymentInterface $payment, Price $amount = NULL) {

  }

  /**
   * {@inheritdoc}
   */
  public function voidPayment(PaymentInterface $payment) {

  }

  /**
   * {@inheritdoc}
   */
  public function refundPayment(PaymentInterface $payment, Price $amount = NULL) {

  }

}

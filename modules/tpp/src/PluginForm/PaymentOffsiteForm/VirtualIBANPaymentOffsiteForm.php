<?php

namespace Drupal\commerce_hipay_tpp\PluginForm\PaymentOffsiteForm;

use Drupal\Core\Form\FormStateInterface;

/**
 * Class VirtualIBANPaymentOffsiteForm.
 *
 * Provides the off-site redirect form for Virtual IBAN payments.
 *
 * @package Drupal\commerce_hipay_tpp\PluginForm\PaymentOffsiteForm
 */
class VirtualIBANPaymentOffsiteForm extends BasePaymentOffsiteForm {

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {


    return parent::buildConfigurationForm($form, $form_state);
  }

}

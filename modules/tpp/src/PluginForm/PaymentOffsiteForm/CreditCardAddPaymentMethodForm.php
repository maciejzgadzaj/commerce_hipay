<?php

namespace Drupal\commerce_hipay_tpp\PluginForm\PaymentOffsiteForm;

use Drupal\commerce_payment\PluginForm\PaymentMethodAddForm as BasePaymentMethodAddForm;
use Drupal\Core\Form\FormStateInterface;

/**
 * Class CreditCardAddPaymentMethodForm
 *
 * @package Drupal\commerce_hipay_tpp\PluginForm\PaymentOffsiteForm
 *
 * @see \Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway\CreditCard
 */
class CreditCardAddPaymentMethodForm extends BasePaymentMethodAddForm {

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form = parent::buildConfigurationForm($form, $form_state);

    // Populate "Payment information" billing details form with stored info
    // if we have it previously saved and available in the order.
    if ($order = $this->routeMatch->getParameter('commerce_order')) {
      if ($billing_profile = $order->getBillingProfile()) {
        // Although this populates "Payment information" billing details form,
        // it also in some cases messes up previously used payment methods,
        // updating them to point to a different billing profile entity.
        // @TODO?
        // $form['billing_information']['#default_value'] = $billing_profile;
      }
    }

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  protected function buildCreditCardForm(array $element, FormStateInterface $form_state) {
    // Generally we don't need anything here, the empty "payment_details" value
    // is just to avoid "Recoverable fatal error: Argument 1 passed to
    // CreditCardAddPaymentMethodForm::validateCreditCardForm() must be of the
    // type array, null given" when selecting "New credit card" option.
    return [
      'payment_details' => [
        '#type' => 'value',
        '#value' => [],
      ],
    ];
  }

  /**
   * {@inheritdoc}
   */
  protected function validateCreditCardForm(array &$element, FormStateInterface $form_state) {
    // No call to parent validation, as there is nothing to validate yet.
  }

  /**
   * {@inheritdoc}
   */
  protected function submitCreditCardForm(array $element, FormStateInterface $form_state) {
    // Temporarily mark the card as not reusable, so that it does not appear
    // in user's "Payment methods" listing yet - until the payment is completed
    // and the payment method entity is updated with card details and remote_id
    // in CreditCard::onReturn().
    $this->entity->reusable = FALSE;
  }

}

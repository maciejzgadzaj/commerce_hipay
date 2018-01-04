<?php

namespace Drupal\commerce_hipay_tpp\PluginForm\PaymentOffsiteForm;

use Drupal\commerce_payment\Exception\PaymentGatewayException;
use Drupal\commerce_payment\PluginForm\PaymentOffsiteForm;
use Drupal\Core\Form\FormStateInterface;
use HiPay\Fullservice\Enum\Transaction\Template;

/**
 * Class CreditCardPaymentOffsiteForm
 *
 * Provides the off-site redirect form for credit card payments.
 *
 * @package Drupal\commerce_hipay_tpp\PluginForm\PaymentOffsiteForm
 */
class CreditCardPaymentOffsiteForm extends BasePaymentOffsiteForm {

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form = parent::buildConfigurationForm($form, $form_state);

    /** @var \Drupal\commerce_payment\Entity\PaymentInterface $payment */
    $payment = $this->entity;

    // Save the payment so that we can get its ID and pass it to Hipay.
    $payment->save();

    /** @var \Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway\CreditCard $payment_gateway_plugin */
    $payment_gateway_plugin = $payment->getPaymentGateway()->getPlugin();

    /** @var \Drupal\commerce_order\Entity\Order $order */
    $order = $payment->getOrder();

    $extra = [
      'return_url' => $form['#return_url'],
      'cancel_url' => $form['#cancel_url'],
      'capture' => $form['#capture'],
    ];

    /** @var \Drupal\commerce_payment\Entity\PaymentMethod $payment_method */
    $payment_method = $payment->getOrder()->get('payment_method')->entity;

    // If it is previously stored payment method with "remote_id" value set,
    // we just request a new order without external redirect.
    if ($payment_method->getRemoteId()) {
      /** @var \HiPay\Fullservice\Gateway\Model\Order $hipay_response */
      $hipay_response = $payment_gateway_plugin->requestNewOrder($payment, $extra);

      $checkout_order_manager = \Drupal::getContainer()->get('commerce_checkout.checkout_order_manager');
      $checkout_flow = $checkout_order_manager->getCheckoutFlow($order)->getPlugin();
      $next_step_id = $checkout_flow->getNextStepId($order->get('checkout_step')->value);
      $checkout_flow->redirectToStep($next_step_id);
    }

    // Otherwise, if this is a new payment method, we need to initialize
    // Hipay's Hosted Payment Page and execute an external redirect.

    /** @var \HiPay\Fullservice\Gateway\Model\HostedPaymentPage $hipay_response */
    $hipay_response = $payment_gateway_plugin->initializeHostedPaymentPage($payment, $extra);

    // If we didn't get the forward URL value back from Hipay, then something
    // went wrong, and we need to exit the checkout.
    if (!$hipay_response->getForwardUrl()) {
      throw new PaymentGatewayException($this->t('There was an error processing your payment with Hipay. Please try again or contact us if the problem persists.'));
    }

    $configuration = $payment_gateway_plugin->getConfiguration();

    // Standard offsite redirect.
    if ($configuration['template'] == Template::BASIC_JS) {
      return $this->buildRedirectForm($form, $form_state, $hipay_response->getForwardUrl(), [], PaymentOffsiteForm::REDIRECT_GET);
    }
    // Embedded iframe.
    else {
      $form['commerce_hipay_tpp_iframe'] = array(
        '#type' => 'inline_template',
        '#template' => '<iframe class="commerce_hipay_tpp_iframe" src="{{ forward_url }}" scrolling="no" frameborder="0" width="600px" height="500px"></iframe>',
        '#context' => [
          'forward_url' => $hipay_response->getForwardUrl(),
        ],
      );
      return $form;
    }
  }

}

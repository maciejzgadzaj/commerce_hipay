<?php

namespace Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway;

use Drupal\commerce_hipay_tpp\Hipay\HipayAPI;
use Drupal\commerce_hipay_tpp\Hipay\HipayAPI\GatewayAPI;
use Drupal\commerce_hipay_tpp\Hipay\HipayTPP;
use Drupal\Core\Form\FormStateInterface;

trait ConfigurationTrait {

  /**
   * {@inheritdoc}
   */
  public function defaultCommonConfiguration() {
    $default_configuration = [
//      'supported_currencies' => drupal_map_assoc(array_keys(commerce_hipay_get_enabled_currencies())),
//      'currency_code' => commerce_default_currency(),
      'api_logging' => array(
        'request' => FALSE,
        'response' => FALSE,
      ),
    ];

    foreach (HipayTPP::getEnabledCurrencies() as $currency_code => $currency) {
      $default_configuration['accounts'][$currency_code] = [
        'api_username' => '',
        'api_password' => '',
        'secret_passphrase' => '',
      ];
    }

    return $default_configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function buildCommonConfigurationForm(array $form, FormStateInterface $form_state) {

    foreach (HipayTPP::getEnabledCurrencies() as $currency_code => $currency) {
      $form['accounts'][$currency_code] = [
        '#type' => 'details',
        '#title' => $this->t('@currency_code account', array('@currency_code' => $currency_code)),
        '#collapsible' => TRUE,
//        '#open' => !empty($this->configuration['accounts'][$currency_code]['api_username']) && !empty($this->configuration['accounts'][$currency_code]['api_password']),
        '#open' => TRUE,
      ];

      $form['accounts'][$currency_code]['api_username'] = [
        '#type' => 'textfield',
        '#title' => $this->t('API username'),
        '#description' => $this->t('The name of the user for accessing Hipay TPP webservice. This, as well as API password, can be found in your Hipay Fullservice Account under <em>Integration / Security Settings</em>.'),
        '#default_value' => $this->configuration['accounts'][$currency_code]['api_username'],
//        '#required' => $currency_code == commerce_default_currency(),
      ];

      $form['accounts'][$currency_code]['api_password'] = [
        '#type' => 'textfield',
        '#title' => $this->t('API password'),
        '#description' => $this->t('The password for the user specified in the above field.'),
        '#default_value' => $this->configuration['accounts'][$currency_code]['api_password'],
//        '#required' => $currency_code == commerce_default_currency(),
      ];

      $form['accounts'][$currency_code]['secret_passphrase'] = [
        '#type' => 'textfield',
        '#title' => $this->t('Secret Passphrase'),
        '#description' => $this->t('This secret passphrase is used to generate a unique character string (signature) hashed with SHA algorithm. It should be the same as the value provided in <em>Secret Passphrase</em> field in your Hipay Fullservice Account configuration in <em>Integration » Security Settings</em>.'),
        '#default_value' => $this->configuration['accounts'][$currency_code]['secret_passphrase'],
//        '#required' => $currency_code == commerce_default_currency(),
      ];
    }

//    $form['supported_currencies'] = [
//      '#type' => 'checkboxes',
//      '#title' => $this->t('Supported currencies'),
//      '#description' => $this->t('Transactions in these currencies will be sent as-is to the gateway, without any prior conversion. This setting should reflect your <em>Settlement currencies</em> configuration of your Hipay Fullservice account.'),
//      '#options' => HipayTPP::getEnabledCurrenciesOptions(),
//      '#multiple' => TRUE,
//      '#default_value' => $this->configuration['supported_currencies'],
//      '#required' => TRUE,
//      '#weight' => 10,
//    ];
//
//    $form['currency_code'] = [
//      '#type' => 'select',
//      '#title' => $this->t('Default currency'),
//      '#description' => $this->t('Transactions in other currencies will be converted to this currency, so multi-currency sites must be configured to use appropriate conversion rates.'),
//      '#options' => HipayTPP::getEnabledCurrenciesOptions(),
//      '#default_value' => $this->configuration['currency_code'],
//      '#weight' => 10,
//    ];

    $form['api_logging'] = [
      '#type' => 'checkboxes',
      '#title' => $this->t('Log the following messages for debugging'),
      '#options' => array(
        'request' => $this->t('API request messages'),
        'response' => $this->t('API response messages'),
      ),
      '#default_value' => $this->configuration['api_logging'],
      '#weight' => 10,
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateCommonConfigurationForm(array &$form, FormStateInterface $form_state) {

  }

  /**
   * {@inheritdoc}
   */
  public function submitCommonConfigurationForm(array &$form, FormStateInterface $form_state) {
    if (!$form_state->getErrors()) {
      $values = $form_state->getValue($form['#parents']);

      $this->configuration['accounts'] = $values['accounts'];
      $this->configuration['api_logging'] = $values['api_logging'];
    }
  }

  public function getHipayAPIUri($version = 'v1') {
    $payment_gateway_configuration = $this->getConfiguration();
    $constant_name = 'ENDPOINT_' . strtoupper($payment_gateway_configuration['mode']);
    return constant("\Drupal\commerce_hipay_tpp\Hipay\HipayAPI\GatewayAPI::$constant_name") . $version .'/';
  }

  /**
   * {@inheritdoc}
   */
  public function getNotifyUrl() {
    return Url::fromRoute('commerce_payment.notify', [
      'commerce_payment_gateway' => $this->entityId,
    ], ['absolute' => TRUE]);
  }

}

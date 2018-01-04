<?php

namespace Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway;

use Drupal\commerce_hipay_tpp\Event\HipayEvents;
use Drupal\commerce_hipay_tpp\Event\HostedPaymentPageNotificationEvent;
use Drupal\commerce_hipay_tpp\Event\HostedPaymentPageRequestEvent;
use Drupal\commerce_hipay_tpp\Event\HostedPaymentPageResponseEvent;
use Drupal\commerce_hipay_tpp\Event\HostedPaymentPageReturnEvent;
use Drupal\commerce_hipay_tpp\Event\MaintenanceOperationResponseEvent;
use Drupal\commerce_hipay_tpp\Event\OrderRequestEvent;
use Drupal\commerce_hipay_tpp\Event\OrderResponseEvent;
use Drupal\commerce_hipay_tpp\Hipay\HipayTPP;
use Drupal\commerce_order\Entity\OrderInterface;
use Drupal\commerce_payment\Entity\PaymentInterface;
use Drupal\commerce_payment\Entity\PaymentMethodInterface;
use Drupal\commerce_payment\Exception\HardDeclineException;
use Drupal\commerce_payment\Exception\InvalidRequestException;
use Drupal\commerce_payment\Exception\InvalidResponseException;
use Drupal\commerce_payment\Exception\PaymentGatewayException;
use Drupal\commerce_payment\Exception\SoftDeclineException;
use Drupal\commerce_payment\PaymentMethodTypeManager;
use Drupal\commerce_payment\PaymentTypeManager;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\OffsitePaymentGatewayBase;
use Drupal\commerce_price\Price;
use Drupal\commerce_price\RounderInterface;
use Drupal\Component\Datetime\TimeInterface;
use Drupal\Component\Serialization\Json;
use Drupal\Component\Utility\Unicode;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Language\LanguageInterface;
use Drupal\Core\Url;
use HiPay\Fullservice\Enum\Transaction\ECI;
use HiPay\Fullservice\Enum\Transaction\Template;
use HiPay\Fullservice\Enum\Transaction\TransactionState;
use HiPay\Fullservice\Enum\Transaction\TransactionStatus;
use HiPay\Fullservice\Gateway\Client\GatewayClient;
use HiPay\Fullservice\Gateway\Request\Info\CustomerBillingInfoRequest;
use HiPay\Fullservice\Gateway\Request\Info\CustomerShippingInfoRequest;
use HiPay\Fullservice\Gateway\Request\Maintenance\MaintenanceRequest;
use HiPay\Fullservice\Gateway\Request\Order\HostedPaymentPageRequest;
use HiPay\Fullservice\Gateway\Request\Order\OrderRequest;
use HiPay\Fullservice\HTTP\Configuration\Configuration;
use HiPay\Fullservice\HTTP\SimpleHTTPClient;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\EventDispatcher\EventDispatcherInterface;
use Symfony\Component\HttpFoundation\Request;

/**
 * Class CreditCard
 *
 * Provides Hipay TPP Credit Card payment gateway.
 *
 * @CommercePaymentGateway(
 *   id = "hipay_tpp_credit_card",
 *   label = "Hipay Enterprise TPP: Credit Card (off-site)",
 *   display_label = "Credit card",
 *   payment_method_types = {"credit_card"},
 *   credit_card_types = {
 *     "visa", "mastercard", "amex", "dinersclub", "discover", "jcb"
 *   },
 *   forms = {
 *     "offsite-payment" = "Drupal\commerce_hipay_tpp\PluginForm\PaymentOffsiteForm\CreditCardPaymentOffsiteForm",
 *     "add-payment-method" = "Drupal\commerce_hipay_tpp\PluginForm\PaymentOffsiteForm\CreditCardAddPaymentMethodForm",
 *   }
 * )
 *
 * @package Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway
 *
 * @see \Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway\CreditCardInterface
 */
class CreditCard extends OffsitePaymentGatewayBase implements CreditCardInterface {

  use ConfigurationTrait;

  /**
   * The Hipay API request object.
   *
   * @var HostedPaymentPageRequest|OrderRequest
   */
  protected $request;

  /**
   * The price rounder.
   *
   * @var \Drupal\commerce_price\RounderInterface
   */
  protected $rounder;

  /**
   * Constructs a new PaymentGatewayBase object.
   *
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin_id for the plugin instance.
   * @param mixed $plugin_definition
   *   The plugin implementation definition.
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The entity type manager.
   * @param \Drupal\commerce_payment\PaymentTypeManager $payment_type_manager
   *   The payment type manager.
   * @param \Drupal\commerce_payment\PaymentMethodTypeManager $payment_method_type_manager
   *   The payment method type manager.
   * @param \Drupal\Component\Datetime\TimeInterface $time
   *   The time.
   * @param \Drupal\commerce_price\RounderInterface $rounder
   *   The price rounder.
   * @param \Drupal\Core\Extension\ModuleHandlerInterface $module_handler
   *   The module handler.
   * @param \Symfony\Component\EventDispatcher\EventDispatcherInterface $event_dispatcher
   *   The event dispatcher.
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, EntityTypeManagerInterface $entity_type_manager, PaymentTypeManager $payment_type_manager, PaymentMethodTypeManager $payment_method_type_manager, TimeInterface $time, RounderInterface $rounder, ModuleHandlerInterface $module_handler, EventDispatcherInterface $event_dispatcher) {
    parent::__construct($configuration, $plugin_id, $plugin_definition, $entity_type_manager, $payment_type_manager, $payment_method_type_manager, $time);

    $this->rounder = $rounder;
    $this->moduleHandler = $module_handler;
    $this->eventDispatcher = $event_dispatcher;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('entity_type.manager'),
      $container->get('plugin.manager.commerce_payment_type'),
      $container->get('plugin.manager.commerce_payment_method_type'),
      $container->get('datetime.time'),
      $container->get('commerce_price.rounder'),
      $container->get('module_handler'),
      $container->get('event_dispatcher')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    $default_configuration = parent::defaultConfiguration() + self::defaultCommonConfiguration();

    $default_configuration += [
      'template' => Template::BASIC_JS,
      'payment_product_list' => [
        'visa' => 'visa',
        'mastercard' => 'mastercard',
        'american-express' => 'american-express',
      ],
      '3ds' => 1,
      'language' => 'en_GB',
      'css' => '',
      'sandbox' => [],
    ];

    return $default_configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form = parent::buildConfigurationForm($form, $form_state) + self::buildCommonConfigurationForm($form, $form_state);

    $form['template'] = [
      '#type' => 'radios',
      '#title' => $this->t('Checkout redirect mode'),
      '#options' => [
        Template::BASIC_JS => $this->t('Redirect to the hosted checkout page through an automatically submitted form'),
        'iframe-js' => $this->t('Stay on this site using an iframe to embed the hosted checkout page'),
      ],
      '#default_value' => $this->configuration['template'],
      '#required' => TRUE,
    ];

    $form['payment_product_list'] = [
      '#type' => 'checkboxes',
      '#title' => $this->t('Enabled payment products'),
      '#options' => [
        'visa' => $this->t('Visa'),
        'mastercard' => $this->t('MasterCard'),
        'american-express' => $this->t('American Express'),
        'cb' => $this->t('Carte Bancaire (France only)'),
        'maestro' => $this->t('Maestro'),
      ],
      '#default_value' => $this->configuration['payment_product_list'],
      '#required' => TRUE,
    ];

    $form['3ds'] = [
      '#type' => 'radios',
      '#title' => $this->t('Should the 3-D Secure authentication be performed for payment transactions'),
      '#options' => [
        0 => $this->t('Bypass 3-D Secure authentication.'),
        1 => $this->t('3-D Secure authentication if available.'),
        2 => $this->t('3-D Secure authentication mandatory.'),
      ],
      '#default_value' => $this->configuration['3ds'],
      '#required' => TRUE,
    ];

    $form['language'] = [
      '#type' => 'select',
      '#title' => $this->t('Default language'),
      '#description' => $this->t("Language to be used by the off-site payment page or iframe if user's language is not supported by the gateway."),
      '#options' => HipayTPP::getSupportedLanguages(),
      '#default_value' => $this->configuration['language'],
    ];

    $form['css'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Payment page style sheet'),
      '#description' => $this->t('Path and filename of the custom style sheet for the hosted payment page (relative to Drupal webroot) or absolute URL.')
        . '<br />' . $this->t('When using relative path, make sure that you test payment process from a valid hostname, otherwise you will end up with a runtime error.')
        . '<br />' . $this->t('Also note that Hipay requires HTTPS protocol, and the URL generated will reflect this requirement - make sure that your server configuration supports it.'),
      '#default_value' => $this->configuration['css'],
    ];

    $form['sandbox'] = [
      '#type' => 'checkboxes',
      '#title' => $this->t('Testing'),
      '#options' => [
        'orderid_datetime' => $this->t('Add current datetime to transaction initialization "orderid" parameter (when testing from multiple environments)'),
      ],
      '#default_value' => $this->configuration['sandbox'],
    ];

    return $form;
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

    if (!$form_state->getErrors()) {
      $values = $form_state->getValue($form['#parents']);

      $this->configuration['template'] = $values['template'];
      $this->configuration['payment_product_list'] = $values['payment_product_list'];
      $this->configuration['3ds'] = $values['3ds'];
      $this->configuration['language'] = $values['language'];
      $this->configuration['css'] = $values['css'];
      $this->configuration['sandbox'] = $values['sandbox'];
    }
  }

  /**
   * {@inheritdoc}
   */
  public function initializeHostedPaymentPage(PaymentInterface $payment, array $extra) {
    /** @var \HiPay\Fullservice\Gateway\Request\Order\HostedPaymentPageRequest $hosted_payment_page_request */
    $this->request = new HostedPaymentPageRequest();

    // Set generic request data.
    $this->setRequestData($payment, $extra);

    // Allow other modules to alter the Hosted Payment Page request.
    $event = new HostedPaymentPageRequestEvent($this->request, $payment);
    $this->eventDispatcher->dispatch(HipayEvents::HOSTED_PAYMENT_PAGE_REQUEST, $event);

    // Execute the request and return its response.
    /** @var \HiPay\Fullservice\Gateway\Model\HostedPaymentPage $hosted_payment_page */
    $hosted_payment_page = $this->doHostedPaymentPageRequest();

    $state = !empty($hosted_payment_page->getForwardUrl()) ? 'initialized' : 'initialization_failed';
    $payment->setState($state);
    $payment->save();

    // Allow other modules to react to the Hosted Payment Page response.
    $event = new HostedPaymentPageResponseEvent($hosted_payment_page, $payment);
    $this->eventDispatcher->dispatch(HipayEvents::HOSTED_PAYMENT_PAGE_RESPONSE, $event);

    return $hosted_payment_page;
  }

  /**
   * {@inheritdoc}
   */
  public function requestNewOrder(PaymentInterface $payment, array $extra) {
    /** @var \Drupal\commerce_order\Entity\Order $order */
    $order = $payment->getOrder();

    /** @var \HiPay\Fullservice\Gateway\Request\Order\OrderRequest $order_request */
    $this->request = new OrderRequest();

    // Set generic request data.
    $this->setRequestData($payment, $extra);

    // Set request data specific to Hipay's New Order request.
    /** @var \Drupal\commerce_payment\Entity\PaymentMethod $payment_method */
    $payment_method = $order->get('payment_method')->entity;

    $this->request->eci = ECI::RECURRING_ECOMMERCE;
    $this->request->payment_product = $payment_method->card_type->value;
    $this->request->cardtoken = $payment_method->getRemoteId();

    // Allow other modules to alter the Order request.
    $event = new OrderRequestEvent($this->request, $payment);
    $this->eventDispatcher->dispatch(HipayEvents::ORDER_REQUEST, $event);

    // Execute the request and return its response.
    /** @var \HiPay\Fullservice\Gateway\Model\Transaction $transaction */
    $transaction = $this->doOrderRequest();

    $payment->setState('hipay_' . $transaction->getState());
    $payment->save();

    // Allow other modules to react to the Order response.
    $event = new OrderResponseEvent($transaction, $payment);
    $this->eventDispatcher->dispatch(HipayEvents::ORDER_RESPONSE, $event);

    return $transaction;
  }

  /**
   * Sets generic data to the API request object.
   *
   * @param \Drupal\commerce_payment\Entity\PaymentInterface $payment
   *   The payment.
   * @param array $extra
   *   Extra data needed for this request.
   */
  protected function setRequestData(PaymentInterface $payment, array $extra) {
    /** @var \Drupal\commerce_order\Entity\Order $order */
    $order = $payment->getOrder();

    // For each HPP initialization call we want to send a different value of
    // "orderid" parameter, to make sure that the "forwardUrl" we receive in
    // gateway response is unique. That's because with each init call we're
    // sending a different value of "custom_data" parameter (which contains
    // real Drupal order ID and payment transaction ID), and if the "forwardUrl"
    // wouldn't change, the gateway would return the value of "custom_data"
    // from the first init call for this order.  Therefore for first init call
    // we set "orderid" parameter to real order ID, and then for subsequent
    // init requests we add an auto-incrementing number at the end.
    $orderid = $order->id();
    $payment_storage = $this->entityTypeManager->getStorage('commerce_payment');
    $existing_transactions = $payment_storage->loadByProperties(['order_id' => $order->id()]);
    if (count($existing_transactions) > 1) {
      $orderid .= '-' . count($existing_transactions);
    }
    if (!empty($this->configuration['sandbox']['orderid_datetime'])) {
      $orderid .= '-' . date('YmdHis');
    }

    $this->request->orderid = $orderid;
    $this->request->operation = !empty($extra['capture']) ? 'sale' : 'authorization';
    $this->request->eci = ECI::SECURE_ECOMMERCE;
    $this->request->template = $this->configuration['template'];
    $this->request->authentication_indicator = !empty($this->configuration['3ds']) ? $this->configuration['3ds'] : 0;
    $this->request->payment_product_category_list = 'credit-card';
    $this->request->payment_product_list = implode(',', array_filter($this->configuration['payment_product_list']));

    $this->request->merchant_display_name = substr($order->getStore()->getName(), 0, 32);
    $this->request->description = substr(t('Order @order_number at @store', array(
      '@order_number' => $order->id(),
      '@store' => $order->getStore()->getName(),
    )), 0, 255);

    $total_price_amount = $this->rounder->round($order->getTotalPrice());
    $this->request->amount = $total_price_amount->getNumber();
    $this->request->currency = $order->getTotalPrice()->getCurrencyCode();

    $this->request->cid = $order->getCustomerId();
    $this->request->ipaddr = $order->getIpAddress();
    $this->request->language = $this->getRequestLanguage($payment);
    $this->request->email = $order->getEmail();

    // Use "custom_data" to send real Drupal order ID value, as the value sent
    // in main "orderid" parameter could be altered for subsequent payment init
    // requests for the same order (see above).
    $this->request->custom_data = Json::encode([
      'order_id' => $order->id(),
      'transaction_id' => $payment->id(),
    ]);

    $this->request->accept_url = $extra['return_url'];
    $this->request->decline_url = $extra['return_url'];
    $this->request->pending_url = $extra['return_url'];
    $this->request->exception_url = $extra['return_url'];
    $this->request->cancel_url = $extra['cancel_url'];

    // Send customer billing details.
    /** @var \Drupal\profile\Entity\Profile $billing_profile */
    if ($billing_profile = $order->getBillingProfile()) {
      /** @var \Drupal\address\Plugin\Field\FieldType\AddressItem $billing_address */
      $billing_address = $billing_profile->get('address')->first();

      $customer_billing_info = new CustomerBillingInfoRequest();
      $customer_billing_info->firstname = $billing_address->getGivenName();
      $customer_billing_info->lastname = $billing_address->getFamilyName();
      $customer_billing_info->recipientinfo = $billing_address->getAdditionalName();
      $customer_billing_info->streetaddress = $billing_address->getAddressLine1();
      $customer_billing_info->streetaddress2 = $billing_address->getAddressLine2();
      $customer_billing_info->city = $billing_address->getLocality();
      $customer_billing_info->zipcode = $billing_address->getPostalCode();
      $customer_billing_info->country = $billing_address->getCountryCode();
      // State could be sent only for USA and Canada.
      if ($customer_billing_info->country == 'US' || $customer_billing_info->country == 'CA') {
        $customer_billing_info->state = $billing_address->getAdministrativeArea();
      }

      $this->request->customerBillingInfo = $customer_billing_info;
    }

    // Send customer shipping details.
    /** @var \Drupal\profile\Entity\Profile $shipping_profile */
    if ($shipping_profile = $this->getShippingProfile($order)) {
      /** @var \Drupal\address\Plugin\Field\FieldType\AddressItem $shipping_address */
      $shipping_address = $shipping_profile->get('address')->first();

      $customer_shipping_info = new CustomerShippingInfoRequest();
      $customer_shipping_info->shipto_firstname = $shipping_address->getGivenName();
      $customer_shipping_info->shipto_lastname = $shipping_address->getFamilyName();
      $customer_shipping_info->shipto_recipientinfo = $shipping_address->getAdditionalName();
      $customer_shipping_info->shipto_streetaddress = $shipping_address->getAddressLine1();
      $customer_shipping_info->shipto_streetaddress2 = $shipping_address->getAddressLine2();
      $customer_shipping_info->shipto_city = $shipping_address->getLocality();
      $customer_shipping_info->shipto_zipcode = $shipping_address->getPostalCode();
      $customer_shipping_info->shipto_country = $shipping_address->getCountryCode();
      // State could be sent only for USA and Canada.
      if ($customer_shipping_info->shipto_country == 'US' || $customer_shipping_info->shipto_country == 'CA') {
        $customer_shipping_info->shipto_state = $shipping_address->getAdministrativeArea();
      }

      $this->request->customerShippingInfo = $customer_shipping_info;
    }

    // Collect the adjustments and calculate shipping and tax prices.
    $shipping_amount = new Price('0', $order->getTotalPrice()->getCurrencyCode());
    $tax_amount = new Price('0', $order->getTotalPrice()->getCurrencyCode());

    foreach ($order->collectAdjustments() as $adjustment) {
      // Tax & Shipping adjustments need to be handled separately.
      if ($adjustment->getType() == 'shipping') {
        $shipping_amount = $shipping_amount->add($adjustment->getAmount());
      }
      elseif ($adjustment->getType() == 'tax') {
        $tax_amount = $tax_amount->add($adjustment->getAmount());
      }
    }

    // Send the shipping amount.
    if (!$shipping_amount->isZero()) {
      $shipping_amount = $this->rounder->round($shipping_amount);
      $this->request->shipping = $shipping_amount->getNumber();
    }

    // Send the tax amount.
    if (!$tax_amount->isZero()) {
      $tax_amount = $this->rounder->round($tax_amount);
      $this->request->tax = $tax_amount->getNumber();
    }

    // Send URL to merchant custom style sheet.
    if (!empty($this->configuration['css'])) {
      $prefix = strpos($this->configuration['css'], 'http') === 0 ? '' : 'base:';
      $this->request->css = Url::fromUri($prefix . $this->configuration['css'], [
        'absolute' => TRUE,
        'https' => TRUE,
      ])->toString();
    }
  }

  /**
   * {@inheritdoc}
   */
  public function doHostedPaymentPageRequest() {
    // Log the request message if request logging is enabled.
    if (!empty($this->configuration['api_logging']['request'])) {
      \Drupal::logger('commerce_hipay_tpp')
        ->debug('Hosted Payment Page request: <pre>@request</pre>', [
          '@request' => var_export((array) $this->request, TRUE),
        ]);
    }

    /** @var \HiPay\Fullservice\Gateway\Client\GatewayClient $gateway_client */
    $gateway_client = $this->getGatewayClient($this->request->currency);

    /** @var \HiPay\Fullservice\Gateway\Model\HostedPaymentPage $hosted_payment_page */
    $hosted_payment_page = $gateway_client->requestHostedPaymentPage($this->request);

    // Log the request message if request logging is enabled.
    if (!empty($this->configuration['api_logging']['response'])) {
      \Drupal::logger('commerce_hipay_tpp')
        ->debug('Hosted Payment Page response: <pre>@response</pre>', [
          '@response' => var_export($hosted_payment_page->toArray(), TRUE),
        ]);
    }

    return $hosted_payment_page;
  }

  /**
   * {@inheritdoc}
   */
  public function doOrderRequest() {
    // Log the request message if request logging is enabled.
    if (!empty($this->configuration['api_logging']['request'])) {
      \Drupal::logger('commerce_hipay_tpp')
        ->debug('Order request: <pre>@request</pre>', [
          '@request' => var_export((array) $this->request, TRUE),
        ]);
    }

    /** @var \HiPay\Fullservice\Gateway\Client\GatewayClient $gateway_client */
    $gateway_client = $this->getGatewayClient($this->request->currency);

    /** @var \HiPay\Fullservice\Gateway\Model\Transaction $transaction */
    $transaction = $gateway_client->requestNewOrder($this->request);

    // Log the request message if request logging is enabled.
    if (!empty($this->configuration['api_logging']['response'])) {
      \Drupal::logger('commerce_hipay_tpp')
        ->debug('Order response: <pre>@response</pre>', [
          '@response' => var_export($transaction->toArray(), TRUE),
        ]);
    }

    return $transaction;
  }

  /**
   * {@inheritdoc}
   */
  public function onReturn(OrderInterface $order, Request $request) {
    parent::onReturn($order, $request);

    $response_data = $request->query->all();

    // Log the response message if request logging is enabled.
    if (!empty($this->configuration['api_logging']['response'])) {
      \Drupal::logger('commerce_hipay_tpp')
        ->debug('Hosted Payment Page return: <pre>@values</pre>', [
          '@values' => var_export($response_data, TRUE),
        ]);
    }

    if (empty($response_data['custom_data'])) {
      throw new InvalidResponseException('Unable to find "custom_data" value in the response.');
    }
    $custom_data = Json::decode($response_data['custom_data']);

    // Try to load the payment transaction.
    $payment_storage = $this->entityTypeManager->getStorage('commerce_payment');
    /** @var \Drupal\commerce_payment\Entity\PaymentInterface $payment */
    if (empty($custom_data['transaction_id']) || !($payment = $payment_storage->load($custom_data['transaction_id']))) {
      throw new InvalidResponseException('Unable to load payment transaction from response values.');
    }

    if (!$this->validateResponseSignature($response_data)) {
      throw new InvalidResponseException('Error validating Hipay response signature.');
    }

    // Hipay state should be one of: completed, forwarding, pending, declined
    // or error.
    $payment->setState('hipay_' . $response_data['state']);
    $payment->setRemoteId($response_data['reference']);
    $payment->setRemoteState($response_data['status']);
    $payment->save();

    /** @var \Drupal\commerce_payment\Entity\PaymentMethod $payment_method */
    $payment_method = $order->get('payment_method')->entity;
    $payment_method->card_type = strtolower($response_data['cardbrand']);
    // Only the last 4 numbers are safe to store.
    $payment_method->card_number = substr($response_data['cardpan'], -4);
    $payment_method->card_exp_month = substr($response_data['cardexpiry'], -2);
    $payment_method->card_exp_year = substr($response_data['cardexpiry'], 0, 4);
    $expires = \Drupal\commerce_payment\CreditCard::calculateExpirationTimestamp(substr($response_data['cardexpiry'], -2), substr($response_data['cardexpiry'], 0, 4));
    // Store the remote ID returned by the request.
    $payment_method
      ->setRemoteId($response_data['cardtoken'])
      ->setExpiresTime($expires)
      // Now that we have all credit card details and remote_id stored
      // in the payment method we can mark it as reusable, so that it is shown
      // in user's "Payment methods" listing and can be reused for future
      // orders.
      ->setReusable(TRUE)
      ->save();


    // Allow other modules to react to the Hosted Payment Page return data.
    $event = new HostedPaymentPageReturnEvent($response_data, $payment);
    $this->eventDispatcher->dispatch(HipayEvents::HOSTED_PAYMENT_PAGE_RETURN, $event);

    // Add order log entry.
    $log_storage = $this->entityTypeManager->getStorage('commerce_log');
    if ($response_data['state'] == TransactionState::ERROR) {
      $log_storage->generate($order, 'payment_submission_failed')->save();
      throw new SoftDeclineException('There was an error processing your payment with Hipay. Please try again or contact us if the problem persists.');
    }
    if ($response_data['state'] == TransactionState::DECLINED) {
      $log_storage->generate($order, 'payment_submission_failed')->save();
      throw new HardDeclineException('There was an error processing your payment with Hipay. Please try again or contact us if the problem persists.');
    }
    $log_storage->generate($order, 'payment_submitted')->save();
  }

  /**
   * {@inheritdoc}
   */
  public function onCancel(OrderInterface $order, Request $request) {
    parent::onCancel($order, $request);

    // Hipay Hosted Payment Page doesn't support payment cancellations,
    // so there's nothing to do here.
  }

  /**
   * {@inheritdoc}
   */
  public function onNotify(Request $request) {
    parent::onNotify($request);

    $notification_data = $request->request->all();

    // Log the response message if request logging is enabled.
    if (!empty($this->configuration['api_logging']['response'])) {
      \Drupal::logger('commerce_hipay_tpp')
        ->debug('Hosted Payment Page @message notification: <pre>@values</pre>', [
          '@message' => $notification_data['message'],
          '@values' => var_export($notification_data, TRUE),
        ]);
    }

    // Exit when we don't receive a payment state we recognize.
    if (empty($notification_data['state']) || !in_array($notification_data['state'], [TransactionState::COMPLETED, TransactionState::FORWARDING, TransactionState::PENDING, TransactionState::DECLINED, TransactionState::ERROR])) {
      throw new InvalidRequestException('Invalid payment status');
    }

    if (empty($notification_data['custom_data'])) {
      throw new InvalidRequestException('Unable to find "custom_data" value in the notification.');
    }

    // Try to load the payment transaction.
    $payment_storage = $this->entityTypeManager->getStorage('commerce_payment');
    /** @var \Drupal\commerce_payment\Entity\PaymentInterface $payment */
    if (!empty($notification_data['operation']['id'])) {
      $payment = $payment_storage->load($notification_data['operation']['id']);
      $amount = new Price($notification_data['operation']['amount'], $notification_data['operation']['currency']);
    }
    elseif (!empty($notification_data['custom_data']['transaction_id'])) {
      $payment = $payment_storage->load($notification_data['custom_data']['transaction_id']);
      $amount = new Price($notification_data['order']['amount'], $notification_data['order']['currency']);
    }
    if (empty($payment)) {
      throw new InvalidRequestException('Unable to load payment transaction from notification values.');
    }

    // Validate notification signature.
    if (!$this->validateResponseSignature($notification_data)) {
      throw new InvalidRequestException('Error validating Hipay notification signature.');
    }

    // Try to load the order.
    $order_storage = $this->entityTypeManager->getStorage('commerce_order');
    if (empty($notification_data['custom_data']['order_id']) || !($order = $order_storage->load($notification_data['custom_data']['order_id']))) {
      throw new InvalidRequestException('Unable to load order from notification values.');
    }

    $payment->setRemoteId($notification_data['transaction_reference']);
    $payment->setRemoteState($notification_data['status']);
    $payment->setAmount($amount);

    // If the transaction has been completed on Hipay side, update its state
    // to one of Drupal Commerce's native final states.
    if ($notification_data['state'] == TransactionState::COMPLETED) {
      switch ($notification_data['status']) {
        case TransactionStatus::AUTHORIZED:
          $payment->setState('authorization');
          $payment->setAuthorizedTime(time());
          break;
        case TransactionStatus::CAPTURED:
          $payment->setState('completed');
          $payment->setCompletedTime(time());
          break;
        case TransactionStatus::REFUNDED:
          $payment->setState('refunded');
          $payment->setCompletedTime(time());
          break;
        case TransactionStatus::CANCELLED:
          $payment->setState('authorization_voided');
          break;
      }
    }
    // Otherwise use a temporary Hipay-specific state, as we should receive
    // another "completed" Hipay notification in the future.
    else {
      $payment->setState('hipay_' . $notification_data['state']);
    }

    $payment->save();

    // Add order log entry.
    $log_storage = $this->entityTypeManager->getStorage('commerce_log');
    $log_storage->generate($order, 'notification_received', ['message' => $notification_data['message']])->save();

    // Allow other modules to react to the Hosted Payment Page notification.
    $event = new HostedPaymentPageNotificationEvent($notification_data, $payment);
    $this->eventDispatcher->dispatch(HipayEvents::HOSTED_PAYMENT_PAGE_NOTIFICATION, $event);
  }

  /**
   * {@inheritdoc}
   */
  public function voidPayment(PaymentInterface $payment) {
    $this->assertPaymentState($payment, ['authorization']);

    /** @var \HiPay\Fullservice\Gateway\Request\Maintenance\MaintenanceRequest $maintenance_request */
    $maintenance_request = new MaintenanceRequest();

    $maintenance_request->operation = 'cancel';

    // Log the request message if request logging is enabled.
    if (!empty($this->configuration['api_logging']['request'])) {
      \Drupal::logger('commerce_hipay_tpp')
        ->debug('Maintenance cancel request: <pre>@request</pre>', [
          '@request' => var_export((array) $maintenance_request, TRUE),
        ]);
    }

    try {
      /** @var \HiPay\Fullservice\Gateway\Client\GatewayClient $gateway_client */
      $gateway_client = $this->getGatewayClient($payment->getAmount()->getCurrencyCode());

      /** @var \HiPay\Fullservice\Gateway\Model\Operation $maintenance_operation */
      $maintenance_operation = $gateway_client->requestMaintenanceOperation($maintenance_request->operation, $payment->getRemoteId(), NULL, NULL, $maintenance_request);
    }
    catch (\Exception $e) {
      // Log the exception message if response logging is enabled.
      if (!empty($this->configuration['api_logging']['response'])) {
        \Drupal::logger('commerce_hipay_tpp')
          ->debug('Maintenance cancel exception: @code: @message', [
            '@code' => $e->getCode(),
            '@message' => $e->getMessage(),
          ]);
      }

      throw new PaymentGatewayException($e->getMessage(), $e->getCode());
    }

    // Log the response message if response logging is enabled.
    if (!empty($this->configuration['api_logging']['response'])) {
      \Drupal::logger('commerce_hipay_tpp')
        ->debug('Maintenance cancel response: <pre>@response</pre>', [
          '@response' => var_export($maintenance_operation->toArray(), TRUE),
        ]);
    }

    $payment->setState('hipay_authorization_voided');
    $remote_status = $maintenance_operation->getStatus();
    $payment->setRemoteState($remote_status);
    $payment->save();

    if ($remote_status == TransactionStatus::CANCELLED) {
      // Add order log entry.
      $log_storage = $this->entityTypeManager->getStorage('commerce_log');
      $log_storage->generate($payment->getOrder(), 'authorization_voided')
        ->save();
    }

    // Allow other modules to react to the Maintenance Operation response data.
    $event = new MaintenanceOperationResponseEvent($maintenance_operation->toArray(), $payment);
    $this->eventDispatcher->dispatch(HipayEvents::MAINTENANCE_OPERATION_RESPONSE, $event);
  }

  /**
   * {@inheritdoc}
   */
  public function capturePayment(PaymentInterface $payment, Price $amount = NULL) {
    $this->assertPaymentState($payment, ['authorization']);

    // If not specified, capture the entire amount.
    $amount = $amount ?: $payment->getAmount();
    $amount = $this->rounder->round($amount);

    // Create new capture payment transaction to allow for multiple captures.
    // We don't use standard DC2's one-payment-transaction-per-order approach
    // here, as it would be impossible to do partial captures then. See
    // https://www.drupal.org/node/2847378#comment-12115052 for more info.
    // Also we need different payment transaction IDs between authorization,
    // capture and refund operations so that we can can send them to Hipay
    // in each request, so that later we can recognize incoming notifications.
    $values = [
      'payment_gateway' => $payment->getPaymentGatewayId(),
      'payment_method' => $payment->getPaymentMethodId(),
      'order_id' => $payment->getOrderId(),
      'remote_id' => $payment->getRemoteId(),
      'state' => 'new',
      'amount' => $amount,
    ];
    $payment_storage = $this->entityTypeManager->getStorage('commerce_payment');
    /** @var PaymentInterface $capture_payment */
    $capture_payment = $payment_storage->create($values);
    // Save the new capture transaction to obtain its ID,
    // which we need to send to Hipay in the capture request.
    $capture_payment->save();

    /** @var \HiPay\Fullservice\Gateway\Request\Maintenance\MaintenanceRequest $maintenance_request */
    $maintenance_request = new MaintenanceRequest();

    $maintenance_request->operation = 'capture';
    $maintenance_request->operation_id = $capture_payment->id();
    $maintenance_request->amount = $amount->getNumber();

    // Log the request message if request logging is enabled.
    if (!empty($this->configuration['api_logging']['request'])) {
      \Drupal::logger('commerce_hipay_tpp')
        ->debug('Maintenance capture request: <pre>@request</pre>', [
          '@request' => var_export((array) $maintenance_request, TRUE),
        ]);
    }

    try {
      /** @var \HiPay\Fullservice\Gateway\Client\GatewayClient $gateway_client */
      $gateway_client = $this->getGatewayClient($amount->getCurrencyCode());

      /** @var \HiPay\Fullservice\Gateway\Model\Operation $maintenance_operation */
      $maintenance_operation = $gateway_client->requestMaintenanceOperation($maintenance_request->operation, $payment->getRemoteId(), $maintenance_request->amount, $maintenance_request->operation_id, $maintenance_request);
    }
    catch (\Exception $e) {
      $capture_payment->delete();

      // Log the exception message if response logging is enabled.
      if (!empty($this->configuration['api_logging']['response'])) {
        \Drupal::logger('commerce_hipay_tpp')
          ->debug('Maintenance capture exception: @code: @message', [
            '@code' => $e->getCode(),
            '@message' => $e->getMessage(),
          ]);
      }

      throw new PaymentGatewayException($e->getMessage(), $e->getCode());
    }

    // Log the response message if response logging is enabled.
    if (!empty($this->configuration['api_logging']['response'])) {
      \Drupal::logger('commerce_hipay_tpp')
        ->debug('Maintenance capture response: <pre>@response</pre>', [
          '@response' => var_export($maintenance_operation->toArray(), TRUE),
        ]);
    }

    // Update new capture payment transaction.
    // Use temporary payment state, which will be updated to final "completed"
    // once we receive the notification from Hipay.
    $capture_payment->setState('capture_requested');
    $remote_status = $maintenance_operation->getStatus();
    $capture_payment->setRemoteState($remote_status);
    $capture_payment->save();

    if (
      $remote_status == TransactionStatus::CAPTURE_REQUESTED
      || $remote_status == TransactionStatus::CAPTURED
      || $remote_status == TransactionStatus::PARTIALLY_CAPTURED
    ) {
      // Update original authorization payment transaction.
      $authorized_amount = $payment->getAmount();
      $payment->setAmount($authorized_amount->subtract($amount));

      // If full amount of original authorization was captured, void it.
      if ($payment->getAmount()->isZero()) {
        $payment->setState('authorization_voided');
      }
      $payment->save();

      // Add order log entry.
      $log_storage = $this->entityTypeManager->getStorage('commerce_log');
      $log_storage->generate($payment->getOrder(), 'authorization_captured', [
        'amount' => $amount->getNumber(),
        'currency_code' => $amount->getCurrencyCode(),
      ])->save();
    }

    // Allow other modules to react to the Maintenance Operation response data.
    $event = new MaintenanceOperationResponseEvent($maintenance_operation->toArray(), $capture_payment);
    $this->eventDispatcher->dispatch(HipayEvents::MAINTENANCE_OPERATION_RESPONSE, $event);
  }

  /**
   * {@inheritdoc}
   */
  public function refundPayment(PaymentInterface $capture_payment, Price $amount = NULL) {
    $this->assertPaymentState($capture_payment, ['completed', 'partially_refunded']);

    // If not specified, refund the entire captured amount.
    $amount = $amount ?: $capture_payment->getAmount();
    $amount = $this->rounder->round($amount);

    // Create new refund payment transaction following up on creating separate
    // transactions for capture operations (see capturePayment() - we do not use
    // standard DC2's one-payment-transaction-per-order approach there - see
    // https://www.drupal.org/node/2847378#comment-12115052 for more info).
    // Also we need different payment transaction IDs between authorization,
    // capture and refund operations so that we can can send them to Hipay
    // in each request, so that later we can recognize incoming notifications.
    $values = [
      'payment_gateway' => $capture_payment->getPaymentGatewayId(),
      'payment_method' => $capture_payment->getPaymentMethodId(),
      'order_id' => $capture_payment->getOrderId(),
      'remote_id' => $capture_payment->getRemoteId(),
      'state' => 'new',
      'amount' => $amount,
    ];
    $payment_storage = $this->entityTypeManager->getStorage('commerce_payment');
    /** @var PaymentInterface $refund_payment */
    $refund_payment = $payment_storage->create($values);
    // Save the new capture transaction to obtain its ID,
    // which we need to send to Hipay in the capture request.
    $refund_payment->save();

    /** @var \HiPay\Fullservice\Gateway\Request\Maintenance\MaintenanceRequest $maintenance_request */
    $maintenance_request = new MaintenanceRequest();

    $maintenance_request->operation = 'refund';
    $maintenance_request->operation_id = $refund_payment->id();
    $maintenance_request->amount = $amount->getNumber();

    // Log the request message if request logging is enabled.
    if (!empty($this->configuration['api_logging']['request'])) {
      \Drupal::logger('commerce_hipay_tpp')
        ->debug('Maintenance refund request: <pre>@request</pre>', [
          '@request' => var_export((array) $maintenance_request, TRUE),
        ]);
    }

    try {
      /** @var \HiPay\Fullservice\Gateway\Client\GatewayClient $gateway_client */
      $gateway_client = $this->getGatewayClient($amount->getCurrencyCode());

      /** @var \HiPay\Fullservice\Gateway\Model\Operation $maintenance_operation */
      $maintenance_operation = $gateway_client->requestMaintenanceOperation($maintenance_request->operation, $capture_payment->getRemoteId(), $maintenance_request->amount, $maintenance_request->operation_id, $maintenance_request);
    }
    catch (\Exception $e) {
      $refund_payment->delete();

      // Log the exception message if response logging is enabled.
      if (!empty($this->configuration['api_logging']['response'])) {
        \Drupal::logger('commerce_hipay_tpp')
          ->debug('Maintenance refund exception: @code: @message', [
            '@code' => $e->getCode(),
            '@message' => $e->getMessage(),
          ]);
      }

      throw new PaymentGatewayException($e->getMessage(), $e->getCode());
    }

    // Log the response message if response logging is enabled.
    if (!empty($this->configuration['api_logging']['response'])) {
      \Drupal::logger('commerce_hipay_tpp')
        ->debug('Maintenance refund response: <pre>@response</pre>', [
          '@response' => var_export($maintenance_operation->toArray(), TRUE),
        ]);
    }

    // Update new refund payment transaction.
    // Use temporary payment state, which will be updated to final "refunded"
    // once we receive the notification from Hipay.
    $refund_payment->setState('refund_requested');
    $remote_status = $maintenance_operation->getStatus();
    $refund_payment->setRemoteState($remote_status);
    $refund_payment->save();

    if (
      $remote_status == TransactionStatus::REFUND_REQUESTED
      || $remote_status == TransactionStatus::REFUNDED
      || $remote_status == TransactionStatus::PARTIALLY_REFUNDED
    ) {
      // Update original capture payment transaction.
      $new_refunded_amount = $capture_payment->getRefundedAmount()->add($amount);
      $capture_payment->setRefundedAmount($new_refunded_amount);
      // Once total captured amount was refunded, update capture payment state
      // to "captured" so that the "Refund" button is not displayed anymore.
      if ($capture_payment->getAmount()->equals($capture_payment->getRefundedAmount())) {
        $capture_payment->setState('captured');
      }
      $capture_payment->save();

      // Add order log entry.
      $log_storage = $this->entityTypeManager->getStorage('commerce_log');
      $log_storage->generate($capture_payment->getOrder(), 'payment_refunded', [
        'amount' => $amount->getNumber(),
        'currency_code' => $amount->getCurrencyCode(),
      ])->save();
    }

    // Allow other modules to react to the Maintenance Operation response data.
    $event = new MaintenanceOperationResponseEvent($maintenance_operation->toArray(), $refund_payment);
    $this->eventDispatcher->dispatch(HipayEvents::MAINTENANCE_OPERATION_RESPONSE, $event);
  }

  /**
   * Gets gateway client for Hipay TPP API request.
   *
   * @param string $currency
   *   A currency code to return the gateway client for.
   *
   * @return \HiPay\Fullservice\Gateway\Client\GatewayClient
   */
  protected function getGatewayClient($currency) {
    $api_username = $this->configuration['accounts'][$currency]['api_username'];
    $api_password = $this->configuration['accounts'][$currency]['api_password'];
    $api_env = $this->configuration['mode'] == 'live' ? Configuration::API_ENV_PRODUCTION : Configuration::API_ENV_STAGE;

    /** @var \HiPay\Fullservice\HTTP\Configuration\Configuration $config */
    $config = new Configuration($api_username, $api_password, $api_env);

    /** @var \HiPay\Fullservice\HTTP\SimpleHTTPClient $client_provider */
    $client_provider = new SimpleHTTPClient($config);

    /** @var \HiPay\Fullservice\Gateway\Client\GatewayClient $gateway_client */
    return new GatewayClient($client_provider);
  }

  /**
   * Returns the language for Hipay API request for the order.
   *
   * @param \Drupal\commerce_payment\Entity\PaymentInterface $payment
   *
   * @return string
   */
  protected function getRequestLanguage(PaymentInterface $payment) {
    /** @var \Drupal\Core\Language\Language $language */
    $language = $payment->getOrder()->language();
    if ($language->getId() == LanguageInterface::LANGCODE_NOT_SPECIFIED) {
      $language = \Drupal::languageManager()->getCurrentLanguage();
    }

    $locale_elements = explode('-', $language->getId(), 2);
    if (isset($locale_elements[1])) {
      $request_language = $locale_elements[0] . '_' . Unicode::strtoupper($locale_elements[1]);
    }
    else {
      $request_language = $locale_elements[0] . '_' . Unicode::strtoupper($locale_elements[0]);
    }

    if (!in_array($request_language, array_keys(HipayTPP::getSupportedLanguages()))) {
      $payment_gateway_configuration = $payment->getPaymentGateway()->getPlugin()->getConfiguration();
      $request_language = $payment_gateway_configuration['language'];
    }

    return $request_language;
  }

  /**
   * Returns the customer shipping profile for the order.
   *
   * @param \Drupal\commerce_order\Entity\OrderInterface $order
   *
   * @return mixed
   */
  protected function getShippingProfile(OrderInterface $order) {
    // Shipping profile might be available only if shipping module is enabled.
    if (\Drupal::moduleHandler()->moduleExists('commerce_shipping')) {
      // Check if the order references shipments.
      if ($order->hasField('shipments') && !$order->get('shipments')
          ->isEmpty()
      ) {
        // Gather the shipping profiles and only send shipping information if
        // there's only one shipping profile referenced by the shipments.
        $shipping_profiles = [];

        // Loop over the shipments to collect shipping profiles.
        foreach ($order->get('shipments')->referencedEntities() as $shipment) {
          if ($shipment->get('shipping_profile')->isEmpty()) {
            continue;
          }
          $shipping_profile = $shipment->getShippingProfile();
          $shipping_profiles[$shipping_profile->id()] = $shipping_profile;
        }

        // Return the shipping profile if we found only one.
        if ($shipping_profiles && count($shipping_profiles) === 1) {
          return reset($shipping_profiles);
        }
      }
    }
  }

  /**
   * Validates API response/notification signature against its content.
   *
   * @param array $response_data
   *
   * @return bool
   */
  public function validateResponseSignature($response_data) {
    $configuration = $this->getConfiguration();

    if (empty($response_data['currency']) || !in_array($response_data['currency'], array_keys($configuration['accounts']))) {
      throw new PaymentGatewayException('Error getting currency code from Hipay return values for signature validation.');
    }

    // Validate the signature either if it was provided by the gateway
    // or when the secret passphrase is configured locally.
    if (
      !empty($configuration['accounts'][$response_data['currency']]['secret_passphrase'])
      // Offsite redirect return will send the signature in the 'hash' feedback
      // parameter, while asynchronous server-to-server notification will use
      // the 'HTTP_X_ALLOPASS_SIGNATURE' header.
      && (!empty($_SERVER['HTTP_X_ALLOPASS_SIGNATURE']) || !empty($response_data['hash']))
    ) {
      return $this->validateSignature($response_data);
    }

    // If no signature was provided by the gateway, or the passphrase is not
    // configured locally, pretend everything is fine.
    return TRUE;
  }

  /**
   * Validates API response/notification signature against its content.
   *
   * @param array $response_data
   *
   * @return bool
   */
  public function validateSignature($response_data) {
    $configuration = $this->getConfiguration();

    $string_to_hash = $hipay_signature = '';

    // If it is a redirection.
    if (isset($response_data['hash'])) {
      $hipay_signature = $response_data['hash'];
      unset($response_data['hash']);
      ksort($response_data);
      foreach ($response_data as $name => $value) {
        if (strlen($value) > 0) {
          $string_to_hash .= $name . $value . $configuration['accounts'][$response_data['currency']]['secret_passphrase'];
        }
      }
    }
    // If it is a server-to-server notification.
    elseif (isset($_SERVER['HTTP_X_ALLOPASS_SIGNATURE'])) {
      $hipay_signature = $_SERVER['HTTP_X_ALLOPASS_SIGNATURE'];
      $string_to_hash = file_get_contents("php://input") . $configuration['accounts'][$response_data['currency']]['secret_passphrase'];
    }

    $local_signature = sha1($string_to_hash);

    return ($local_signature === $hipay_signature) ? TRUE : FALSE;
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
    $payment_method->delete();
  }

}

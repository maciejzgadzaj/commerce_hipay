<?php

namespace Drupal\commerce_hipay_tpp\Plugin\Commerce\PaymentGateway;

use Drupal\commerce_hipay_tpp\Event\HostedPaymentPageNotificationEvent;
use Drupal\commerce_hipay_tpp\Event\HostedPaymentPageResponseEvent;
use Drupal\commerce_hipay_tpp\Event\HostedPaymentPageReturnEvent;
use Drupal\commerce_hipay_tpp\Hipay\HipayTPP;
use Drupal\commerce_payment\Exception\SoftDeclineException;
use Drupal\Component\Serialization\Json;
use Drupal\Component\Utility\UrlHelper;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Drupal\commerce_payment\Entity\PaymentInterface;
//use Drupal\commerce_payment\Entity\PaymentMethodInterface;
//use Drupal\commerce_payment\Exception\DeclineException;
use Drupal\commerce_payment\Exception\InvalidResponseException;
use Drupal\commerce_payment\Exception\HardDeclineException;
use Drupal\commerce_payment\Exception\InvalidRequestException;
use Drupal\commerce_payment\Exception\PaymentGatewayException;
use Drupal\commerce_payment\PaymentMethodTypeManager;
use Drupal\commerce_payment\PaymentTypeManager;
use Drupal\commerce_payment\Plugin\Commerce\PaymentGateway\OffsitePaymentGatewayBase;
use Drupal\commerce_price\Price;
use HiPay\Fullservice\Enum\Transaction\ECI;
use HiPay\Fullservice\Enum\Transaction\Template;
use HiPay\Fullservice\Gateway\Request\Info\CustomerShippingInfoRequest;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\commerce_order\Entity\OrderInterface;
use Symfony\Component\HttpFoundation\Request;
use HiPay\Fullservice\HTTP\Configuration\Configuration;
use HiPay\Fullservice\HTTP\SimpleHTTPClient;
use HiPay\Fullservice\Gateway\Client\GatewayClient;
use HiPay\Fullservice\Gateway\Request\Order\HostedPaymentPageRequest;
use Drupal\Core\Language\LanguageInterface;
use Drupal\Component\Utility\Unicode;
use Drupal\commerce_price\RounderInterface;
use Drupal\Component\Datetime\TimeInterface;
use Symfony\Component\EventDispatcher\EventDispatcherInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\commerce_hipay_tpp\Event\HipayEvents;
use Drupal\commerce_hipay_tpp\Event\HostedPaymentPageRequestEvent;
use HiPay\Fullservice\Enum\Transaction\TransactionState;
use HiPay\Fullservice\Enum\Transaction\TransactionStatus;
use HiPay\Fullservice\Gateway\Request\Info\CustomerBillingInfoRequest;

/**
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
 *   }
 * )
 */
class CreditCard extends OffsitePaymentGatewayBase implements CreditCardInterface {

  use ConfigurationTrait;

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
      'payment_product_list' => [],
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
   *
   * @see https://developer.hipay.com/doc-api/enterprise/gateway/#!/payments/generateHostedPaymentPage
   */
  public function initializeHostedPaymentPage(PaymentInterface $payment, array $extra) {
    // Save the payment so that we can get its ID and pass it to Hipay.
    $payment->save();

    /** @var \Drupal\commerce_order\Entity\Order $order */
    $order = $payment->getOrder();

    /** @var \HiPay\Fullservice\Gateway\Request\Order\HostedPaymentPageRequest $hosted_payment_page_request */
    $hosted_payment_page_request = new HostedPaymentPageRequest();

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

    $hosted_payment_page_request->orderid = $orderid;
    $hosted_payment_page_request->operation = !empty($extra['capture']) ? 'sale' : 'authorization';
    $hosted_payment_page_request->eci = ECI::SECURE_ECOMMERCE;
    $hosted_payment_page_request->template = $this->configuration['template'];
    $hosted_payment_page_request->authentication_indicator = !empty($this->configuration['3ds']) ? $this->configuration['3ds'] : 0;
    $hosted_payment_page_request->payment_product_category_list = 'credit-card';
    $hosted_payment_page_request->payment_product_list = implode(',', array_filter($this->configuration['payment_product_list']));

    $hosted_payment_page_request->merchant_display_name = substr($order->getStore()->getName(), 0, 32);
    $hosted_payment_page_request->description = substr(t('Order @order_number at @store', array(
      '@order_number' => $order->id(),
      '@store' => $order->getStore()->getName(),
    )), 0, 255);

    $total_price_amount = $this->rounder->round($order->getTotalPrice());
    $hosted_payment_page_request->amount = $total_price_amount->getNumber();
    $hosted_payment_page_request->currency = $order->getTotalPrice()->getCurrencyCode();

    $hosted_payment_page_request->cid = $order->getCustomerId();
    $hosted_payment_page_request->ipaddr = $order->getIpAddress();
    $hosted_payment_page_request->language = $this->getRequestLanguage($payment);
    $hosted_payment_page_request->email = $order->getEmail();

    // Use "custom_data" to send real Drupal order ID value, as the value sent
    // in main "orderid" parameter could be altered for subsequent payment init
    // requests for the same order (see above).
    $hosted_payment_page_request->custom_data = Json::encode([
      'order_id' => $order->id(),
      'transaction_id' => $payment->id(),
    ]);

    $hosted_payment_page_request->accept_url = $extra['return_url'];
    $hosted_payment_page_request->decline_url = $extra['return_url'];
    $hosted_payment_page_request->pending_url = $extra['return_url'];
    $hosted_payment_page_request->exception_url = $extra['return_url'];
    $hosted_payment_page_request->cancel_url = $extra['cancel_url'];

    // Send customer billing details.
    /** @var \Drupal\profile\Entity\Profile $billing_profile */
    if ($billing_profile = $order->getBillingProfile()) {
      /** @var \Drupal\address\Plugin\Field\FieldType\AddressItem $billing_address */
      $billing_address = $billing_profile->address->first();

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

      $hosted_payment_page_request->customerBillingInfo = $customer_billing_info;
    }

    // Send customer shipping details.
    /** @var \Drupal\profile\Entity\Profile $shipping_profile */
    if ($shipping_profile = $this->getShippingProfile($order)) {
      /** @var \Drupal\address\Plugin\Field\FieldType\AddressItem $billing_address */
      $shipping_address = $shipping_profile->address->first();

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

      $hosted_payment_page_request->customerShippingInfo = $customer_shipping_info;
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
      $hosted_payment_page_request->shipping = $shipping_amount->getNumber();
    }

    // Send the tax amount.
    if (!$tax_amount->isZero()) {
      $tax_amount = $this->rounder->round($tax_amount);
      $hosted_payment_page_request->tax = $tax_amount->getNumber();
    }

    // Send URL to merchant custom style sheet.
    if (!empty($this->configuration['css'])) {
      $prefix = strpos($this->configuration['css'], 'http') === 0 ? '' : 'base:';
      $hosted_payment_page_request->css = Url::fromUri($prefix . $this->configuration['css'], [
        'absolute' => TRUE,
        'https' => TRUE,
      ])->toString();
    }

    // Allow other modules to alter the Hosted Payment Page request.
    $event = new HostedPaymentPageRequestEvent($hosted_payment_page_request, $payment);
    $this->eventDispatcher->dispatch(HipayEvents::HOSTED_PAYMENT_PAGE_REQUEST, $event);

    // Execute the request and return its response.
    /** @var \HiPay\Fullservice\Gateway\Model\HostedPaymentPage $hosted_payment_page */
    $hosted_payment_page = $this->doHostedPaymentPageRequest($hosted_payment_page_request);

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
  public function doHostedPaymentPageRequest(HostedPaymentPageRequest $hosted_payment_page_request) {
    // Log the request message if request logging is enabled.
    if (!empty($this->configuration['api_logging']['request'])) {
      \Drupal::logger('commerce_hipay_tpp')
        ->debug('Hosted Payment Page request: <pre>@request</pre>', [
          '@request' => var_export((array) $hosted_payment_page_request, TRUE),
        ]);
    }

    $api_username = $this->configuration['accounts'][$hosted_payment_page_request->currency]['api_username'];
    $api_password = $this->configuration['accounts'][$hosted_payment_page_request->currency]['api_password'];
    $api_env = $this->configuration['mode'] == 'live' ? Configuration::API_ENV_PRODUCTION : Configuration::API_ENV_STAGE;

    /** @var \HiPay\Fullservice\HTTP\Configuration\Configuration $config */
    $config = new Configuration($api_username, $api_password, $api_env);

    /** @var \HiPay\Fullservice\HTTP\SimpleHTTPClient $client_provider */
    $client_provider = new SimpleHTTPClient($config);

    /** @var \HiPay\Fullservice\Gateway\Client\GatewayClient $gateway_client */
    $gateway_client = new GatewayClient($client_provider);

    /** @var \HiPay\Fullservice\Gateway\Model\HostedPaymentPage $hosted_payment_page */
    $hosted_payment_page = $gateway_client->requestHostedPaymentPage($hosted_payment_page_request);

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

    // Allow other modules to react to the Hosted Payment Page return data.
    $event = new HostedPaymentPageReturnEvent($response_data, $payment);
    $this->eventDispatcher->dispatch(HipayEvents::HOSTED_PAYMENT_PAGE_RETURN, $event);

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
    if (empty($notification_data['custom_data']['transaction_id']) || !($payment = $payment_storage->load($notification_data['custom_data']['transaction_id']))) {
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

    $payment->setRemoteState($notification_data['status']);

    // @TODO:
    // We want to update the main transaction status only for the very first call
    // (regardless whether it was authorization or capture), which means only when
    // the main transaction still has its 'pending' status.
    if ($notification_data['state'] == TransactionState::COMPLETED) {
      switch ($notification_data['status']) {
        case TransactionStatus::AUTHORIZED:
          $amount = new Price($notification_data['authorized_amount'], $notification_data['currency']);
          $payment->setAmount($amount);
          $payment->setState('authorization');
          $payment->setAuthorizedTime(time());
          break;
        case TransactionStatus::CAPTURED:
          $amount = new Price($notification_data['captured_amount'], $notification_data['currency']);
          $payment->setAmount($amount);
          $payment->setState('completed');
          $payment->setCompletedTime(time());
          break;
      }
    }
    else {
      $payment->setState('hipay_' . $notification_data['state']);
    }

    $payment->save();

    $log_storage = $this->entityTypeManager->getStorage('commerce_log');
    $log_storage->generate($order, 'notification_received', ['message' => $notification_data['message']])->save();

    // Allow other modules to react to the Hosted Payment Page notification.
    $event = new HostedPaymentPageNotificationEvent($notification_data, $payment);
    $this->eventDispatcher->dispatch(HipayEvents::HOSTED_PAYMENT_PAGE_NOTIFICATION, $event);
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

}

<?php

namespace Drupal\commerce_hipay_tpp\Event;

/**
 * Defines events for the Commerce Hipay TPP module.
 */
final class HipayEvents {

  /**
   * Name of the event fired when performing the Hosted Payment Page requests.
   *
   * @Event
   */
  const HOSTED_PAYMENT_PAGE_REQUEST = 'commerce_hipay_tpp.hosted_payment_page_request';

  /**
   * Name of the event fired when receiving the Hosted Payment Page response.
   *
   * @Event
   */
  const HOSTED_PAYMENT_PAGE_RESPONSE = 'commerce_hipay_tpp.hosted_payment_page_response';

  /**
   * Name of the event fired when returning from the Hosted Payment Page redirect.
   *
   * @Event
   */
  const HOSTED_PAYMENT_PAGE_RETURN = 'commerce_hipay_tpp.hosted_payment_page_return';

  /**
   * Name of the event fired when receiving the Hosted Payment Page notification.
   *
   * @Event
   */
  const HOSTED_PAYMENT_PAGE_NOTIFICATION = 'commerce_hipay_tpp.hosted_payment_page_notification';

  /**
   * Name of the event fired when receiving the Maintenance Operation response.
   *
   * @Event
   */
  const MAINTENANCE_OPERATION_RESPONSE = 'commerce_hipay_tpp.maintenance_operation_response';

}

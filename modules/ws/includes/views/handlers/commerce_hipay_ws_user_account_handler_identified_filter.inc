<?php

/**
 * Filter by order status.
 */
class commerce_hipay_ws_user_account_handler_identified_filter extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Identified');
      $this->value_options = drupal_map_assoc(array(
        COMMERCE_HIPAY_WS_ACCOUNT_IDENTIFIED,
        COMMERCE_HIPAY_WS_ACCOUNT_NOT_IDENTIFIED,
      ));
    }
  }
}

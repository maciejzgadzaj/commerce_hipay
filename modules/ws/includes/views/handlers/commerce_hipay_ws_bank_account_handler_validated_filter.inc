<?php

/**
 * Filter by order status.
 */
class commerce_hipay_ws_bank_account_handler_validated_filter extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Validation status');
      $this->value_options = drupal_map_assoc(array(
        COMMERCE_HIPAY_WS_ACCOUNT_BANK_INFO_VALIDATED,
        COMMERCE_HIPAY_WS_ACCOUNT_BANK_INFO_VALIDATION_IN_PROGRESS,
        COMMERCE_HIPAY_WS_ACCOUNT_NO_BANK_INFO,
      ));
    }
  }
}

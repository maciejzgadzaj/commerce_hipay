<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying links to entities
 * as fields.
 */

class commerce_hipay_ws_handler_link_transfer_id extends commerce_hipay_ws_handler_link_account_id {

  function render($values) {
    $value = $this->sanitize_value($this->get_value($values));

    if (!empty($this->options['link_to_entity'])) {
      return l($value, 'admin/commerce/hipay-wallet/transfers/' . $value);
    }
    else {
      return $value;
    }
  }
}

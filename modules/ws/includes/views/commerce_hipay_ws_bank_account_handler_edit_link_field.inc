<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields.
 */

class commerce_hipay_ws_bank_account_handler_edit_link_field extends commerce_hipay_ws_bank_account_handler_link_field {
  function render($values) {
    $type = $values->{$this->aliases['type']};

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $bank_account_id = $values->{$this->aliases['bank_account_id']};

    return l($text, 'admin/commerce/hipay-wallet/bank-accounts/' . $bank_account_id . '/edit');
  }
}

<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying delete links
 * as fields.
 */

class commerce_hipay_ws_user_account_handler_delete_link_field extends commerce_hipay_ws_user_account_handler_link_field {
  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $user_account_id = $values->{$this->aliases['user_account_id']};

    return l($text, 'admin/commerce/hipay-wallet/user-accounts/' . $user_account_id . '/delete');
  }
}

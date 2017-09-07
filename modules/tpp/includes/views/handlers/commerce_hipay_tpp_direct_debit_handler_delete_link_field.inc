<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying delete links
 * as fields.
 */

class commerce_hipay_tpp_direct_debit_handler_delete_link_field extends commerce_hipay_tpp_direct_debit_handler_link_field {

  /**
   * {@inheritdoc}
   */
  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $direct_debit_id = $values->{$this->aliases['direct_debit_id']};

    return l($text, 'admin/commerce/hipay-tpp/direct-debits/' . $direct_debit_id . '/delete');
  }

}

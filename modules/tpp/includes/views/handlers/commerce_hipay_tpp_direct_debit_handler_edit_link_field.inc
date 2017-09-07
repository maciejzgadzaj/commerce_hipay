<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields.
 */

class commerce_hipay_tpp_direct_debit_handler_edit_link_field extends commerce_hipay_tpp_direct_debit_handler_link_field {

  /**
   * {@inheritdoc}
   */
  function render($values) {
    $type = $values->{$this->aliases['type']};

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $direct_debit_id = $values->{$this->aliases['direct_debit_id']};

    return l($text, 'admin/commerce/hipay-tpp/direct-debits/' . $direct_debit_id . '/edit');
  }

}

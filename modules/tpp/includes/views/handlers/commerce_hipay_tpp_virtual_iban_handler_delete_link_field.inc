<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying delete links
 * as fields.
 */

class commerce_hipay_tpp_virtual_iban_handler_delete_link_field extends commerce_hipay_tpp_virtual_iban_handler_link_field {

  /**
   * {@inheritdoc}
   */
  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $virtual_iban_id = $values->{$this->aliases['virtual_iban_id']};

    return l($text, 'admin/commerce/hipay-tpp/virtual-ibans/' . $virtual_iban_id . '/delete');
  }

}

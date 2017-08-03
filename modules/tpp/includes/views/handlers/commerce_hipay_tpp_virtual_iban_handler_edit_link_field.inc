<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields.
 */

class commerce_hipay_tpp_virtual_iban_handler_edit_link_field extends commerce_hipay_tpp_virtual_iban_handler_link_field {

  /**
   * {@inheritdoc}
   */
  function render($values) {
    $type = $values->{$this->aliases['type']};

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $virtual_iban_id = $values->{$this->aliases['virtual_iban_id']};

    return l($text, 'admin/commerce/hipay-tpp/virtual-ibans/' . $virtual_iban_id . '/edit');
  }

}

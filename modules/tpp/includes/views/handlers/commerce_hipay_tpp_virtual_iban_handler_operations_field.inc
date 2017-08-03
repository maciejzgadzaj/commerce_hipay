<?php

/**
 * This field handler aggregates operations that can be done on a Virtual IBAN
 * under a single field providing a more flexible way to present them in a view.
 */
class commerce_hipay_tpp_virtual_iban_handler_operations_field extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  function construct() {
    parent::construct();

    $this->additional_fields['virtual_iban_id'] = 'virtual_iban_id';
  }

  /**
   * {@inheritdoc}
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * {@inheritdoc}
   */
  function render($values) {
    $links = menu_contextual_links('commerce_hipay_tpp_virtual_iban', 'admin/commerce/hipay-tpp/virtual-ibans', array($this->get_value($values, 'virtual_iban_id')));

    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }

}

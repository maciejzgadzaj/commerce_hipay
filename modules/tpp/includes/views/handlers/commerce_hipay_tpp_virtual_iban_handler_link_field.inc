<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying links to entities
 * as fields.
 */

class commerce_hipay_tpp_virtual_iban_handler_link_field extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  function construct() {
    parent::construct();

    $this->additional_fields['virtual_iban_id'] = 'virtual_iban_id';
    $this->additional_fields['type'] = 'type';
  }

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
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
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $virtual_iban_id = $values->{$this->aliases['virtual_iban_id']};

    return l($text, 'admin/commerce/hipay-tpp/virtual-ibans/' . $virtual_iban_id);
  }

}

<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying links to entities
 * as fields.
 */

class commerce_hipay_tpp_handler_link_direct_debit_id extends views_handler_field_numeric {

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_entity'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_entity'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link this field to its entity'),
      '#default_value' => $this->options['link_to_entity'],
    );
  }

  /**
   * {@inheritdoc}
   */
  function render($values) {
    $value = $this->sanitize_value($this->get_value($values));

    if (!empty($this->options['link_to_entity'])) {
      return l($value, 'admin/commerce/hipay-tpp/direct-debits/' . $value);
    }
    else {
      return $value;
    }
  }

}

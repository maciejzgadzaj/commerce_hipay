<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying links to entities
 * as fields.
 */

class commerce_hipay_ws_withdrawal_handler_link_transfer_id extends views_handler_field_field {

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_entity'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_entity'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link this field to its entity'),
      '#default_value' => $this->options['link_to_entity'],
      '#states' => array(
        'visible' => array(
          ':input[name="options[type]"]' => array('value' => 'entityreference_entity_id'),
        ),
      ),
    );
  }

  function render_item($count, $item) {
    if (!empty($this->options['link_to_entity']) && $this->options['type'] == 'entityreference_entity_id') {
      return l(render($item['rendered']), 'admin/commerce/hipay-wallet/transfers/' . $item['raw']['target_id']);
    }
    else {
      return render($item['rendered']);
    }
  }
}

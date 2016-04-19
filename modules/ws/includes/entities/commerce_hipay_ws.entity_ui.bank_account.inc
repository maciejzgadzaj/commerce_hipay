<?php

function commerce_hipay_ws_bank_account_menu() {
  $items = array();

  $items['admin/commerce/config/hipay-wallet/bank-accounts/add'] = array(
    'title' => 'Add a bank account',
    'page callback' => 'commerce_hipay_ws_bank_account_add',
    'access arguments' => array('create commerce_hipay_ws_bank_account entities'),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/config/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account'] = array(
    'title callback' => 'commerce_hipay_ws_bank_account_title',
    'title arguments' => array(5),
    'page callback' => 'commerce_hipay_ws_bank_account_view',
    'page arguments' => array(5),
    'access arguments' => array('view any commerce_hipay_ws_bank_account entity'),
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/config/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account/view'] = array(
    'title' => 'view',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/config/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account/edit'] = array(
    'title' => 'edit',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_ws_bank_account_form', 5),
    'access arguments' => array('edit any commerce_hipay_ws_bank_account entity'),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/config/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account/delete'] = array(
    'title' => 'delete',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_ws_bank_account_delete_form', 5),
    'access arguments' => array('delete any commerce_hipay_ws_bank_account entity'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  return $items;
}

/**
 * Menu callback to display an entity.
 */
function commerce_hipay_ws_bank_account_view($entity, $view_mode = 'full') {
  return entity_view('commerce_hipay_ws_bank_account', array($entity->bank_account_id => $entity), $view_mode, NULL, TRUE);
}

/**
 * Provides a wrapper on the edit form to add a new entity.
 */
function commerce_hipay_ws_bank_account_add() {
  // Create a basic entity structure to be used and passed to the validation
  // and submission functions.
  $bank_account = entity_get_controller('commerce_hipay_ws_bank_account')->create();
  return drupal_get_form('commerce_hipay_ws_bank_account_form', $bank_account);
}

/**
 * Form function to create a bpost_accounting_reference entity.
 */
function commerce_hipay_ws_bank_account_form($form, &$form_state, $bank_account) {
  global $user;

  // Ensure the owner name is accessible if the uid is set.
  if (isset($bank_account->uid) && $owner = user_load($bank_account->uid)) {
    $bank_account->name = $owner->name;
  }

  $form['commerce_hipay_ws_bank_account'] = array(
    '#type' => 'value',
    '#value' => $bank_account,
  );

  // Add the field related form elements.
  field_attach_form('commerce_hipay_ws_bank_account', $bank_account, $form, $form_state);

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  // Add the user account field.
  $form['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User information'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'additional_settings',
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'commerce_hipay_ws') . '/includes/entities/commerce_hipay_ws.entity_ui.js',
        array(
          'type' => 'setting',
          'data' => array('anonymous' => variable_get('anonymous', t('Anonymous'))),
        ),
      ),
    ),
    '#weight' => 50,
  );
  $form['user']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Owned by'),
    '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => isset($bank_account->name) ? $bank_account->name : $user->name,
    '#weight' => -1,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('commerce_hipay_ws_bank_account_delete'),
    '#weight' => 200,
  );

  return $form;
}

/**
 * Validation handler for bpost_accounting_reference_form form.
 */
function commerce_hipay_ws_bank_account_form_validate($form, &$form_state) {
  // Validate the 'Owned by' field.
  if (!empty($form_state['values']['name']) && !($account = user_load_by_name($form_state['values']['name']))) {
    // The use of empty() is mandatory in the context of usernames as the empty
    // string denotes an anonymous user.
    form_set_error('name', t('The username %name does not exist.', array('%name' => $form_state['values']['name'])));
  }

  // Notify field widgets to validate their data.
  field_attach_form_validate('commerce_hipay_ws_bank_account', $form_state['values']['commerce_hipay_ws_bank_account'], $form, $form_state);
}

/**
 * Form submit handler: submits basic_add_form information
 */
function commerce_hipay_ws_bank_account_form_submit($form, &$form_state) {
  $bank_account = $form_state['values']['commerce_hipay_ws_bank_account'];

  // Set the profile's owner uid based on the supplied name.
  if (!empty($form_state['values']['name']) && $account = user_load_by_name($form_state['values']['name'])) {
    $bank_account->uid = $account->uid;
  }
  else {
    $bank_account->uid = 0;
  }

  // Notify field widgets.
  field_attach_submit('commerce_hipay_ws_bank_account', $bank_account, $form, $form_state);

  // Save the Hipay bank account.
  commerce_hipay_ws_bank_account_save($bank_account);

  drupal_set_message(t('Hipay bank account saved.'));
  $form_state['redirect'] = 'admin/commerce/config/hipay-wallet/bank-accounts/' . $bank_account->bank_account_id;
}

/**
 * Form deletion handler.
 */
function commerce_hipay_ws_bank_account_delete($form, &$form_state) {
  return drupal_get_form('commerce_hipay_ws_bank_account_delete_form', $form_state['values']['commerce_hipay_ws_bank_account']);
}

/**
 * Form constructor for delete an accounting reference form.
 */
function commerce_hipay_ws_bank_account_delete_form($form, &$form_state, $bank_account) {
  $form_state['commerce_hipay_ws_bank_account'] = $bank_account;

  $form = confirm_form($form,
    t('Are you sure you want to delete this bank account?'),
    'admin/commerce/config/hipay-wallet/bank-accounts',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Form submission handler for bpost_accounting_reference_ui_delete_form().
 */
function commerce_hipay_ws_bank_account_delete_form_submit($form, &$form_state) {
  $form_state['commerce_hipay_ws_bank_account']->delete();

  drupal_set_message(t('The bank account has been deleted.'));
  $form_state['redirect'] = 'admin/commerce/config/hipay-wallet/bank-accounts';
}

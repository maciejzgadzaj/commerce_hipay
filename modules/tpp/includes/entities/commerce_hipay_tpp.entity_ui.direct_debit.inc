<?php

/**
 * Returns Direct Debit mandate-related menu items.
 *
 * @see commerce_hipay_tpp_menu()
 */
function commerce_hipay_tpp_direct_debit_menu() {
  $items = array();

  $items['admin/commerce/hipay-tpp/direct-debits/add'] = array(
    'title' => 'Add a Direct Debit mandate',
    'page callback' => 'commerce_hipay_tpp_direct_debit_ui_add',
    'access callback' => 'commerce_hipay_tpp_direct_debit_access',
    'access arguments' => array('create'),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.direct_debit.inc',
  );

  $items['admin/commerce/hipay-tpp/direct-debits/%commerce_hipay_tpp_direct_debit'] = array(
    'title callback' => 'commerce_hipay_tpp_direct_debit_title',
    'title arguments' => array(4),
    'page callback' => 'commerce_hipay_tpp_direct_debit_ui_view',
    'page arguments' => array(4),
    'access callback' => 'commerce_hipay_tpp_direct_debit_access',
    'access arguments' => array('view', 4),
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.direct_debit.inc',
  );

  $items['admin/commerce/hipay-tpp/direct-debits/%commerce_hipay_tpp_direct_debit/view'] = array(
    'title' => 'view',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.direct_debit.inc',
  );

  $items['admin/commerce/hipay-tpp/direct-debits/%commerce_hipay_tpp_direct_debit/view/entity'] = array(
    'title' => 'local',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.direct_debit.inc',
  );

  $items['admin/commerce/hipay-tpp/direct-debits/%commerce_hipay_tpp_direct_debit/view/payload'] = array(
    'title' => 'payload',
    'page callback' => 'commerce_hipay_tpp_direct_debit_ui_view_payload_info',
    'page arguments' => array(4),
    'access arguments' => array('access Hipay Direct Debit mandate calls payload'),
    'type' => MENU_LOCAL_TASK,
    'weight' => -2,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.direct_debit.inc',
  );

  $items['admin/commerce/hipay-tpp/direct-debits/%commerce_hipay_tpp_direct_debit/edit'] = array(
    'title' => 'edit',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_tpp_direct_debit_ui_form', 4),
    'access callback' => 'commerce_hipay_tpp_direct_debit_access',
    'access arguments' => array('edit', 4),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.direct_debit.inc',
  );

  $items['admin/commerce/hipay-tpp/direct-debits/%commerce_hipay_tpp_direct_debit/delete'] = array(
    'title' => 'delete',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_tpp_direct_debit_ui_delete_form', 4),
    'access callback' => 'commerce_hipay_tpp_direct_debit_access',
    'access arguments' => array('delete', 4),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.direct_debit.inc',
  );

  return $items;
}

/**
 * Checks the current user's access to the specified Direct Debit operation.
 *
 * @param string $op
 *   The operation being performed. One of 'view', 'edit', 'create' or
 *   'delete'.
 * @param object $entity
 *   Optionally an entity to check access for. If no entity is given, it will be
 *   determined whether access is allowed for all entities of the given type.
 * @param object $account
 *   The user to check for. Leave it to NULL to check for the global user.
 *
 * @return boolean
 *   Whether access is allowed or not. If the entity type does not specify any
 *   access information, NULL is returned.
 */
function commerce_hipay_tpp_direct_debit_access($op, $entity = NULL, $account = NULL) {
  switch ($op) {
    case 'create':
      return user_access('create commerce_hipay_tpp_direct_debit entities');
    case 'view':
      return user_access('view any commerce_hipay_tpp_direct_debit entity');
    case 'edit':
      return user_access('edit any commerce_hipay_tpp_direct_debit entity');
    case 'delete':
      return user_access('administer commerce_hipay_tpp_direct_debit entities');
  }
}

/**
 * Menu callback: displays entity information.
 *
 * @param object $entity
 *   An entity to render.
 * @param string $view_mode
 *   A view mode as used by this entity type, e.g. 'full', 'teaser'...
 *
 * @return
 *   The renderable array, keyed by the entity type and by entity identifiers,
 *   for which the entity name is used if existing - see entity_id(). If there
 *   is no information on how to view an entity, FALSE is returned.
 *
 * @see commerce_hipay_tpp_direct_debit_menu()
 */
function commerce_hipay_tpp_direct_debit_ui_view($entity, $view_mode = 'full') {
  $return = entity_view('commerce_hipay_tpp_direct_debit', array($entity->direct_debit_id => $entity), $view_mode, NULL, TRUE);

  if (empty($entity->debit_agreement_id)) {
    $return['commerce_hipay_tpp_direct_debit_ui_create'] = drupal_get_form('commerce_hipay_tpp_direct_debit_ui_view_hipay_create_form', $entity);
  }

  return $return;
}

/**
 * Implements hook_ENTITY_TYPE_view_alter() for commerce_hipay_tpp_direct_debit.
 */
function commerce_hipay_tpp_commerce_hipay_tpp_direct_debit_view_alter(&$build, $entity_type) {
  $build['uid']['#content'] = l($build['#entity']->uid, 'user/' . $build['#entity']->uid);
  $build['created']['#content'] = format_date($build['#entity']->created);
  $build['changed']['#content'] = format_date($build['#entity']->changed);
}

/**
 * Menu calback: displays payload information.
 *
 * @param object $entity
 *   An entity to render.
 * @param string $view_mode
 *   A view mode as used by this entity type, e.g. 'full', 'teaser'...

 * @return string
 *   The payload information.
 *
 * @see commerce_hipay_tpp_direct_debit_menu()
 */
function commerce_hipay_tpp_direct_debit_ui_view_payload_info($entity, $view_mode = 'full') {
  $markup = '';

  if (!empty($entity->data['payload'])) {
    $markup = '<pre>' . check_plain(print_r($entity->data['payload'], TRUE)) . '</pre>';
  }

  return $markup;
}

/**
 * Returns a form to create a Direct Debit mandate in Hipay.
 *
 * @see commerce_hipay_tpp_direct_debit_ui_view_hipay_create_form_submit()
 */
function commerce_hipay_tpp_direct_debit_ui_view_hipay_create_form($form, &$form_state, $entity) {
  $form_state['commerce_hipay_tpp_direct_debit'] = $entity;

  $form['info'] = array(
    '#markup' => '<p>' . t('The Direct Debit mandate has not yet been created for this entity in Hipay.') . '</p>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create Direct Debit mandate in Hipay TPP'),
  );

  return $form;
}

/**
 * Form submit handler: creates a Direct Debit mandate in Hipay.
 *
 * @see commerce_hipay_tpp_direct_debit_ui_view_hipay_create_form()
 */
function commerce_hipay_tpp_direct_debit_ui_view_hipay_create_form_submit($form, $form_state) {
  $direct_debit = $form_state['commerce_hipay_tpp_direct_debit'];

  $response = commerce_hipay_tpp_api_direct_debit_create($direct_debit);

  if ($response['code'] !== COMMERCE_HIPAY_TPP_REST_STATUS_SUCCESS) {
    drupal_set_message(t('Error creating direct debit mandate in Hipay: @error', array('@error' => $response['description'])), 'error');
  }
}

/**
 * Provides a wrapper on the edit form to add a new Direct Debit entity.
 *
 * @see commerce_hipay_tpp_direct_debit_menu()
 */
function commerce_hipay_tpp_direct_debit_ui_add() {
  // Create a basic entity structure to be used and passed to the validation
  // and submission functions.
  $direct_debit = entity_get_controller('commerce_hipay_tpp_direct_debit')->create();
  return drupal_get_form('commerce_hipay_tpp_direct_debit_ui_form', $direct_debit);
}

/**
 * Form function to edit a Direct Debit mandate entity.
 *
 * @see commerce_hipay_tpp_direct_debit_ui_form_validate()
 * @see commerce_hipay_tpp_direct_debit_ui_form_submit()
 */
function commerce_hipay_tpp_direct_debit_ui_form($form, &$form_state, $direct_debit) {
  global $user;

  // Ensure the owner name is accessible if the uid is set.
  if (isset($direct_debit->uid) && $owner = user_load($direct_debit->uid)) {
    $direct_debit->name = $owner->name;
  }

  if (empty($direct_debit->created)) {
    $direct_debit->date = format_date(REQUEST_TIME, 'custom', 'Y-m-d H:i:s O');
  }

  $form['commerce_hipay_tpp_direct_debit'] = array(
    '#type' => 'value',
    '#value' => $direct_debit,
  );

  // Add the field related form elements.
  field_attach_form('commerce_hipay_tpp_direct_debit', $direct_debit, $form, $form_state);

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
        drupal_get_path('module', 'commerce_hipay_tpp') . '/includes/entities/commerce_hipay_tpp.entity_ui.js',
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
    '#default_value' => isset($direct_debit->name) ? $direct_debit->name : $user->name,
    '#weight' => -1,
  );

  // Add a log checkbox and timestamp field to a Revision information tab.
  $form['revision_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Revision information', array()),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'additional_settings',
    '#weight' => 40,
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'commerce_hipay_tpp') . '/includes/entities/commerce_hipay_tpp.entity_ui.js'),
    ),
  );
  $form['revision_information']['revision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create new revision'),
    '#description' => t('If a revision log message is entered, a revision will be created even if this is unchecked.'),
    '#default_value' => empty($direct_debit->is_new),
    '#access' => user_access('administer commerce_hipay_tpp_direct_debit entities'),
  );
  $form['revision_information']['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Created on'),
    '#description' => t('Format: %time. The date format is YYYY-MM-DD and %timezone is the time zone offset from UTC. Leave blank to use the time of form submission.', array(
      '%time' => !empty($direct_debit->date) ? date_format(date_create($direct_debit->date), 'Y-m-d H:i:s O') : format_date($direct_debit->created, 'custom', 'Y-m-d H:i:s O'),
      '%timezone' => !empty($direct_debit->date) ? date_format(date_create($direct_debit->date), 'O') : format_date($direct_debit->created, 'custom', 'O'),
    )),
    '#maxlength' => 25,
    '#default_value' => !empty($direct_debit->created) ? format_date($direct_debit->created, 'custom', 'Y-m-d H:i:s O') : '',
  );
  $form['revision_information']['log'] = array(
    '#type' => 'textarea',
    '#title' => t('Revision log message'),
    '#description' => t('Provide an explanation of the changes you are making. This will provide a meaningful audit trail for updates to this entity.'),
    '#rows' => 4,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('commerce_hipay_tpp_direct_debit_delete'),
    '#weight' => 200,
  );

  return $form;
}

/**
 * Validation handler for Direct Debit mandate edit form.
 *
 * @see commerce_hipay_tpp_direct_debit_ui_form()
 * @see commerce_hipay_tpp_direct_debit_ui_form_submit()
 */
function commerce_hipay_tpp_direct_debit_ui_form_validate($form, &$form_state) {
  $direct_debit = $form_state['values']['commerce_hipay_tpp_direct_debit'];

  // Validate the 'Owned by' field.
  if (!empty($form_state['values']['name']) && !($account = user_load_by_name($form_state['values']['name']))) {
    // The use of empty() is mandatory in the context of usernames as the empty
    // string denotes an anonymous user.
    form_set_error('name', t('The username %name does not exist.', array('%name' => $form_state['values']['name'])));
  }

  // Validate the 'Created on' field.
  if (!empty($form_state['values']['date']) && strtotime($form_state['values']['date']) === FALSE) {
    form_set_error('date', t('You have to specify a valid date.'));
  }

  // Notify field widgets to validate their data.
  field_attach_form_validate('commerce_hipay_tpp_direct_debit', $form_state['values']['commerce_hipay_tpp_direct_debit'], $form, $form_state);
}

/**
 * Submission handler for Direct Debit mandate edit form.
 *
 * @see commerce_hipay_tpp_direct_debit_ui_form()
 * @see commerce_hipay_tpp_direct_debit_ui_form_validate()
 */
function commerce_hipay_tpp_direct_debit_ui_form_submit($form, &$form_state) {
  $direct_debit = $form_state['values']['commerce_hipay_tpp_direct_debit'];

  if ($form_state['values']['revision'] || !empty($form_state['values']['log'])) {
    $direct_debit->revision = TRUE;
    $direct_debit->log = $form_state['values']['log'];
  }

  // Set the profile's owner uid based on the supplied name.
  if (!empty($form_state['values']['name']) && $account = user_load_by_name($form_state['values']['name'])) {
    $direct_debit->uid = $account->uid;
  }
  else {
    $direct_debit->uid = 0;
  }

  $direct_debit->created = !empty($form_state['values']['date']) ? strtotime($form_state['values']['date']) : REQUEST_TIME;

  // Notify field widgets.
  field_attach_submit('commerce_hipay_tpp_direct_debit', $direct_debit, $form, $form_state);

  // Save the Direct Debit mandate entity.
  commerce_hipay_tpp_direct_debit_save($direct_debit);

  drupal_set_message(t('Hipay Direct Debit mandate saved.'));
  $form_state['redirect'] = 'admin/commerce/hipay-tpp/direct-debits/' . $direct_debit->direct_debit_id;
}

/**
 * Form constructor for Direct Debit mandate deletion form.
 *
 * @see commerce_hipay_tpp_direct_debit_ui_form_submit()
 */
function commerce_hipay_tpp_direct_debit_ui_delete_form($form, &$form_state, $direct_debit) {
  $form_state['commerce_hipay_tpp_direct_debit'] = $direct_debit;

  $form = confirm_form($form,
    t('Are you sure you want to delete this Direct Debit mandate?'),
    'admin/commerce/hipay-tpp/direct-debits',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submission handler for Direct Debit mandate deletion form.
 *
 * @see commerce_hipay_tpp_direct_debit_ui_delete_form()
 */
function commerce_hipay_tpp_direct_debit_ui_delete_form_submit($form, &$form_state) {
  if (commerce_hipay_tpp_direct_debit_delete($form_state['commerce_hipay_tpp_direct_debit']->direct_debit_id)) {
    drupal_set_message(t('The Direct Debit mandate has been deleted.'));
  }
  else {
    drupal_set_message(t('The Direct Debit mandate could not be deleted.'), 'error');
  }

  $form_state['redirect'] = 'admin/commerce/hipay-tpp/direct-debits';
}

/**
 * Provides Views data structure for the Direct Debit mandate entities.
 *
 * @see commerce_hipay_tpp_views_data_alter()
 */
function commerce_hipay_tpp_direct_debit_views_data_alter(&$data) {
  // Direct Debit mandates.
  $data['commerce_hipay_tpp_direct_debit']['direct_debit_id']['field']['handler'] = 'commerce_hipay_tpp_handler_link_direct_debit_id';
  $data['commerce_hipay_tpp_direct_debit']['uid'] = array(
    'title' => t('Uid'),
    'help' => t("The owner's user ID."),
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name', // display this field in the summary
    ),
    'filter' => array(
      'title' => t('Name'),
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('Owner'),
      'help' => t("Relate this Direct Debit mandate to its owner's user account"),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('Direct Debit mandate owner'),
    ),
  );
  // Expose gender.
  $data['commerce_hipay_tpp_direct_debit']['gender'] = array(
    'title' => t('Gender'),
    'help' => t('Gender of Direct Debit mandate owner.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  // Expose first name.
  $data['commerce_hipay_tpp_direct_debit']['first_name'] = array(
    'title' => t('First name'),
    'help' => t('First name of Direct Debit mandate owner.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  // Expose last name.
  $data['commerce_hipay_tpp_direct_debit']['last_name'] = array(
    'title' => t('Last name'),
    'help' => t('Last name of Direct Debit mandate owner.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  // Expose issuer bank ID.
  $data['commerce_hipay_tpp_direct_debit']['issuer_bank_id'] = array(
    'title' => t('Issuer bank ID'),
    'help' => t('ID of the bank requesting Direct Debit mandate.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  // Expose bank name.
  $data['commerce_hipay_tpp_direct_debit']['bank_name'] = array(
    'title' => t('Bank name'),
    'help' => t('Bank name of Direct Debit mandate owner bank account.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  // Expose IBAN.
  $data['commerce_hipay_tpp_direct_debit']['iban'] = array(
    'title' => t('IBAN'),
    'help' => t('IBAN number of Direct Debit mandate owner bank account.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  // Expose debit mandate ID.
  $data['commerce_hipay_tpp_direct_debit']['debit_agreement_id'] = array(
    'title' => t('Remote ID'),
    'help' => t('Hipay remote ID of Direct Debit mandate.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  // Expose debit mandate status.
  $data['commerce_hipay_tpp_direct_debit']['debit_agreement_status'] = array(
    'title' => t('Debit mandate status'),
    'help' => t('Hipay status of Direct Debit mandate.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  // Expose operation links.
  $data['commerce_hipay_tpp_direct_debit']['link'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the Direct Debit mandate.'),
      'handler' => 'commerce_hipay_tpp_direct_debit_handler_link_field',
    ),
  );
  $data['commerce_hipay_tpp_direct_debit']['edit'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a link to the edit form for the Direct Debit mandate.'),
      'handler' => 'commerce_hipay_tpp_direct_debit_handler_edit_link_field',
    ),
  );
  $data['commerce_hipay_tpp_direct_debit']['delete'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a link to delete the Direct Debit mandate.'),
      'handler' => 'commerce_hipay_tpp_direct_debit_handler_delete_link_field',
    ),
  );
  $data['commerce_hipay_tpp_direct_debit']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for the Direct Debit mandate.'),
      'handler' => 'commerce_hipay_tpp_direct_debit_handler_operations_field',
    ),
  );




  // Direct Debit mandate revisions.
  $data['commerce_hipay_tpp_direct_debit_revision']['revision_id'] = array(
    'title' => t('Revision ID'),
    'help' => t('The ID of the Direct Debit mandate revision.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'commerce_hipay_tpp_direct_debit',
      'base field' => 'revision_id',
      'title' => t('Direct Debit mandate'),
      'label' => t('Latest Direct Debit mandate revision'),
    ),
  );
  $data['commerce_hipay_tpp_direct_debit_revision']['revision_uid'] = array(
    'title' => t('User'),
    'help' => t('Relate Direct Debit mandate revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'revision_uid',
      'field_name' => 'revision_uid',
      'label' => t('Revision user'),
    ),
  );
  $data['commerce_hipay_tpp_direct_debit_revision']['log'] = array(
    'title' => t('Log message'),
    'help' => t('The log message entered when the revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['commerce_hipay_tpp_direct_debit_revision']['revision_timestamp'] = array(
    'title' => t('Revision date'),
    'help' => t('The date the Direct Debit mandate revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

}

/**
 * Provides default views for the Direct Debit mandate entities.
 *
 * @see commerce_hipay_tpp_views_default_views()
 */
function commerce_hipay_tpp_direct_debit_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'hipay_tpp_direct_debits';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_tpp_direct_debit';
  $view->human_name = 'Hipay TPP: Direct Debit mandates';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Direct Debit mandates';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view any commerce_hipay_tpp_direct_debit entity';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'direct_debit_id' => 'direct_debit_id',
    'issuer_bank_id' => 'issuer_bank_id',
    'gender' => 'last_name',
    'first_name' => 'last_name',
    'last_name' => 'last_name',
    'bank_name' => 'bank_name',
    'iban' => 'iban',
    'debit_agreement_id' => 'debit_agreement_id',
    'debit_agreement_status' => 'debit_agreement_status',
    'changed' => 'changed',
    'name' => 'name',
    'order_id' => 'order_id',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = 'direct_debit_id';
  $handler->display->display_options['style_options']['info'] = array(
    'direct_debit_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'issuer_bank_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'gender' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'first_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'last_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' ',
      'empty_column' => 0,
    ),
    'bank_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'iban' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'debit_agreement_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'debit_agreement_status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No Direct Debit mandates found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Hipay Direct Debit mandate: Owner */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['direct_debit_order_target_id']['id'] = 'direct_debit_order_target_id';
  $handler->display->display_options['relationships']['direct_debit_order_target_id']['table'] = 'field_data_direct_debit_order';
  $handler->display->display_options['relationships']['direct_debit_order_target_id']['field'] = 'direct_debit_order_target_id';
  /* Field: Hipay Direct Debit mandate: Direct Debit mandate ID */
  $handler->display->display_options['fields']['direct_debit_id']['id'] = 'direct_debit_id';
  $handler->display->display_options['fields']['direct_debit_id']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['fields']['direct_debit_id']['field'] = 'direct_debit_id';
  $handler->display->display_options['fields']['direct_debit_id']['label'] = 'ID';
  $handler->display->display_options['fields']['direct_debit_id']['separator'] = '';
  $handler->display->display_options['fields']['direct_debit_id']['link_to_entity'] = TRUE;
  /* Field: Hipay Direct Debit mandate: Issuer bank ID */
  $handler->display->display_options['fields']['issuer_bank_id']['id'] = 'issuer_bank_id';
  $handler->display->display_options['fields']['issuer_bank_id']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['fields']['issuer_bank_id']['field'] = 'issuer_bank_id';
  /* Field: Hipay Direct Debit mandate: Gender */
  $handler->display->display_options['fields']['gender']['id'] = 'gender';
  $handler->display->display_options['fields']['gender']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['fields']['gender']['field'] = 'gender';
  /* Field: Hipay Direct Debit mandate: First name */
  $handler->display->display_options['fields']['first_name']['id'] = 'first_name';
  $handler->display->display_options['fields']['first_name']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['fields']['first_name']['field'] = 'first_name';
  /* Field: Hipay Direct Debit mandate: Last name */
  $handler->display->display_options['fields']['last_name']['id'] = 'last_name';
  $handler->display->display_options['fields']['last_name']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['fields']['last_name']['field'] = 'last_name';
  $handler->display->display_options['fields']['last_name']['label'] = 'Name';
  /* Field: Hipay Direct Debit mandate: Bank name */
  $handler->display->display_options['fields']['bank_name']['id'] = 'bank_name';
  $handler->display->display_options['fields']['bank_name']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['fields']['bank_name']['field'] = 'bank_name';
  /* Field: Hipay Direct Debit mandate: IBAN */
  $handler->display->display_options['fields']['iban']['id'] = 'iban';
  $handler->display->display_options['fields']['iban']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['fields']['iban']['field'] = 'iban';
  /* Field: Hipay Direct Debit mandate: Debit mandate ID */
  $handler->display->display_options['fields']['debit_agreement_id']['id'] = 'debit_agreement_id';
  $handler->display->display_options['fields']['debit_agreement_id']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['fields']['debit_agreement_id']['field'] = 'debit_agreement_id';
  $handler->display->display_options['fields']['debit_agreement_id']['label'] = 'Remote ID';
  /* Field: Hipay Direct Debit mandate: Debit mandate status */
  $handler->display->display_options['fields']['debit_agreement_status']['id'] = 'debit_agreement_status';
  $handler->display->display_options['fields']['debit_agreement_status']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['fields']['debit_agreement_status']['field'] = 'debit_agreement_status';
  $handler->display->display_options['fields']['debit_agreement_status']['label'] = 'Status';
  /* Field: Hipay Direct Debit mandate: Date updated */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Updated';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'long';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Owner';
  /* Field: GROUP_CONCAT(DISTINCT NULLIF(Commerce Order: Order ID, '') SEPARATOR ', ') */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['relationship'] = 'direct_debit_order_target_id';
  $handler->display->display_options['fields']['order_id']['group_type'] = 'group_concat_distinct';
  $handler->display->display_options['fields']['order_id']['label'] = 'Order';
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 'admin';
  /* Field: Hipay Direct Debit mandate: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = '';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;
  /* Filter criterion: Hipay Direct Debit mandate: Issuer bank ID */
  $handler->display->display_options['filters']['issuer_bank_id']['id'] = 'issuer_bank_id';
  $handler->display->display_options['filters']['issuer_bank_id']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['filters']['issuer_bank_id']['field'] = 'issuer_bank_id';
  $handler->display->display_options['filters']['issuer_bank_id']['operator'] = 'contains';
  $handler->display->display_options['filters']['issuer_bank_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['issuer_bank_id']['expose']['operator_id'] = 'issuer_bank_id_op';
  $handler->display->display_options['filters']['issuer_bank_id']['expose']['label'] = 'Issuer bank ID';
  $handler->display->display_options['filters']['issuer_bank_id']['expose']['operator'] = 'issuer_bank_id_op';
  $handler->display->display_options['filters']['issuer_bank_id']['expose']['identifier'] = 'issuer_bank_id';
  $handler->display->display_options['filters']['issuer_bank_id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    6 => 0,
    5 => 0,
    7 => 0,
    8 => 0,
  );
  /* Filter criterion: Global: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'combine';
  $handler->display->display_options['filters']['combine']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    6 => 0,
    5 => 0,
    7 => 0,
    8 => 0,
  );
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'gender' => 'gender',
    'first_name' => 'first_name',
    'last_name' => 'last_name',
  );
  /* Filter criterion: Hipay Direct Debit mandate: Bank name */
  $handler->display->display_options['filters']['bank_name']['id'] = 'bank_name';
  $handler->display->display_options['filters']['bank_name']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['filters']['bank_name']['field'] = 'bank_name';
  $handler->display->display_options['filters']['bank_name']['operator'] = 'contains';
  $handler->display->display_options['filters']['bank_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['bank_name']['expose']['operator_id'] = 'bank_name_op';
  $handler->display->display_options['filters']['bank_name']['expose']['label'] = 'Bank name';
  $handler->display->display_options['filters']['bank_name']['expose']['operator'] = 'bank_name_op';
  $handler->display->display_options['filters']['bank_name']['expose']['identifier'] = 'bank_name';
  $handler->display->display_options['filters']['bank_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    6 => 0,
    5 => 0,
    7 => 0,
    8 => 0,
  );
  /* Filter criterion: Hipay Direct Debit mandate: IBAN */
  $handler->display->display_options['filters']['iban']['id'] = 'iban';
  $handler->display->display_options['filters']['iban']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['filters']['iban']['field'] = 'iban';
  $handler->display->display_options['filters']['iban']['operator'] = 'contains';
  $handler->display->display_options['filters']['iban']['exposed'] = TRUE;
  $handler->display->display_options['filters']['iban']['expose']['operator_id'] = 'iban_op';
  $handler->display->display_options['filters']['iban']['expose']['label'] = 'IBAN';
  $handler->display->display_options['filters']['iban']['expose']['operator'] = 'iban_op';
  $handler->display->display_options['filters']['iban']['expose']['identifier'] = 'iban';
  $handler->display->display_options['filters']['iban']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    6 => 0,
    5 => 0,
    7 => 0,
    8 => 0,
  );
  /* Filter criterion: Hipay Direct Debit mandate: Debit mandate ID */
  $handler->display->display_options['filters']['debit_agreement_id']['id'] = 'debit_agreement_id';
  $handler->display->display_options['filters']['debit_agreement_id']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['filters']['debit_agreement_id']['field'] = 'debit_agreement_id';
  $handler->display->display_options['filters']['debit_agreement_id']['operator'] = 'contains';
  $handler->display->display_options['filters']['debit_agreement_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['debit_agreement_id']['expose']['operator_id'] = 'debit_agreement_id_op';
  $handler->display->display_options['filters']['debit_agreement_id']['expose']['label'] = 'Remote ID';
  $handler->display->display_options['filters']['debit_agreement_id']['expose']['operator'] = 'debit_agreement_id_op';
  $handler->display->display_options['filters']['debit_agreement_id']['expose']['identifier'] = 'debit_agreement_id';
  $handler->display->display_options['filters']['debit_agreement_id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    6 => 0,
    5 => 0,
    7 => 0,
    8 => 0,
  );
  /* Filter criterion: Hipay Direct Debit mandate: Debit mandate status */
  $handler->display->display_options['filters']['debit_agreement_status']['id'] = 'debit_agreement_status';
  $handler->display->display_options['filters']['debit_agreement_status']['table'] = 'commerce_hipay_tpp_direct_debit';
  $handler->display->display_options['filters']['debit_agreement_status']['field'] = 'debit_agreement_status';
  $handler->display->display_options['filters']['debit_agreement_status']['operator'] = 'contains';
  $handler->display->display_options['filters']['debit_agreement_status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['debit_agreement_status']['expose']['operator_id'] = 'debit_agreement_status_op';
  $handler->display->display_options['filters']['debit_agreement_status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['debit_agreement_status']['expose']['operator'] = 'debit_agreement_status_op';
  $handler->display->display_options['filters']['debit_agreement_status']['expose']['identifier'] = 'debit_agreement_status';
  $handler->display->display_options['filters']['debit_agreement_status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    6 => 0,
    5 => 0,
    7 => 0,
    8 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-tpp/direct-debits/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['description'] = 'Find and manage Hipay Direct Debit mandates.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Direct Debit mandates';
  $handler->display->display_options['tab_options']['description'] = 'Manage Hipay Direct Debit mandates.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['hipay_tpp_direct_debits'] = array(
    t('Master'),
    t('Direct Debit mandates'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('No Direct Debit mandates found.'),
    t('Direct Debit mandate owner'),
    t('Commerce Order entity referenced from direct_debit_order'),
    t('ID'),
    t('.'),
    t('Issuer bank ID'),
    t('Gender'),
    t('First name'),
    t('Name'),
    t('Bank name'),
    t('IBAN'),
    t('Remote ID'),
    t('Status'),
    t('Updated'),
    t('Owner'),
    t('Order'),
    t('Page'),
  );
  $export[$view->name] = $view;

  $view = new view();
  $view->name = 'hipay_tpp_direct_debit_revisions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_tpp_direct_debit_revision';
  $view->human_name = 'Hipay TPP: Direct Debit mandate revisions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Revisions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer commerce_hipay_tpp_direct_debit entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'revision_id' => 'revision_id',
    'revision_timestamp' => 'revision_timestamp',
    'name' => 'name',
    'issuer_bank_id' => 'issuer_bank_id',
    'gender' => 'last_name',
    'first_name' => 'last_name',
    'last_name' => 'last_name',
    'bank_name' => 'bank_name',
    'iban' => 'iban',
    'debit_agreement_id' => 'debit_agreement_id',
    'debit_agreement_status' => 'debit_agreement_status',
    'order_id' => 'order_id',
    'log' => 'log',
  );
  $handler->display->display_options['style_options']['default'] = 'revision_id';
  $handler->display->display_options['style_options']['info'] = array(
    'revision_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'revision_timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'issuer_bank_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'gender' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'first_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'last_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' ',
      'empty_column' => 0,
    ),
    'bank_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'iban' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'debit_agreement_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'debit_agreement_status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'log' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* Relationship: Hipay Direct Debit mandate Revisions: User */
  $handler->display->display_options['relationships']['revision_uid']['id'] = 'revision_uid';
  $handler->display->display_options['relationships']['revision_uid']['table'] = 'commerce_hipay_tpp_direct_debit_revision';
  $handler->display->display_options['relationships']['revision_uid']['field'] = 'revision_uid';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['direct_debit_order_target_id']['id'] = 'direct_debit_order_target_id';
  $handler->display->display_options['relationships']['direct_debit_order_target_id']['table'] = 'field_revision_direct_debit_order';
  $handler->display->display_options['relationships']['direct_debit_order_target_id']['field'] = 'direct_debit_order_target_id';
  /* Field: Hipay Direct Debit mandate Revisions: Revision ID */
  $handler->display->display_options['fields']['revision_id']['id'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['table'] = 'commerce_hipay_tpp_direct_debit_revision';
  $handler->display->display_options['fields']['revision_id']['field'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['label'] = 'Revision';
  /* Field: Hipay Direct Debit mandate Revisions: Revision date */
  $handler->display->display_options['fields']['revision_timestamp']['id'] = 'revision_timestamp';
  $handler->display->display_options['fields']['revision_timestamp']['table'] = 'commerce_hipay_tpp_direct_debit_revision';
  $handler->display->display_options['fields']['revision_timestamp']['field'] = 'revision_timestamp';
  $handler->display->display_options['fields']['revision_timestamp']['label'] = 'Created on';
  $handler->display->display_options['fields']['revision_timestamp']['date_format'] = 'short';
  $handler->display->display_options['fields']['revision_timestamp']['second_date_format'] = 'long';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'revision_uid';
  $handler->display->display_options['fields']['name']['label'] = 'Created by';
  /* Field: Hipay Direct Debit mandate Revisions: Issuer bank ID */
  $handler->display->display_options['fields']['issuer_bank_id']['id'] = 'issuer_bank_id';
  $handler->display->display_options['fields']['issuer_bank_id']['table'] = 'commerce_hipay_tpp_direct_debit_revision';
  $handler->display->display_options['fields']['issuer_bank_id']['field'] = 'issuer_bank_id';
  /* Field: Hipay Direct Debit mandate Revisions: Owner gender */
  $handler->display->display_options['fields']['gender']['id'] = 'gender';
  $handler->display->display_options['fields']['gender']['table'] = 'commerce_hipay_tpp_direct_debit_revision';
  $handler->display->display_options['fields']['gender']['field'] = 'gender';
  $handler->display->display_options['fields']['gender']['label'] = 'Gender';
  /* Field: Hipay Direct Debit mandate Revisions: Owner first name */
  $handler->display->display_options['fields']['first_name']['id'] = 'first_name';
  $handler->display->display_options['fields']['first_name']['table'] = 'commerce_hipay_tpp_direct_debit_revision';
  $handler->display->display_options['fields']['first_name']['field'] = 'first_name';
  $handler->display->display_options['fields']['first_name']['label'] = 'First name';
  /* Field: Hipay Direct Debit mandate Revisions: Owner last name */
  $handler->display->display_options['fields']['last_name']['id'] = 'last_name';
  $handler->display->display_options['fields']['last_name']['table'] = 'commerce_hipay_tpp_direct_debit_revision';
  $handler->display->display_options['fields']['last_name']['field'] = 'last_name';
  $handler->display->display_options['fields']['last_name']['label'] = 'Name';
  /* Field: Hipay Direct Debit mandate Revisions: Bank name */
  $handler->display->display_options['fields']['bank_name']['id'] = 'bank_name';
  $handler->display->display_options['fields']['bank_name']['table'] = 'commerce_hipay_tpp_direct_debit_revision';
  $handler->display->display_options['fields']['bank_name']['field'] = 'bank_name';
  /* Field: Hipay Direct Debit mandate Revisions: IBAN */
  $handler->display->display_options['fields']['iban']['id'] = 'iban';
  $handler->display->display_options['fields']['iban']['table'] = 'commerce_hipay_tpp_direct_debit_revision';
  $handler->display->display_options['fields']['iban']['field'] = 'iban';
  /* Field: Hipay Direct Debit mandate Revisions: Remote ID */
  $handler->display->display_options['fields']['debit_agreement_id']['id'] = 'debit_agreement_id';
  $handler->display->display_options['fields']['debit_agreement_id']['table'] = 'commerce_hipay_tpp_direct_debit_revision';
  $handler->display->display_options['fields']['debit_agreement_id']['field'] = 'debit_agreement_id';
  /* Field: Hipay Direct Debit mandate Revisions: Debit mandate status */
  $handler->display->display_options['fields']['debit_agreement_status']['id'] = 'debit_agreement_status';
  $handler->display->display_options['fields']['debit_agreement_status']['table'] = 'commerce_hipay_tpp_direct_debit_revision';
  $handler->display->display_options['fields']['debit_agreement_status']['field'] = 'debit_agreement_status';
  $handler->display->display_options['fields']['debit_agreement_status']['label'] = 'Status';
  /* Field: GROUP_CONCAT(DISTINCT NULLIF(Commerce Order: Order ID, '') SEPARATOR ', ') */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['relationship'] = 'direct_debit_order_target_id';
  $handler->display->display_options['fields']['order_id']['group_type'] = 'group_concat_distinct';
  $handler->display->display_options['fields']['order_id']['label'] = 'Order';
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 'admin';
  /* Field: Hipay Direct Debit mandate Revisions: Log message */
  $handler->display->display_options['fields']['log']['id'] = 'log';
  $handler->display->display_options['fields']['log']['table'] = 'commerce_hipay_tpp_direct_debit_revision';
  $handler->display->display_options['fields']['log']['field'] = 'log';
  /* Contextual filter: Hipay Direct Debit mandate Revisions: Direct Debit mandate ID */
  $handler->display->display_options['arguments']['direct_debit_id']['id'] = 'direct_debit_id';
  $handler->display->display_options['arguments']['direct_debit_id']['table'] = 'commerce_hipay_tpp_direct_debit_revision';
  $handler->display->display_options['arguments']['direct_debit_id']['field'] = 'direct_debit_id';
  $handler->display->display_options['arguments']['direct_debit_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['direct_debit_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['direct_debit_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['direct_debit_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['direct_debit_id']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-tpp/direct-debits/%/revisions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'revisions';
  $handler->display->display_options['menu']['description'] = 'View revisions of this Direct Debit mandate.';
  $handler->display->display_options['menu']['weight'] = '20';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['hipay_tpp_direct_debit_revisions'] = array(
    t('Master'),
    t('Revisions'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Revision user'),
    t('Revision'),
    t('Created on'),
    t('Created by'),
    t('Issuer bank ID'),
    t('Gender'),
    t('First name'),
    t('Name'),
    t('Bank name'),
    t('IBAN'),
    t('Remote ID'),
    t('Status'),
    t('Log message'),
    t('All'),
    t('Page'),
  );
  $export[$view->name] = $view;

  return $export;
}

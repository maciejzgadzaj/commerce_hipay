<?php

/**
 * Returns Hipay bank account-related menu items.
 *
 * @see commerce_hipay_ws_menu()
 */
function commerce_hipay_ws_bank_account_menu() {
  $items = array();

  $items['admin/commerce/hipay-wallet/bank-accounts/add'] = array(
    'title' => 'Add a bank account',
    'page callback' => 'commerce_hipay_ws_bank_account_add',
    'access callback' => 'commerce_hipay_ws_bank_account_access',
    'access arguments' => array('create'),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account'] = array(
    'title callback' => 'commerce_hipay_ws_bank_account_title',
    'title arguments' => array(4),
    'page callback' => 'commerce_hipay_ws_bank_account_view',
    'page arguments' => array(4),
    'access callback' => 'commerce_hipay_ws_bank_account_access',
    'access arguments' => array('view', 4),
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account/view'] = array(
    'title' => 'view',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account/view/entity'] = array(
    'title' => 'local',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account/view/hipay'] = array(
    'title' => 'Hipay Wallet',
    'page callback' => 'commerce_hipay_ws_bank_account_view_hipay_info',
    'page arguments' => array(4),
    'access callback' => 'commerce_hipay_ws_bank_account_access',
    'access arguments' => array('view', 4),
    'type' => MENU_LOCAL_TASK,
    'weight' => -5,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account/view/payload'] = array(
    'title' => 'payload',
    'page callback' => 'commerce_hipay_ws_bank_account_view_payload_info',
    'page arguments' => array(4),
    'access arguments' => array('access Hipay Wallet API calls payload'),
    'type' => MENU_LOCAL_TASK,
    'weight' => -2,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account/edit'] = array(
    'title' => 'edit',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_ws_bank_account_form', 4),
    'access callback' => 'commerce_hipay_ws_bank_account_access',
    'access arguments' => array('edit', 4),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account/delete'] = array(
    'title' => 'delete',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_ws_bank_account_delete_form', 4),
    'access callback' => 'commerce_hipay_ws_bank_account_access',
    'access arguments' => array('delete', 4),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  return $items;
}

/**
 * Checks the current user's access to the specified bank account operation.
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
function commerce_hipay_ws_bank_account_access($op, $entity = NULL, $account = NULL) {
  switch ($op) {
    case 'create':
      return user_access('create commerce_hipay_ws_bank_account entities');
    case 'view':
      return user_access('view any commerce_hipay_ws_bank_account entity');
    case 'edit':
      return user_access('edit any commerce_hipay_ws_bank_account entity');
    case 'delete':
      return user_access('administer commerce_hipay_ws_bank_account entities');
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
 * @see commerce_hipay_ws_bank_account_menu()
 */
function commerce_hipay_ws_bank_account_view($entity, $view_mode = 'full') {
  return entity_view('commerce_hipay_ws_bank_account', array($entity->bank_account_id => $entity), $view_mode, NULL, TRUE);
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
 * @see commerce_hipay_ws_bank_account_menu()
 */
function commerce_hipay_ws_bank_account_view_payload_info($entity, $view_mode = 'full') {
  $markup = '';

  if (!empty($entity->data['payload'])) {
    $markup = '<pre>' . check_plain(print_r($entity->data['payload'], TRUE)) . '</pre>';
  }

  return $markup;
}

/**
 * Menu callback: fetches and displays Hipay Bank Account information.
 *
 * @param object $entity
 *   An entity to render.
 * @param string $view_mode
 *   A view mode as used by this entity type, e.g. 'full', 'teaser'...
 *
 * @return mixed
 *   A renderable array, or a rendered HTML.
 *
 * @see commerce_hipay_ws_bank_account_menu()
 */
function commerce_hipay_ws_bank_account_view_hipay_info($entity, $view_mode = 'full') {
  $return = '';

  $wrapper = entity_metadata_wrapper('commerce_hipay_ws_bank_account', $entity);
  $hipay_user_account = $wrapper->hipay_ws_bank_user_account->value();

  if (!empty($hipay_user_account->hipay_account_id)) {
    // Fetch: bankInfosStatus.
    $parameters = array('wsSubAccountId' => $hipay_user_account->hipay_account_id);
    $context = array('commerce_hipay_ws_bank_account' => $entity);
    $response = commerce_hipay_ws_api_bank_account_get_info_status($parameters, $context);

    if ($response['status'] == COMMERCE_HIPAY_WS_ACCOUNT_NO_BANK_INFO) {
      $return = drupal_get_form('commerce_hipay_ws_bank_account_view_hipay_create_form', $entity);
    }
    else {
      // Display: bankInfosStatus.
      if (!empty($response['status'])) {
        $return .= '<p><strong>bankInfosStatus</strong></p>';
        $return .= check_plain($response['status']);
      }

      // Fetch: bankInfosCheck.
      $parameters = array('wsSubAccountId' => $hipay_user_account->hipay_account_id);
      $context = array('commerce_hipay_ws_bank_account' => $entity);
      $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_CHECK, $parameters, NULL, $context);

      // Display: bankInfosCheck.
      if (isset($response['code'])) {
        $return .= '<p><strong>bankInfosCheck</strong></p>';
        if ($response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
          $exclude = array('code', 'description');
          $header = $row = array();
          foreach (array_diff_key($response, array_flip($exclude)) as $key => $value) {
            $header[$key] = check_plain($key);
            $row[$key] = check_plain($value);
          }
          $return .= theme('table', array(
            'header' => $header,
            'rows' => array($row)
          ));
        }
        else {
          $return .= check_plain($response['description']);
        }
      }

      return $return;
    }
  }
  else {
    $uri = entity_uri('commerce_hipay_ws_user_account', $hipay_user_account);
    $return .= t('Parent user account has not yet been created in Hipay Wallet. You can create it from <a href="@url">this page</a>.', array(
      '@url' => url($uri['path'] . '/view/hipay', array('query' => drupal_get_destination())),
    ));
  }

  return $return;
}

/**
 * Returns a form to create a bank account in Hipay Wallet.
 *
 * @see commerce_hipay_ws_bank_account_view_hipay_info()
 * @see commerce_hipay_ws_bank_account_view_hipay_create_form_submit()
 */
function commerce_hipay_ws_bank_account_view_hipay_create_form($form, &$form_state, $entity) {
  $form_state['commerce_hipay_ws_bank_account'] = $entity;

  $form['info'] = array(
    '#markup' => '<p>' . t('The bank account has not yet been created for this entity in Hipay Wallet.') . '</p>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create bank account in Hipay Wallet'),
  );

  return $form;
}

/**
 * Form submit handler: creates a bank account in Hipay Wallet.
 *
 * @see commerce_hipay_ws_bank_account_view_hipay_create_form()
 */
function commerce_hipay_ws_bank_account_view_hipay_create_form_submit($form, $form_state) {
  $bank_account = $form_state['commerce_hipay_ws_bank_account'];

  $response = commerce_hipay_ws_api_bank_account_create($bank_account);

  if ($response['code'] !== COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    drupal_set_message(t('Error creating a new bank account in Hipay Wallet: @error.', array('@error' => $response['description'])), 'error');
  }
}

/**
 * Provides a wrapper on the edit form to add a new Hipay bank account entity.
 *
 * @see commerce_hipay_ws_bank_account_menu()
 */
function commerce_hipay_ws_bank_account_add() {
  // Create a basic entity structure to be used and passed to the validation
  // and submission functions.
  $bank_account = entity_get_controller('commerce_hipay_ws_bank_account')->create();
  return drupal_get_form('commerce_hipay_ws_bank_account_form', $bank_account);
}

/**
 * Form function to edit a Hipay bank account entity.
 *
 * @see commerce_hipay_ws_bank_account_form_validate()
 * @see commerce_hipay_ws_bank_account_form_submit()
 */
function commerce_hipay_ws_bank_account_form($form, &$form_state, $bank_account) {
  global $user;

  // Ensure the owner name is accessible if the uid is set.
  if (isset($bank_account->uid) && $owner = user_load($bank_account->uid)) {
    $bank_account->name = $owner->name;
  }

  if (empty($bank_account->created)) {
    $bank_account->date = format_date(REQUEST_TIME, 'custom', 'Y-m-d H:i:s O');
  }

  $form['commerce_hipay_ws_bank_account'] = array(
    '#type' => 'value',
    '#value' => $bank_account,
  );

  // Add the field related form elements.
  field_attach_form('commerce_hipay_ws_bank_account', $bank_account, $form, $form_state);

  // Bank address country must be required, so that we can call Hipay API
  // to check which other bank fields are required for this country.
  $form['hipay_ws_bank_address'][LANGUAGE_NONE][0]['country']['#required'] = TRUE;

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

  // Add a log checkbox and timestamp field to a Revision information tab.
  $form['revision_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Revision information', array()),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'additional_settings',
    '#weight' => 40,
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'commerce_hipay_ws') . '/includes/entities/commerce_hipay_ws.entity_ui.js'),
    ),
  );
  $form['revision_information']['revision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create new revision'),
    '#description' => t('If a revision log message is entered, a revision will be created even if this is unchecked.'),
    '#default_value' => empty($bank_account->is_new),
    '#access' => user_access('administer commerce_hipay_ws_bank_account entities'),
  );
  $form['revision_information']['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Created on'),
    '#description' => t('Format: %time. The date format is YYYY-MM-DD and %timezone is the time zone offset from UTC. Leave blank to use the time of form submission.', array(
      '%time' => !empty($bank_account->date) ? date_format(date_create($bank_account->date), 'Y-m-d H:i:s O') : format_date($bank_account->created, 'custom', 'Y-m-d H:i:s O'),
      '%timezone' => !empty($bank_account->date) ? date_format(date_create($bank_account->date), 'O') : format_date($bank_account->created, 'custom', 'O'),
    )),
    '#maxlength' => 25,
    '#default_value' => !empty($bank_account->created) ? format_date($bank_account->created, 'custom', 'Y-m-d H:i:s O') : '',
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
    '#submit' => array('commerce_hipay_ws_bank_account_delete'),
    '#weight' => 200,
  );

  return $form;
}

/**
 * Validation handler for Hipay bank account edit form.
 *
 * @see commerce_hipay_ws_bank_account_form()
 * @see commerce_hipay_ws_bank_account_form_submit()
 */
function commerce_hipay_ws_bank_account_form_validate($form, &$form_state) {
  $bank_account = $form_state['values']['commerce_hipay_ws_bank_account'];

  // Validate that the bank account for this user account does not exist yet.
  if (
    ($existing_bank_accounts = commerce_hipay_ws_user_account_load_bank_accounts($form_state['values']['hipay_ws_bank_user_account'][LANGUAGE_NONE][0]['target_id']))
    && !in_array($bank_account->bank_account_id, array_keys($existing_bank_accounts))
  ) {
    form_set_error('hipay_ws_bank_user_account', t('A bank account for the selected user account already exists.'));
  }

  // Validate other required fields based on the country the bank account
  // is to be created in.
  if (!$response = commerce_hipay_ws_api_bank_account_get_info_fields($form_state['values']['hipay_ws_bank_address'][LANGUAGE_NONE][0]['country'])) {
    form_set_error('hipay_ws_bank_address[country', t('Error fetching required bank fields for country !country from Hipay API.', array(
      '!country' => $form_state['values']['hipay_ws_bank_address'][LANGUAGE_NONE][0]['country'],
    )));
  }
  if (!empty($response['fields']['item'])) {
    // If only the bank country element is required, the required element
    // returned in the response will not be an array - so we need to convert
    // it into the array ourselves.
    if (!empty($response['fields']['item']['code'])) {
      $response['fields']['item'] = array($response['fields']['item']);
    }
    // Mapping between Hipay API elements and our elements.
    $field_mapping = commerce_hipay_ws_bank_account_get_field_mapping();
    // Validate we have all required values.
    foreach ($response['fields']['item'] as $item) {
      $required_element = $field_mapping[$item['code']];
      if (empty($form_state['values'][$required_element][LANGUAGE_NONE][0]['value']) && empty($form_state['values']['hipay_ws_bank_address'][LANGUAGE_NONE][0][$required_element])) {
        // Get field title for displaying from the relevant form element.
        $field_title = (!empty($form[$required_element][LANGUAGE_NONE][0]['#title'])) ? $form[$required_element][LANGUAGE_NONE][0]['#title'] : $required_element;

        // We need custom handling for the address fields - as form elements
        // are nested deeper in the structure.
        // @TODO: Nice $field_title value.
        if (isset($form_state['values']['hipay_ws_bank_address'][LANGUAGE_NONE][0][$required_element])) {
          $required_element = 'hipay_ws_bank_address][und][0][' . $required_element;
        }

        form_set_error($required_element, t('!field_title field is required for country !country.', array(
          '!field_title' => $field_title,
          '!country' => $form_state['values']['hipay_ws_bank_address'][LANGUAGE_NONE][0]['country'],
        )));
      }
    }
  }

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
  field_attach_form_validate('commerce_hipay_ws_bank_account', $form_state['values']['commerce_hipay_ws_bank_account'], $form, $form_state);
}

/**
 * Submission handler for Hipay bank account edit form.
 *
 * @see commerce_hipay_ws_bank_account_form()
 * @see commerce_hipay_ws_bank_account_form_validate()
 */
function commerce_hipay_ws_bank_account_form_submit($form, &$form_state) {
  $bank_account = $form_state['values']['commerce_hipay_ws_bank_account'];

  if ($form_state['values']['revision'] || !empty($form_state['values']['log'])) {
    $bank_account->revision = TRUE;
    $bank_account->log = $form_state['values']['log'];
  }

  // Set the profile's owner uid based on the supplied name.
  if (!empty($form_state['values']['name']) && $account = user_load_by_name($form_state['values']['name'])) {
    $bank_account->uid = $account->uid;
  }
  else {
    $bank_account->uid = 0;
  }

  $bank_account->created = !empty($form_state['values']['date']) ? strtotime($form_state['values']['date']) : REQUEST_TIME;

  // Notify field widgets.
  field_attach_submit('commerce_hipay_ws_bank_account', $bank_account, $form, $form_state);

  // Save the Hipay bank account.
  commerce_hipay_ws_bank_account_save($bank_account);

  drupal_set_message(t('Hipay bank account saved.'));
  $form_state['redirect'] = 'admin/commerce/hipay-wallet/bank-accounts/' . $bank_account->bank_account_id;
}

/**
 * Menu callback: returns Hipay bank account deletion form.
 *
 * @see commerce_hipay_ws_bank_account_menu()
 */
function commerce_hipay_ws_bank_account_delete($form, &$form_state) {
  return drupal_get_form('commerce_hipay_ws_bank_account_delete_form', $form_state['values']['commerce_hipay_ws_bank_account']);
}

/**
 * Form constructor for delete an accounting reference form.
 *
 * @see commerce_hipay_ws_bank_account_delete_form_submit()
 */
function commerce_hipay_ws_bank_account_delete_form($form, &$form_state, $bank_account) {
  $form_state['commerce_hipay_ws_bank_account'] = $bank_account;

  $form = confirm_form($form,
    t('Are you sure you want to delete this bank account?'),
    'admin/commerce/hipay-wallet/bank-accounts',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submission handler for Hipay bank account delete form.
 *
 * @see commerce_hipay_ws_bank_account_delete_form()
 */
function commerce_hipay_ws_bank_account_delete_form_submit($form, &$form_state) {
  $form_state['commerce_hipay_ws_bank_account']->delete();

  drupal_set_message(t('The bank account has been deleted.'));
  $form_state['redirect'] = 'admin/commerce/hipay-wallet/bank-accounts';
}

/**
 * Returns mapping between Hipay bank API parameters and Drupal field names.
 *
 * @return array
 *   The mapping between Hipay bank API parameters and Drupal field names.
 */
function commerce_hipay_ws_bank_account_get_field_mapping() {
  return array(
    'bankName' => 'hipay_ws_bank_name',
    'bankAddress' => 'thoroughfare',
    'bankZipCode' => 'postal_code',
    'bankCity' => 'locality',
    'bankCountry' => 'country',
    'iban' => 'hipay_ws_bank_iban',
    'swift' => 'hipay_ws_bank_swift',
    'acct_num' => 'hipay_ws_bank_account_number',
    'aba_num' => 'hipay_ws_bank_aba_number',
    'transit_num' => 'hipay_ws_bank_transit_number',
  );
}

/**
 * Provides Views data structure for the bank account entities.
 *
 * @see commerce_hipay_ws_views_data_alter()
 */
function commerce_hipay_ws_bank_account_views_data_alter(&$data) {
  // Bank accounts.
  $data['commerce_hipay_ws_bank_account']['link'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the bank account.'),
      'handler' => 'commerce_hipay_ws_bank_account_handler_link_field',
    ),
  );
  $data['commerce_hipay_ws_bank_account']['edit'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a link to the edit form for the bank account.'),
      'handler' => 'commerce_hipay_ws_bank_account_handler_edit_link_field',
    ),
  );
  $data['commerce_hipay_ws_bank_account']['delete'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a link to delete the bank account.'),
      'handler' => 'commerce_hipay_ws_bank_account_handler_delete_link_field',
    ),
  );
  $data['commerce_hipay_ws_bank_account']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this bank account.'),
      'handler' => 'commerce_hipay_ws_bank_account_handler_operations_field',
    ),
  );
  $data['commerce_hipay_ws_bank_account']['hipay_account_id']['field']['handler'] = 'commerce_hipay_ws_handler_link_account_id';

  // Bank account revisions.
  $data['commerce_hipay_ws_bank_account_revision']['revision_id'] = array(
    'title' => t('Revision ID'),
    'help' => t('The revision ID of the Hipay bank account revision.'),
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
      'base' => 'commerce_hipay_ws_bank_account',
      'base field' => 'revision_id',
      'title' => t('Hipay bank account'),
      'label' => t('Latest bank account revision'),
    ),
  );
  $data['commerce_hipay_ws_bank_account_revision']['revision_uid'] = array(
    'title' => t('User'),
    'help' => t('Relate a bank account revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'revision_uid',
      'field_name' => 'revision_uid',
      'label' => t('Revision user'),
    ),
  );
  $data['commerce_hipay_ws_bank_account_revision']['log'] = array(
    'title' => t('Log message'),
    'help' => t('The log message entered when the revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['commerce_hipay_ws_bank_account_revision']['revision_timestamp'] = array(
    'title' => t('Revision date'),
    'help' => t('The date the Hipay bank account revision was created.'),
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
 * Provides default views for the bank account entities.
 *
 * @see commerce_hipay_ws_views_default_views()
 */
function commerce_hipay_ws_bank_account_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'hipay_ws_bank_accounts';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_ws_bank_account';
  $view->human_name = 'Hipay Wallet: Bank accounts';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Bank accounts';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit own commerce_hipay_ws_bank_account entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'bank_account_id' => 'bank_account_id',
    'hipay_account_id' => 'hipay_account_id',
    'rendered_entity' => 'rendered_entity',
    'rendered_entity_1' => 'rendered_entity_1',
    'hipay_ws_user_currency' => 'hipay_ws_user_currency',
    'hipay_status' => 'hipay_status',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = 'bank_account_id';
  $handler->display->display_options['style_options']['info'] = array(
    'bank_account_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hipay_account_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rendered_entity' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rendered_entity_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hipay_ws_user_currency' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hipay_status' => array(
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
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['hipay_ws_bank_user_account_target_id']['id'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['relationships']['hipay_ws_bank_user_account_target_id']['table'] = 'field_data_hipay_ws_bank_user_account';
  $handler->display->display_options['relationships']['hipay_ws_bank_user_account_target_id']['field'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['relationships']['hipay_ws_bank_user_account_target_id']['required'] = TRUE;
  /* Field: Hipay Wallet bank account: Bank account ID */
  $handler->display->display_options['fields']['bank_account_id']['id'] = 'bank_account_id';
  $handler->display->display_options['fields']['bank_account_id']['table'] = 'commerce_hipay_ws_bank_account';
  $handler->display->display_options['fields']['bank_account_id']['field'] = 'bank_account_id';
  $handler->display->display_options['fields']['bank_account_id']['label'] = 'ID';
  /* Field: Hipay Wallet user account: Hipay user account ID */
  $handler->display->display_options['fields']['hipay_account_id']['id'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['hipay_account_id']['field'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['relationship'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['fields']['hipay_account_id']['link_to_entity'] = TRUE;
  /* Field: Hipay Wallet user account: Rendered Hipay Wallet user account */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['relationship'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Hipay user account';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'full';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  /* Field: Hipay Wallet bank account: Rendered Hipay Wallet bank account */
  $handler->display->display_options['fields']['rendered_entity_1']['id'] = 'rendered_entity_1';
  $handler->display->display_options['fields']['rendered_entity_1']['table'] = 'views_entity_commerce_hipay_ws_bank_account';
  $handler->display->display_options['fields']['rendered_entity_1']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity_1']['label'] = 'Bank';
  $handler->display->display_options['fields']['rendered_entity_1']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['view_mode'] = 'full';
  $handler->display->display_options['fields']['rendered_entity_1']['bypass_access'] = 0;
  /* Field: Hipay Wallet user account: Currency */
  $handler->display->display_options['fields']['hipay_ws_user_currency']['id'] = 'hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['table'] = 'field_data_hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['field'] = 'hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['relationship'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_currency']['field_api_classes'] = TRUE;
  /* Field: Hipay Wallet bank account: Hipay account status */
  $handler->display->display_options['fields']['hipay_status']['id'] = 'hipay_status';
  $handler->display->display_options['fields']['hipay_status']['table'] = 'commerce_hipay_ws_bank_account';
  $handler->display->display_options['fields']['hipay_status']['field'] = 'hipay_status';
  $handler->display->display_options['fields']['hipay_status']['label'] = 'Status';
  /* Field: Hipay Wallet bank account: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_hipay_ws_bank_account';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = '';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;

  /* Display: Page: Admin */
  $handler = $view->new_display('page', 'Page: Admin', 'page_bank_accounts_admin');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-wallet/bank-accounts/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['description'] = 'Find and manage Hipay Wallet bank accounts.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Bank accounts';
  $handler->display->display_options['tab_options']['description'] = 'Manage Hipay Wallet bank accounts.';
  $handler->display->display_options['tab_options']['weight'] = '20';
  $handler->display->display_options['tab_options']['name'] = 'management';

  /* Display: Page: Store */
  $handler = $view->new_display('page', 'Page: Store', 'page_bank_accounts_store');
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'The Hipay Wallet bank account has not yet been created for this store.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Hipay Wallet user account: Store (hipay_ws_user_store) */
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['id'] = 'hipay_ws_user_store_target_id';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['table'] = 'field_data_hipay_ws_user_store';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['field'] = 'hipay_ws_user_store_target_id';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['relationship'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'store/%/hipay-wallet/bank-accounts';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Bank accounts';
  $handler->display->display_options['menu']['description'] = 'Manage your Hipay Wallet bank accounts.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['hipay_ws_bank_accounts'] = array(
    t('Master'),
    t('Bank accounts'),
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
    t('Hipay Wallet user account entity referenced from hipay_ws_bank_user_account'),
    t('ID'),
    t('.'),
    t(','),
    t('Hipay user account ID'),
    t('Hipay user account'),
    t('Bank'),
    t('Currency'),
    t('Status'),
    t('Page: Admin'),
    t('Page: Store'),
    t('The Hipay Wallet bank account has not yet been created for this store.'),
    t('All'),
  );
  $export[$view->name] = $view;

  $view = new view();
  $view->name = 'hipay_ws_bank_account_revisions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_ws_bank_account_revision';
  $view->human_name = 'Hipay Wallet: Bank account revisions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Revisions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer commerce_hipay_ws_bank_account entities';
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
    'hipay_account_id' => 'hipay_account_id',
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
    'hipay_account_id' => array(
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
  /* Relationship: Hipay Wallet bank account Revisions: User */
  $handler->display->display_options['relationships']['revision_uid']['id'] = 'revision_uid';
  $handler->display->display_options['relationships']['revision_uid']['table'] = 'commerce_hipay_ws_bank_account_revision';
  $handler->display->display_options['relationships']['revision_uid']['field'] = 'revision_uid';
  $handler->display->display_options['relationships']['revision_uid']['required'] = TRUE;
  /* Field: Hipay Wallet bank account Revisions: Revision ID */
  $handler->display->display_options['fields']['revision_id']['id'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['table'] = 'commerce_hipay_ws_bank_account_revision';
  $handler->display->display_options['fields']['revision_id']['field'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['label'] = 'Revision';
  /* Field: Hipay Wallet bank account Revisions: Revision date */
  $handler->display->display_options['fields']['revision_timestamp']['id'] = 'revision_timestamp';
  $handler->display->display_options['fields']['revision_timestamp']['table'] = 'commerce_hipay_ws_bank_account_revision';
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
  /* Field: Hipay Wallet bank account Revisions: Related Hipay user account ID */
  $handler->display->display_options['fields']['hipay_account_id']['id'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['table'] = 'commerce_hipay_ws_bank_account_revision';
  $handler->display->display_options['fields']['hipay_account_id']['field'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['label'] = 'Hipay user account ID';
  /* Field: Hipay Wallet bank account Revisions: Hipay account status */
  $handler->display->display_options['fields']['hipay_status']['id'] = 'hipay_status';
  $handler->display->display_options['fields']['hipay_status']['table'] = 'commerce_hipay_ws_bank_account_revision';
  $handler->display->display_options['fields']['hipay_status']['field'] = 'hipay_status';
  $handler->display->display_options['fields']['hipay_status']['label'] = 'Status';
  /* Field: Hipay Wallet bank account Revisions: Log message */
  $handler->display->display_options['fields']['log']['id'] = 'log';
  $handler->display->display_options['fields']['log']['table'] = 'commerce_hipay_ws_bank_account_revision';
  $handler->display->display_options['fields']['log']['field'] = 'log';
  /* Contextual filter: Hipay Wallet bank account Revisions: Bank account ID */
  $handler->display->display_options['arguments']['bank_account_id']['id'] = 'bank_account_id';
  $handler->display->display_options['arguments']['bank_account_id']['table'] = 'commerce_hipay_ws_bank_account_revision';
  $handler->display->display_options['arguments']['bank_account_id']['field'] = 'bank_account_id';
  $handler->display->display_options['arguments']['bank_account_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['bank_account_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['bank_account_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['bank_account_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['bank_account_id']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-wallet/bank-accounts/%/revisions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'revisions';
  $handler->display->display_options['menu']['description'] = 'View revisions of this Hipay bank account.';
  $handler->display->display_options['menu']['weight'] = '20';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['hipay_ws_bank_account_revisions'] = array(
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
    t('Hipay user account ID'),
    t('Status'),
    t('Log message'),
    t('All'),
    t('Page'),
  );
  $export[$view->name] = $view;

  return $export;
}

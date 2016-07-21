<?php

/**
 * Returns Hipay user account-related menu items.
 *
 * @see commerce_hipay_ws_menu()
 */
function commerce_hipay_ws_user_account_menu() {
  $items = array();

  $items['admin/commerce/hipay-wallet/user-accounts/add'] = array(
    'title' => 'Add a user account',
    'page callback' => 'commerce_hipay_ws_user_account_add',
    'page arguments' => array('hipay_user_account'),
    'access callback' => 'commerce_hipay_ws_user_account_access',
    'access arguments' => array('create'),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-accounts/%commerce_hipay_ws_user_account'] = array(
    'title callback' => 'commerce_hipay_ws_user_account_title',
    'title arguments' => array(4),
    'page callback' => 'commerce_hipay_ws_user_account_view',
    'page arguments' => array(4),
    'access callback' => 'commerce_hipay_ws_user_account_access',
    'access arguments' => array('view', 4),
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-accounts/%commerce_hipay_ws_user_account/view'] = array(
    'title' => 'view',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-accounts/%commerce_hipay_ws_user_account/view/entity'] = array(
    'title' => 'local',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-accounts/%commerce_hipay_ws_user_account/view/hipay'] = array(
    'title' => 'Hipay Wallet',
    'page callback' => 'commerce_hipay_ws_user_account_view_hipay_info',
    'page arguments' => array(4),
    'access callback' => 'commerce_hipay_ws_user_account_access',
    'access arguments' => array('view', 4),
    'type' => MENU_LOCAL_TASK,
    'weight' => -5,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-accounts/%commerce_hipay_ws_user_account/edit'] = array(
    'title' => 'edit',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_ws_user_account_form', 4),
    'access callback' => 'commerce_hipay_ws_user_account_access',
    'access arguments' => array('edit', 4),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-accounts/%commerce_hipay_ws_user_account/delete'] = array(
    'title' => 'delete',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_ws_user_account_delete_form', 4, 'account'),
    'access callback' => 'commerce_hipay_ws_user_account_access',
    'access arguments' => array('delete', 4),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  // For Hipay user sub-account we use the same page callbacks, in some cases
  // just providing additional parameter saying that we are dealing with
  // a user sub-account.
  $items['admin/commerce/hipay-wallet/user-subaccounts/add'] = array(
    'title' => 'Add a user sub-account',
    'page callback' => 'commerce_hipay_ws_user_account_add',
    'page arguments' => array('hipay_user_subaccount'),
    'access callback' => 'commerce_hipay_ws_user_account_access',
    'access arguments' => array('create'),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-subaccounts/%commerce_hipay_ws_user_account'] = array(
    'title callback' => 'commerce_hipay_ws_user_account_title',
    'title arguments' => array(4),
    'page callback' => 'commerce_hipay_ws_user_account_view',
    'page arguments' => array(4),
    'access callback' => 'commerce_hipay_ws_user_account_access',
    'access arguments' => array('view', 4),
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-subaccounts/%commerce_hipay_ws_user_account/view'] = array(
    'title' => 'view',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-subaccounts/%commerce_hipay_ws_user_account/edit'] = array(
    'title' => 'edit',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_ws_user_account_form', 4),
    'access callback' => 'commerce_hipay_ws_user_account_access',
    'access arguments' => array('edit', 4),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-subaccounts/%commerce_hipay_ws_user_account/delete'] = array(
    'title' => 'delete',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_ws_user_account_delete_form', 4, 'sub-account'),
    'access callback' => 'commerce_hipay_ws_user_account_access',
    'access arguments' => array('delete', 4),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  return $items;
}

/**
 * Checks the current user's access to the specified user account operation.
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
function commerce_hipay_ws_user_account_access($op, $entity = NULL, $account = NULL) {
  switch ($op) {
    case 'create':
      return user_access('create commerce_hipay_ws_user_account entities');
    case 'view':
      return user_access('view any commerce_hipay_ws_user_account entity');
    case 'edit':
      return user_access('edit any commerce_hipay_ws_user_account entity');
    case 'delete':
      return user_access('administer commerce_hipay_ws_user_account entities');
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
 * @see commerce_hipay_ws_user_account_menu()
 */
function commerce_hipay_ws_user_account_view($entity, $view_mode = 'full') {
  return entity_view('commerce_hipay_ws_user_account', array($entity->user_account_id => $entity), $view_mode, NULL, TRUE);
}

/**
 * Menu callback: fetches and displays Hipay User Account information.
 *
 * @param object $entity
 *   An entity to render.
 * @param string $view_mode
 *   A view mode as used by this entity type, e.g. 'full', 'teaser'...
 *
 * @return mixed
 *   A renderable array, or a rendered HTML.
 *
 * @see commerce_hipay_ws_user_account_menu()
 */
function commerce_hipay_ws_user_account_view_hipay_info($entity, $view_mode = 'full') {
  $return = array(
    'hipay_user_account_title' => array(
      '#markup' => '<h2>' . t('Hipay user account') . '</h2>',
    ),
  );

  if (!empty($entity->hipay_account_id)) {
    $output = '';

    // Fetch: getAccountInfos.
    $context = array('commerce_hipay_ws_user_account' => $entity);
    $response = commerce_hipay_ws_api_user_account_get_infos($entity->hipay_account_id, $context);
    if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_WS_STATUS_SUCCESS) {

      // Display: getAccountInfos.
      $output .= '<h3>' . t('getAccountInfos') . '</h3>';
      $exclude = array('code', 'description', 'websites', 'subAccounts');
      $header = $row = array();
      foreach (array_diff_key($response, array_flip($exclude)) as $key => $value) {
        $header[$key] = check_plain($key);
        $row[$key] = check_plain($value);
      }
      $output .= theme('table', array(
        'header' => $header,
        'rows' => array($row)
      ));

      // Display: getAccountInfos: websites.
      if (!empty($response['websites'])) {
        $output .= '<h3>' . t('getAccountInfos: websites') . '</h3>';
        $header = $rows = array();
        foreach ($response['websites'] as $item) {
          $row = array();
          foreach ($item as $key => $value) {
            $header[$key] = check_plain($key);
            $row[$key] = check_plain($value);
          }
          $rows[] = $row;
        }
        $output .= theme('table', array(
          'header' => $header,
          'rows' => array($row)
        ));
      }

      // @TODO: Display: getAccountInfos: subAccounts.
      if (!empty($response['subAccounts'])) {
        $output .= '<h3>' . t('getAccountInfos: subAccounts') . '</h3>';
      }
    }

    // Fetch: getBalance.
    $parameters = array('wsSubAccountId' => $entity->hipay_account_id);
    $context = array('commerce_hipay_ws_user_account' => $entity);
    $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_BALANCE, $parameters, NULL, $context);

    // Display: getBalance.
    if (!empty($response['balances'])) {
      $output .= '<h3>' . t('getBalance') . '</h3>';
      $header = $rows = array();
      foreach ($response['balances'] as $item) {
        $row = array();
        foreach ($item as $key => $value) {
          $header[$key] = check_plain($key);
          $row[$key] = check_plain($value);
        }
        $rows[] = $row;
      }
      $output .= theme('table', array('header' => $header, 'rows' => array($row)));
    }

    // Fetch: getTransactions.
    $parameters = array(
      'wsSubAccountId' => $entity->hipay_account_id,
      'startDate' => date('Y-m-d H:i:s', $entity->created),
      'endDate' => date('Y-m-d H:i:s', REQUEST_TIME),
      'pageNumber' => 1,
    );
    $context = array('commerce_hipay_ws_user_account' => $entity);
    $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_TRANSACTIONS, $parameters, NULL, $context);

    // Display: getTransactions.
    if (!empty($response['transactions']['item'])) {
      // If there is only one transaction, 'item', we need to put it into
      // a sub-array (as is the case with multiple transactions).
      if (!isset($response['transactions']['item'][0])) {
        $response['transactions']['item'] = array($response['transactions']['item']);
      }

      $output .= '<h3>' . t('getTransactions') . '</h3>';
      $header = $rows = array();
      foreach ($response['transactions']['item'] as $item) {
        $row = array();
        foreach ($item as $key => $value) {
          $header[$key] = check_plain($key);
          $row[$key] = check_plain($value);
        }
        $rows[] = $row;
      }
      $output .= theme('table', array('header' => $header, 'rows' => $rows));
    }

    $return['hipay_user_account'] = array(
      '#markup' => $output,
    );
  }
  else {
    $return['hipay_user_account'] = drupal_get_form('commerce_hipay_ws_user_account_view_hipay_create_form', $entity);
  }

  // KYC documents.
  $wrapper = entity_metadata_wrapper('commerce_hipay_ws_user_account', $entity);
  $kyc_documents = $wrapper->hipay_ws_user_kyc_documents->value();

  if (!empty($kyc_documents)) {
    $return['kyc_documents_title'] = array(
      '#markup' => '<h2>' . t('KYC documents') . '</h2>',
    );

    $return['kyc_documents'] = array(
      '#theme' => 'kyc_documents_table',
      '#items' => $kyc_documents,
    );

    // Check if all KYC documents were uploaded to Hipay Wallet,
    // and display an upload form if there is still something left.
    $all_documents_uploaded = TRUE;
    foreach ($kyc_documents as $kyc_document) {
      if (empty($kyc_document['kyc_uploaded'])) {
        $all_documents_uploaded = FALSE;
      }
    }
    if (!$all_documents_uploaded) {
      $return['kyc_documents_upload'] = drupal_get_form('commerce_hipay_ws_user_account_view_hipay_kyc_upload_form', $entity);
    }
  }

  return $return;
}

/**
 * Returns a form to create a user account in Hipay Wallet.
 *
 * @see commerce_hipay_ws_user_account_view_hipay_info()
 * @see commerce_hipay_ws_user_account_view_hipay_create_form_submit()
 */
function commerce_hipay_ws_user_account_view_hipay_create_form($form, &$form_state, $entity) {
  $form_state['commerce_hipay_ws_user_account'] = $entity;

  $form['info'] = array(
    '#markup' => '<p>' . t('The user account has not yet been created for this entity in Hipay Wallet.') . '</p>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create user account in Hipay Wallet'),
  );

  return $form;
}

/**
 * Form submit handler: creates a user account in Hipay Wallet.
 *
 * @see commerce_hipay_ws_user_account_view_hipay_create_form()
 */
function commerce_hipay_ws_user_account_view_hipay_create_form_submit($form, $form_state) {
  $user_account = $form_state['commerce_hipay_ws_user_account'];

  $response = commerce_hipay_ws_api_user_account_create($user_account);

  if (!isset($response['code']) || $response['code'] !== COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
    drupal_set_message(t('Error creating a new user account in Hipay Wallet: @error.', array('@error' => $response['description'])), 'error');
  }
}

/**
 * Returns a form to upload KYC documents if not uploaded yet.
 *
 * @see commerce_hipay_ws_user_account_view_hipay_info()
 * @see commerce_hipay_ws_user_account_view_hipay_kyc_upload_form_submit()
 */
function commerce_hipay_ws_user_account_view_hipay_kyc_upload_form($form, &$form_state, $entity) {
  $form_state['commerce_hipay_ws_user_account'] = $entity;

  $form['info'] = array(
    '#markup' => '<p>' . t('Not all KYC documents have been uploaded to Hipay Wallet yet.') . '</p>',
  );

  // If user account has already been created in Hipay Wallet,
  // allow to upload the KYC documents.
  if (!empty($entity->hipay_account_id)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Upload remaining KYC documents to Hipay Wallet'),
    );
  }
  // Otherwise just display relevant info.
  else {
    $form['info_create'] = array(
      '#markup' => '<p>' . t('You have to create the user account in Hipay Wallet first before being able to upload the KYC documents.') . '</p>',
    );
  }

  return $form;
}

/**
 * Form submit handler: uploads KYC documents to Hipay Wallet.
 *
 * @see commerce_hipay_ws_user_account_view_hipay_kyc_upload_form()
 */
function commerce_hipay_ws_user_account_view_hipay_kyc_upload_form_submit($form, $form_state) {
  $user_account = $form_state['commerce_hipay_ws_user_account'];
  commerce_hipay_ws_user_account_upload_kyc_documents($user_account);
}

/**
 * Uploads all not yet uploaded entity KYC documents to Hipay Wallet.
 *
 * @param object $user_account
 *   A Hipay user account entity object which to upload the KYC documents for.
 */
function commerce_hipay_ws_user_account_upload_kyc_documents($user_account) {
  $wrapper = entity_metadata_wrapper('commerce_hipay_ws_user_account', $user_account);
  $kyc_documents = $wrapper->hipay_ws_user_kyc_documents->value();
  $success = TRUE;

  foreach ($kyc_documents as $delta => $kyc_document) {
    if (empty($kyc_document['kyc_uploaded'])) {
      $kyc_document['delta'] = $delta;
      $response = commerce_hipay_ws_api_upload($user_account, $kyc_document);

      // If there was an error uploading the KYC document.
      if (!isset($response['code']) || $response['code'] !== COMMERCE_HIPAY_WS_STATUS_SUCCESS) {
        // Combine all error messages into one string.
        $error_message = '';
        if (!empty($response['errors'])) {
          $error_messages = array();
          foreach ($response['errors'] as $error) {
            $error_messages[] = implode(': ', $error);
          }
          $error_message = ': ' . implode(', ', $error_messages);
        }

        drupal_set_message(t('Error uploading KYC document %filename to Hipay Wallet: @error.', array(
          '%filename' => $kyc_document['filename'],
          '@error' => $response['message'] . $error_message,
        )), 'error');
        $success = FALSE;
      }
      // Everything was fine with the upload.
      else {
        drupal_set_message(t('Successfully uploaded KYC document %filename to Hipay Wallet.', array(
          '%filename' => $kyc_document['filename'],
        )));
      }
    }
  }

  return $success;
}

/**
 * Provides a wrapper on the edit form to add a new Hipay user account entity.
 *
 * @see commerce_hipay_ws_user_account_menu()
 */
function commerce_hipay_ws_user_account_add($type) {
  // Create a basic entity structure to be used and passed to the validation
  // and submission functions.
  $user_account = entity_get_controller('commerce_hipay_ws_user_account')->create(array('type' => $type));
  return drupal_get_form('commerce_hipay_ws_user_account_form', $user_account);
}

/**
 * Form function to edit a Hipay user account entity.
 *
 * @see commerce_hipay_ws_user_account_form_validate()
 * @see commerce_hipay_ws_user_account_form_submit()
 */
function commerce_hipay_ws_user_account_form($form, &$form_state, $user_account) {
  global $user;

  // Ensure the owner name is accessible if the uid is set.
  if (isset($user_account->uid) && $owner = user_load($user_account->uid)) {
    $user_account->name = $owner->name;
  }

  if (empty($user_account->created)) {
    $user_account->date = format_date(REQUEST_TIME, 'custom', 'Y-m-d H:i:s O');
  }

  $form['commerce_hipay_ws_user_account'] = array(
    '#type' => 'value',
    '#value' => $user_account,
  );

  // Add the field related form elements.
  field_attach_form('commerce_hipay_ws_user_account', $user_account, $form, $form_state);

  // Add custom processing function to each uploaded file, so that we can
  // replace its description field with own KYC type dropdown.
  foreach (element_children($form['hipay_ws_user_kyc_documents'][LANGUAGE_NONE], TRUE) as $delta => $key) {
    $form['hipay_ws_user_kyc_documents'][LANGUAGE_NONE][$key]['#process'][] = 'commerce_hipay_ws_file_field_widget_process';
  }

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
    '#default_value' => isset($user_account->name) ? $user_account->name : $user->name,
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
    '#default_value' => empty($user_account->is_new),
    '#access' => user_access('administer commerce_hipay_ws_user_account entities'),
  );
  $form['revision_information']['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Created on'),
    '#description' => t('Format: %time. The date format is YYYY-MM-DD and %timezone is the time zone offset from UTC. Leave blank to use the time of form submission.', array(
      '%time' => !empty($user_account->date) ? date_format(date_create($user_account->date), 'Y-m-d H:i:s O') : format_date($user_account->created, 'custom', 'Y-m-d H:i:s O'),
      '%timezone' => !empty($user_account->date) ? date_format(date_create($user_account->date), 'O') : format_date($user_account->created, 'custom', 'O'),
    )),
    '#maxlength' => 25,
    '#default_value' => !empty($user_account->created) ? format_date($user_account->created, 'custom', 'Y-m-d H:i:s O') : '',
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
    '#submit' => array('commerce_hipay_ws_user_account_delete'),
    '#weight' => 200,
  );

  return $form;
}

/**
 * Validation handler for Hipay user account edit form.
 *
 * @see commerce_hipay_ws_user_account_form()
 * @see commerce_hipay_ws_user_account_form_submit()
 */
function commerce_hipay_ws_user_account_form_validate($form, &$form_state) {
  $user_account = $form_state['values']['commerce_hipay_ws_user_account'];

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
  field_attach_form_validate('commerce_hipay_ws_user_account', $form_state['values']['commerce_hipay_ws_user_account'], $form, $form_state);

  if ($user_account->type != 'hipay_user_account') {
    return;
  }

  // Validate the 'Email' address.
  if ($error = user_validate_mail($form_state['values']['hipay_ws_user_email'][LANGUAGE_NONE][0]['value'])) {
    form_set_error('hipay_ws_user_email', $error);
  }
  // Check if the email address is not taken by an existing Hipay user account.
  else {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'commerce_hipay_ws_user_account')
      ->fieldCondition('hipay_ws_user_email', 'value', $form_state['values']['hipay_ws_user_email'][LANGUAGE_NONE][0]['value'], '=');
    // If we are modifying an existing user account, do not check the form email
    // address against this account email address.
    if (!empty($user_account->user_account_id)) {
      $query->propertyCondition('user_account_id', $user_account->user_account_id, '<>');
    }
    $result = $query->execute();
    if (!empty($result['commerce_hipay_ws_user_account'])) {
      form_set_error('hipay_ws_user_email', t('The e-mail address %email is already registered.', array('%email' => $form_state['values']['hipay_ws_user_email'][LANGUAGE_NONE][0]['value'])));
    }
  }

  // Validate the 'Contact email' address.
  if ($error = user_validate_mail($form_state['values']['hipay_ws_user_contact_email'][LANGUAGE_NONE][0]['value'])) {
    form_set_error('hipay_ws_user_contact_email', $error);
  }
}

/**
 * Submission handler for Hipay user account edit form.
 *
 * @see commerce_hipay_ws_user_account_form()
 * @see commerce_hipay_ws_user_account_form_validate()
 */
function commerce_hipay_ws_user_account_form_submit($form, &$form_state) {
  $user_account = $form_state['values']['commerce_hipay_ws_user_account'];

  if ($form_state['values']['revision'] || !empty($form_state['values']['log'])) {
    $user_account->revision = TRUE;
    $user_account->log = $form_state['values']['log'];
  }

  // Set the profile's owner uid based on the supplied name.
  if (!empty($form_state['values']['name']) && $account = user_load_by_name($form_state['values']['name'])) {
    $user_account->uid = $account->uid;
  }
  else {
    $user_account->uid = 0;
  }

  $user_account->created = !empty($form_state['values']['date']) ? strtotime($form_state['values']['date']) : REQUEST_TIME;

  // Notify field widgets.
  field_attach_submit('commerce_hipay_ws_user_account', $user_account, $form, $form_state);

  // Save the Hipay user account.
  commerce_hipay_ws_user_account_save($user_account);

  drupal_set_message(t('Hipay user account saved.'));
  $form_state['redirect'] = 'admin/commerce/hipay-wallet/user-accounts/' . $user_account->user_account_id;
}

/**
 * Menu callback: returns Hipay user account deletion form.
 *
 * @see commerce_hipay_ws_user_account_menu()
 */
function commerce_hipay_ws_user_account_delete($form, &$form_state) {
  return drupal_get_form('commerce_hipay_ws_user_account_delete_form', $form_state['values']['commerce_hipay_ws_user_account']);
}

/**
 * Form constructor for delete an accounting reference form.
 *
 * @see commerce_hipay_ws_user_account_delete_form_submit()
 */
function commerce_hipay_ws_user_account_delete_form($form, &$form_state, $user_account, $type) {
  // Do not allow to delete a principal user account if it has a sub-account
  // connected to it.
  if (
    $user_account->type == 'hipay_user_account'
    && $subaccounts = commerce_hipay_ws_user_account_load_subaccounts($user_account->user_account_id)
  ) {
    $form['info'] = array(
      '#markup' => t('There are !count user sub-accounts connected to this user account. Please delete them first.', array(
        '!count' => count($subaccounts),
      )),
    );
    return $form;
  }

  // Do not allow to delete a user account if it has a bank account
  // connected to it.
  if (commerce_hipay_ws_user_account_load_bank_accounts($user_account->user_account_id)) {
    $form['info'] = array(
      '#markup' => t('There is a bank account connected to this user account. Please delete it first.'),
    );
    return $form;
  }

  $form_state['type'] = $type;
  $form_state['commerce_hipay_ws_user_account'] = $user_account;

  $form = confirm_form($form,
    t('Are you sure you want to delete this user !type?', array('!type' => $type)),
    'admin/commerce/hipay-wallet/user-accounts',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submission handler for Hipay user account delete form.
 *
 * @see commerce_hipay_ws_user_account_delete_form()
 */
function commerce_hipay_ws_user_account_delete_form_submit($form, &$form_state) {
  $form_state['commerce_hipay_ws_user_account']->delete();

  drupal_set_message(t('The user !type has been deleted.', array('!type' => $form_state['type'])));

  $account_path = ($form_state['commerce_hipay_ws_user_account']->type == 'hipay_user_subaccount') ? 'user-subaccounts' : 'user-accounts';
  $form_state['redirect'] = 'admin/commerce/hipay-wallet/' . $account_path;
}

/**
 * Provides Views data structure for the user account entities.
 *
 * @see commerce_hipay_ws_views_data_alter()
 */
function commerce_hipay_ws_user_account_views_data_alter(&$data) {
  // User accounts.
  $data['commerce_hipay_ws_user_account']['link'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the user account.'),
      'handler' => 'commerce_hipay_ws_user_account_handler_link_field',
    ),
  );
  $data['commerce_hipay_ws_user_account']['edit'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a link to the edit form for the user account.'),
      'handler' => 'commerce_hipay_ws_user_account_handler_edit_link_field',
    ),
  );
  $data['commerce_hipay_ws_user_account']['delete'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a link to delete the user account.'),
      'handler' => 'commerce_hipay_ws_user_account_handler_delete_link_field',
    ),
  );
  $data['commerce_hipay_ws_user_account']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this user account.'),
      'handler' => 'commerce_hipay_ws_user_account_handler_operations_field',
    ),
  );

  // User account revisions.
  $data['commerce_hipay_ws_user_account_revision']['revision_id'] = array(
    'title' => t('Revision ID'),
    'help' => t('The revision ID of the Hipay user account revision.'),
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
      'base' => 'commerce_hipay_ws_user_account',
      'base field' => 'revision_id',
      'title' => t('Hipay user account'),
      'label' => t('Latest user account revision'),
    ),
  );
  $data['commerce_hipay_ws_user_account_revision']['revision_uid'] = array(
    'title' => t('User'),
    'help' => t('Relate an user account revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'revision_uid',
      'field_name' => 'revision_uid',
      'label' => t('Revision user'),
    ),
  );
  $data['commerce_hipay_ws_user_account_revision']['log'] = array(
    'title' => t('Log message'),
    'help' => t('The log message entered when the revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['commerce_hipay_ws_user_account_revision']['revision_timestamp'] = array(
    'title' => t('Revision date'),
    'help' => t('The date the Hipay user account revision was created.'),
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
 * Provides default views for the user account entities.
 *
 * @see commerce_hipay_ws_views_default_views()
 */
function commerce_hipay_ws_user_account_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'hipay_ws_user_accounts';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_ws_user_account';
  $view->human_name = 'Hipay Wallet: User accounts';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Hipay Wallet user accounts';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit own commerce_hipay_ws_user_account entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'user_account_id' => 'user_account_id',
    'hipay_account_id' => 'hipay_account_id',
    'rendered_entity' => 'rendered_entity',
    'hipay_ws_user_currency' => 'hipay_ws_user_currency',
    'hipay_identified' => 'hipay_identified',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = 'user_account_id';
  $handler->display->display_options['style_options']['info'] = array(
    'user_account_id' => array(
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
    'hipay_ws_user_currency' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hipay_identified' => array(
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
  $handler->display->display_options['relationships']['hipay_ws_user_user_account_target_id']['id'] = 'hipay_ws_user_user_account_target_id';
  $handler->display->display_options['relationships']['hipay_ws_user_user_account_target_id']['table'] = 'field_data_hipay_ws_user_user_account';
  $handler->display->display_options['relationships']['hipay_ws_user_user_account_target_id']['field'] = 'hipay_ws_user_user_account_target_id';
  /* Field: Hipay Wallet user account: User account ID */
  $handler->display->display_options['fields']['user_account_id']['id'] = 'user_account_id';
  $handler->display->display_options['fields']['user_account_id']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['user_account_id']['field'] = 'user_account_id';
  $handler->display->display_options['fields']['user_account_id']['label'] = 'ID';
  /* Field: Hipay Wallet user account: Hipay user account ID */
  $handler->display->display_options['fields']['hipay_account_id']['id'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['hipay_account_id']['field'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['label'] = 'Hipay account ID';
  /* Field: Hipay Wallet user account: Rendered Hipay Wallet user account */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Account';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'full';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  /* Field: Hipay Wallet user account: Currency */
  $handler->display->display_options['fields']['hipay_ws_user_currency']['id'] = 'hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['table'] = 'field_data_hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['field'] = 'hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_currency']['field_api_classes'] = TRUE;
  /* Field: Hipay Wallet user account: Hipay account status */
  $handler->display->display_options['fields']['hipay_identified']['id'] = 'hipay_identified';
  $handler->display->display_options['fields']['hipay_identified']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['hipay_identified']['field'] = 'hipay_identified';
  $handler->display->display_options['fields']['hipay_identified']['label'] = 'Identified';
  /* Field: Hipay Wallet user account: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = '';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;
  /* Filter criterion: Hipay Wallet user account: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'hipay_user_account' => 'hipay_user_account',
  );

  /* Display: Page: Admin: Accounts */
  $handler = $view->new_display('page', 'Page: Admin: Accounts', 'page_user_accounts_admin');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-wallet/user-accounts/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['description'] = 'Find and manage Hipay Wallet user accounts.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'User accounts';
  $handler->display->display_options['tab_options']['description'] = 'Manage Hipay Wallet user accounts.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  /* Display: Page: Admin: Sub-accounts */
  $handler = $view->new_display('page', 'Page: Admin: Sub-accounts', 'page_user_subaccounts_admin');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Hipay Wallet user sub-accounts';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Hipay Wallet user account: User account ID */
  $handler->display->display_options['fields']['user_account_id']['id'] = 'user_account_id';
  $handler->display->display_options['fields']['user_account_id']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['user_account_id']['field'] = 'user_account_id';
  $handler->display->display_options['fields']['user_account_id']['label'] = 'ID';
  /* Field: Hipay Wallet user account: Hipay user account ID */
  $handler->display->display_options['fields']['hipay_account_id']['id'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['hipay_account_id']['field'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['label'] = 'Hipay account ID';
  /* Field: Hipay Wallet user account: Rendered Hipay Wallet user account */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['relationship'] = 'hipay_ws_user_user_account_target_id';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Principal account';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'full';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  /* Field: Hipay Wallet user account: Currency */
  $handler->display->display_options['fields']['hipay_ws_user_currency']['id'] = 'hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['table'] = 'field_data_hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['field'] = 'hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_currency']['field_api_classes'] = TRUE;
  /* Field: Hipay Wallet user account: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = '';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Hipay Wallet user account: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'hipay_user_subaccount' => 'hipay_user_subaccount',
  );
  $handler->display->display_options['path'] = 'admin/commerce/hipay-wallet/user-subaccounts/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['description'] = 'Find and manage Hipay Wallet user accounts.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'User sub-accounts';
  $handler->display->display_options['tab_options']['description'] = 'Manage Hipay Wallet user accounts.';
  $handler->display->display_options['tab_options']['weight'] = '10';
  $handler->display->display_options['tab_options']['name'] = 'management';

  /* Display: Page: Store */
  $handler = $view->new_display('page', 'Page: Store', 'page_user_accounts_store');
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'The Hipay Wallet user account has not yet been created for this store.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Hipay Wallet user account: Store (hipay_ws_user_store) */
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['id'] = 'hipay_ws_user_store_target_id';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['table'] = 'field_data_hipay_ws_user_store';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['field'] = 'hipay_ws_user_store_target_id';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'store/%/hipay-wallet/user-accounts';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'User accounts';
  $handler->display->display_options['menu']['description'] = 'Manage your Hipay Wallet user accounts.';
  $handler->display->display_options['menu']['weight'] = '-20';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['title'] = 'Hipay Wallet';
  $handler->display->display_options['tab_options']['description'] = 'Manage your Hipay Wallet.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $translatables['hipay_ws_user_accounts'] = array(
    t('Master'),
    t('Hipay Wallet user accounts'),
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
    t('Hipay Wallet user account entity referenced from hipay_ws_user_user_account'),
    t('ID'),
    t('.'),
    t(','),
    t('Hipay account ID'),
    t('Account'),
    t('Currency'),
    t('Identified'),
    t('Page: Admin: Accounts'),
    t('Page: Admin: Sub-accounts'),
    t('Hipay Wallet user sub-accounts'),
    t('Principal account'),
    t('Page: Store'),
    t('The Hipay Wallet user account has not yet been created for this store.'),
    t('All'),
  );
  $export[$view->name] = $view;

  $view = new view();
  $view->name = 'hipay_ws_user_account_revisions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_ws_user_account_revision';
  $view->human_name = 'Hipay Wallet: User account revisions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Revisions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer commerce_hipay_ws_user_account entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'revision_id' => 'revision_id',
    'revision_timestamp' => 'revision_timestamp',
    'name' => 'name',
    'hipay_account_id' => 'hipay_account_id',
    'hipay_identified' => 'hipay_identified',
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
    'hipay_identified' => array(
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
  /* Relationship: Hipay Wallet user account Revisions: User */
  $handler->display->display_options['relationships']['revision_uid']['id'] = 'revision_uid';
  $handler->display->display_options['relationships']['revision_uid']['table'] = 'commerce_hipay_ws_user_account_revision';
  $handler->display->display_options['relationships']['revision_uid']['field'] = 'revision_uid';
  $handler->display->display_options['relationships']['revision_uid']['required'] = TRUE;
  /* Field: Hipay Wallet user account Revisions: Revision ID */
  $handler->display->display_options['fields']['revision_id']['id'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['table'] = 'commerce_hipay_ws_user_account_revision';
  $handler->display->display_options['fields']['revision_id']['field'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['label'] = 'Revision';
  /* Field: Hipay Wallet user account Revisions: Revision date */
  $handler->display->display_options['fields']['revision_timestamp']['id'] = 'revision_timestamp';
  $handler->display->display_options['fields']['revision_timestamp']['table'] = 'commerce_hipay_ws_user_account_revision';
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
  /* Field: Hipay Wallet user account Revisions: Hipay user account ID */
  $handler->display->display_options['fields']['hipay_account_id']['id'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['table'] = 'commerce_hipay_ws_user_account_revision';
  $handler->display->display_options['fields']['hipay_account_id']['field'] = 'hipay_account_id';
  /* Field: Hipay Wallet user account Revisions: Hipay account status */
  $handler->display->display_options['fields']['hipay_identified']['id'] = 'hipay_identified';
  $handler->display->display_options['fields']['hipay_identified']['table'] = 'commerce_hipay_ws_user_account_revision';
  $handler->display->display_options['fields']['hipay_identified']['field'] = 'hipay_identified';
  $handler->display->display_options['fields']['hipay_identified']['label'] = 'Identified';
  /* Field: Hipay Wallet user account Revisions: Log message */
  $handler->display->display_options['fields']['log']['id'] = 'log';
  $handler->display->display_options['fields']['log']['table'] = 'commerce_hipay_ws_user_account_revision';
  $handler->display->display_options['fields']['log']['field'] = 'log';
  /* Contextual filter: Hipay Wallet user account Revisions: User account ID */
  $handler->display->display_options['arguments']['user_account_id']['id'] = 'user_account_id';
  $handler->display->display_options['arguments']['user_account_id']['table'] = 'commerce_hipay_ws_user_account_revision';
  $handler->display->display_options['arguments']['user_account_id']['field'] = 'user_account_id';
  $handler->display->display_options['arguments']['user_account_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['user_account_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['user_account_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['user_account_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['user_account_id']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-wallet/user-accounts/%/revisions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'revisions';
  $handler->display->display_options['menu']['description'] = 'View revisions of this Hipay user account.';
  $handler->display->display_options['menu']['weight'] = '20';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['commerce_hipay_ws_user_account_revisions'] = array(
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
    t('Identified'),
    t('Log message'),
    t('All'),
    t('Page'),
  );
  $export[$view->name] = $view;

  return $export;
}

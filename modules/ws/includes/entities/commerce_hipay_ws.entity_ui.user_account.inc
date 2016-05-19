<?php

function commerce_hipay_ws_user_account_menu() {
  $items = array();

  $items['admin/commerce/hipay-wallet/user-accounts/add'] = array(
    'title' => 'Add a user account',
    'page callback' => 'commerce_hipay_ws_user_account_add',
    'page arguments' => array('hipay_user_account'),
    'access arguments' => array('create commerce_hipay_ws_user_account entities'),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-accounts/%commerce_hipay_ws_user_account'] = array(
    'title callback' => 'commerce_hipay_ws_user_account_title',
    'title arguments' => array(4),
    'page callback' => 'commerce_hipay_ws_user_account_view',
    'page arguments' => array(4),
    'access arguments' => array('view any commerce_hipay_ws_user_account entity'),
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
    'access arguments' => array('view any commerce_hipay_ws_user_account entity'),
    'type' => MENU_LOCAL_TASK,
    'weight' => -5,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-accounts/%commerce_hipay_ws_user_account/edit'] = array(
    'title' => 'edit',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_ws_user_account_form', 4),
    'access arguments' => array('edit any commerce_hipay_ws_user_account entity'),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-accounts/%commerce_hipay_ws_user_account/delete'] = array(
    'title' => 'delete',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_ws_user_account_delete_form', 4, 'account'),
    'access arguments' => array('delete any commerce_hipay_ws_user_account entity'),
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
    'access arguments' => array('create commerce_hipay_ws_user_account entities'),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-subaccounts/%commerce_hipay_ws_user_account'] = array(
    'title callback' => 'commerce_hipay_ws_user_account_title',
    'title arguments' => array(4),
    'page callback' => 'commerce_hipay_ws_user_account_view',
    'page arguments' => array(4),
    'access arguments' => array('view any commerce_hipay_ws_user_account entity'),
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
    'access arguments' => array('edit any commerce_hipay_ws_user_account entity'),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  $items['admin/commerce/hipay-wallet/user-subaccounts/%commerce_hipay_ws_user_account/delete'] = array(
    'title' => 'delete',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_ws_user_account_delete_form', 4, 'sub-account'),
    'access arguments' => array('delete any commerce_hipay_ws_user_account entity'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.user_account.inc',
  );

  return $items;
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
  if (!empty($entity->hipay_account_id)) {
    $return = '';

    // Fetch: getAccountInfos.
    $parameters = array('accountId' => $entity->hipay_account_id);
    $context = array('commerce_hipay_ws_user_account' => $entity);
    $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_ACCOUNT_INFOS, $parameters, NULL, $context);
    if (isset($response['code']) && $response['code'] === 0) {

      // Display: getAccountInfos.
      $return .= '<p><strong>getAccountInfos</strong></p>';
      $exclude = array('code', 'description', 'websites', 'subAccounts');
      $header = $row = array();
      foreach (array_diff_key($response, array_flip($exclude)) as $key => $value) {
        $header[$key] = check_plain($key);
        $row[$key] = check_plain($value);
      }
      $return .= theme('table', array(
        'header' => $header,
        'rows' => array($row)
      ));

      // Display: getAccountInfos: websites.
      if (!empty($response['websites'])) {
        $return .= '<p><strong>getAccountInfos: websites</strong></p>';
        $header = $rows = array();
        foreach ($response['websites'] as $item) {
          $row = array();
          foreach ($item as $key => $value) {
            $header[$key] = check_plain($key);
            $row[$key] = check_plain($value);
          }
          $rows[] = $row;
        }
        $return .= theme('table', array(
          'header' => $header,
          'rows' => array($row)
        ));
      }

      // @TODO: Display: getAccountInfos: subAccounts.
      if (!empty($response['subAccounts'])) {
        $return .= '<p><strong>getAccountInfos: subAccounts</strong></p>';
      }
    }

    // Fetch: getBalance.
    $parameters = array('wsSubAccountId' => $entity->hipay_account_id);
    $context = array('commerce_hipay_ws_user_account' => $entity);
    $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_GET_BALANCE, $parameters, NULL, $context);

    // Display: getBalance.
    if (!empty($response['balances'])) {
      $return .= '<p><strong>getBalance</strong></p>';
      $header = $rows = array();
      foreach ($response['balances'] as $item) {
        $row = array();
        foreach ($item as $key => $value) {
          $header[$key] = check_plain($key);
          $row[$key] = check_plain($value);
        }
        $rows[] = $row;
      }
      $return .= theme('table', array('header' => $header, 'rows' => array($row)));
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

    // @TODO: Display: getTransactions.
    if (!empty($response['transactions'])) {
      $return .= '<p><strong>getTransactions</strong></p>';
    }

    return $return;
  }
  else {
    return drupal_get_form('commerce_hipay_ws_user_account_view_hipay_create_form', $entity);
  }
}

/**
 * Returns a form to create a Hipay User Account if it doesn't exist yet.
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
 * Form submit handler: created a Hipay User Account for an entity.
 *
 * @see commerce_hipay_ws_user_account_view_hipay_create_form()
 */
function commerce_hipay_ws_user_account_view_hipay_create_form_submit($form, $form_state) {
  $user_account = $form_state['commerce_hipay_ws_user_account'];

  $response = commerce_hipay_ws_api_user_account_create($user_account);

  if ($response['code'] !== 0) {
    drupal_set_message(t('Error creating a new user account in Hipay Wallet: @error.', array('@error' => $response['description'])), 'error');
  }
}

/**
 * Provides a wrapper on the edit form to add a new entity.
 */
function commerce_hipay_ws_user_account_add($type) {
  // Create a basic entity structure to be used and passed to the validation
  // and submission functions.
  $user_account = entity_get_controller('commerce_hipay_ws_user_account')->create(array('type' => $type));
  return drupal_get_form('commerce_hipay_ws_user_account_form', $user_account);
}

/**
 * Form function to create a bpost_accounting_reference entity.
 */
function commerce_hipay_ws_user_account_form($form, &$form_state, $user_account) {
  global $user;

  // Ensure the owner name is accessible if the uid is set.
  if (isset($user_account->uid) && $owner = user_load($user_account->uid)) {
    $user_account->name = $owner->name;
  }

  $form['commerce_hipay_ws_user_account'] = array(
    '#type' => 'value',
    '#value' => $user_account,
  );

  // Add the field related form elements.
  field_attach_form('commerce_hipay_ws_user_account', $user_account, $form, $form_state);

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
 * Validation handler for bpost_accounting_reference_form form.
 */
function commerce_hipay_ws_user_account_form_validate($form, &$form_state) {
  $user_account = $form_state['values']['commerce_hipay_ws_user_account'];

  // Validate the 'Owned by' field.
  if (!empty($form_state['values']['name']) && !($account = user_load_by_name($form_state['values']['name']))) {
    // The use of empty() is mandatory in the context of usernames as the empty
    // string denotes an anonymous user.
    form_set_error('name', t('The username %name does not exist.', array('%name' => $form_state['values']['name'])));
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
 * Form submit handler: submits basic_add_form information
 */
function commerce_hipay_ws_user_account_form_submit($form, &$form_state) {
  $user_account = $form_state['values']['commerce_hipay_ws_user_account'];

  // Set the profile's owner uid based on the supplied name.
  if (!empty($form_state['values']['name']) && $account = user_load_by_name($form_state['values']['name'])) {
    $user_account->uid = $account->uid;
  }
  else {
    $user_account->uid = 0;
  }

  // Notify field widgets.
  field_attach_submit('commerce_hipay_ws_user_account', $user_account, $form, $form_state);

  // Save the Hipay user account.
  commerce_hipay_ws_user_account_save($user_account);

  drupal_set_message(t('Hipay user account saved.'));
  $form_state['redirect'] = 'admin/commerce/hipay-wallet/user-accounts/' . $user_account->user_account_id;
}

/**
 * Form deletion handler.
 */
function commerce_hipay_ws_user_account_delete($form, &$form_state) {
  return drupal_get_form('commerce_hipay_ws_user_account_delete_form', $form_state['values']['commerce_hipay_ws_user_account']);
}

/**
 * Form constructor for delete an accounting reference form.
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
 * Form submission handler for bpost_accounting_reference_ui_delete_form().
 */
function commerce_hipay_ws_user_account_delete_form_submit($form, &$form_state) {
  $form_state['commerce_hipay_ws_user_account']->delete();

  drupal_set_message(t('The user !type has been deleted.', array('!type' => $form_state['type'])));

  $account_path = ($form_state['commerce_hipay_ws_user_account']->type == 'hipay_user_subaccount') ? 'user-subaccounts' : 'user-accounts';
  $form_state['redirect'] = 'admin/commerce/hipay-wallet/' . $account_path;
}

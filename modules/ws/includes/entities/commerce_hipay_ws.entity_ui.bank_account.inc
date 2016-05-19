<?php

function commerce_hipay_ws_bank_account_menu() {
  $items = array();

  $items['admin/commerce/hipay-wallet/bank-accounts/add'] = array(
    'title' => 'Add a bank account',
    'page callback' => 'commerce_hipay_ws_bank_account_add',
    'access arguments' => array('create commerce_hipay_ws_bank_account entities'),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account'] = array(
    'title callback' => 'commerce_hipay_ws_bank_account_title',
    'title arguments' => array(4),
    'page callback' => 'commerce_hipay_ws_bank_account_view',
    'page arguments' => array(4),
    'access arguments' => array('view any commerce_hipay_ws_bank_account entity'),
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
    'access arguments' => array('view any commerce_hipay_ws_bank_account entity'),
    'type' => MENU_LOCAL_TASK,
    'weight' => -5,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account/edit'] = array(
    'title' => 'edit',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_ws_bank_account_form', 4),
    'access arguments' => array('edit any commerce_hipay_ws_bank_account entity'),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
  );

  $items['admin/commerce/hipay-wallet/bank-accounts/%commerce_hipay_ws_bank_account/delete'] = array(
    'title' => 'delete',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_ws_bank_account_delete_form', 4),
    'access arguments' => array('delete any commerce_hipay_ws_bank_account entity'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.bank_account.inc',
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
 * @see commerce_hipay_ws_bank_account_menu()
 */
function commerce_hipay_ws_bank_account_view($entity, $view_mode = 'full') {
  return entity_view('commerce_hipay_ws_bank_account', array($entity->bank_account_id => $entity), $view_mode, NULL, TRUE);
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
    $response = commerce_hipay_ws_api_request(COMMERCE_HIPAY_WS_RESOURCE_SOAP_BANK_INFOS_STATUS, $parameters, NULL, $context);

    if ($response['status'] == 'No bank information') {
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
        if ($response['code'] === 0) {
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
 * Returns a form to create a Hipay Bank Account if it doesn't exist yet.
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
 * Form submit handler: created a Hipay User Account for an entity.
 *
 * @see commerce_hipay_ws_user_account_view_hipay_create_form()
 */
function commerce_hipay_ws_bank_account_view_hipay_create_form_submit($form, $form_state) {
  $user_account = $form_state['commerce_hipay_ws_bank_account'];

  $response = commerce_hipay_ws_api_bank_account_create($user_account);

  if ($response['code'] !== 0) {
    drupal_set_message(t('Error creating a new bank account in Hipay Wallet: @error.', array('@error' => $response['description'])), 'error');
  }
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
  if (!$response = commerce_hipay_ws_get_bank_fields($form_state['values']['hipay_ws_bank_address'][LANGUAGE_NONE][0]['country'])) {
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
  $form_state['redirect'] = 'admin/commerce/hipay-wallet/bank-accounts/' . $bank_account->bank_account_id;
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
    'admin/commerce/hipay-wallet/bank-accounts',
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
    'acct_num' => '	hipay_ws_bank_account_number',
    'aba_num' => 'hipay_ws_bank_aba_number',
    'transit_num' => 'hipay_ws_bank_transit_number',
  );
}

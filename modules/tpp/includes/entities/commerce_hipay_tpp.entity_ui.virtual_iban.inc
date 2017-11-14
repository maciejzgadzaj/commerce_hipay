<?php

/**
 * Returns Virtual IBAN-related menu items.
 *
 * @see commerce_hipay_tpp_menu()
 */
function commerce_hipay_tpp_virtual_iban_menu() {
  $items = array();

  $items['admin/commerce/hipay-tpp/virtual-ibans/add'] = array(
    'title' => 'Add a Virtual IBAN',
    'page callback' => 'commerce_hipay_tpp_virtual_iban_add',
    'access callback' => 'commerce_hipay_tpp_virtual_iban_access',
    'access arguments' => array('create'),
    'type' => MENU_LOCAL_ACTION,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.virtual_iban.inc',
  );

  $items['admin/commerce/hipay-tpp/virtual-ibans/%commerce_hipay_tpp_virtual_iban'] = array(
    'title callback' => 'commerce_hipay_tpp_virtual_iban_title',
    'title arguments' => array(4),
    'page callback' => 'commerce_hipay_tpp_virtual_iban_view',
    'page arguments' => array(4),
    'access callback' => 'commerce_hipay_tpp_virtual_iban_access',
    'access arguments' => array('view', 4),
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.virtual_iban.inc',
  );

  $items['admin/commerce/hipay-tpp/virtual-ibans/%commerce_hipay_tpp_virtual_iban/view'] = array(
    'title' => 'view',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.virtual_iban.inc',
  );

  $items['admin/commerce/hipay-tpp/virtual-ibans/%commerce_hipay_tpp_virtual_iban/view/entity'] = array(
    'title' => 'local',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.virtual_iban.inc',
  );

  $items['admin/commerce/hipay-tpp/virtual-ibans/%commerce_hipay_tpp_virtual_iban/view/hipay'] = array(
    'title' => 'Hipay',
    'page callback' => 'commerce_hipay_tpp_virtual_iban_view_hipay_info',
    'page arguments' => array(4),
    'access callback' => 'commerce_hipay_tpp_virtual_iban_access',
    'access arguments' => array('view', 4),
    'type' => MENU_LOCAL_TASK,
    'weight' => -5,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.virtual_iban.inc',
  );

  $items['admin/commerce/hipay-tpp/virtual-ibans/%commerce_hipay_tpp_virtual_iban/view/payload'] = array(
    'title' => 'payload',
    'page callback' => 'commerce_hipay_tpp_virtual_iban_view_payload_info',
    'page arguments' => array(4),
    'access arguments' => array('access Hipay Virtual IBAN calls payload'),
    'type' => MENU_LOCAL_TASK,
    'weight' => -2,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.virtual_iban.inc',
  );

  $items['admin/commerce/hipay-tpp/virtual-ibans/%commerce_hipay_tpp_virtual_iban/edit'] = array(
    'title' => 'edit',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_tpp_virtual_iban_form', 4),
    'access callback' => 'commerce_hipay_tpp_virtual_iban_access',
    'access arguments' => array('edit', 4),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.virtual_iban.inc',
  );

  $items['admin/commerce/hipay-tpp/virtual-ibans/%commerce_hipay_tpp_virtual_iban/delete'] = array(
    'title' => 'delete',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_tpp_virtual_iban_delete_form', 4),
    'access callback' => 'commerce_hipay_tpp_virtual_iban_access',
    'access arguments' => array('delete', 4),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.virtual_iban.inc',
  );

  $items['admin/commerce/hipay-tpp/virtual-ibans/payment-transactions/%commerce_payment_transaction'] = array(
    'title callback' => 'commerce_payment_ui_payment_transaction_title',
    'title arguments' => array(5),
    'page callback' => 'commerce_payment_ui_payment_transaction_view',
    'page arguments' => array(NULL, 5, 'administrator'),
    'access callback' => 'commerce_payment_transaction_order_access',
    'access arguments' => array('view', 5),
    'file' => 'commerce_payment_ui.admin.inc',
    'file path' => drupal_get_path('module', 'commerce_payment_ui') . '/includes',
  );

  $items['admin/commerce/hipay-tpp/virtual-ibans/payment-transactions/%commerce_payment_transaction/assign-to-order'] = array(
    'title' => 'Assign payment transaction to an order',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_hipay_tpp_virtual_iban_payment_assign_to_order_form', 5),
    'access callback' => 'commerce_payment_transaction_order_access',
    'access arguments' => array('update', 5),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_tpp.entity_ui.virtual_iban.inc',
  );

  return $items;
}

/**
 * Checks the current user's access to the specified Virtual IBAN operation.
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
function commerce_hipay_tpp_virtual_iban_access($op, $entity = NULL, $account = NULL) {
  switch ($op) {
    case 'create':
      return user_access('create commerce_hipay_tpp_virtual_iban entities');
    case 'view':
      return user_access('view any commerce_hipay_tpp_virtual_iban entity');
    case 'edit':
      return user_access('edit any commerce_hipay_tpp_virtual_iban entity');
    case 'delete':
      return user_access('administer commerce_hipay_tpp_virtual_iban entities');
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
 * @see commerce_hipay_tpp_virtual_iban_menu()
 */
function commerce_hipay_tpp_virtual_iban_view($entity, $view_mode = 'full') {
  $return = entity_view('commerce_hipay_tpp_virtual_iban', array($entity->virtual_iban_id => $entity), $view_mode, NULL, TRUE);

  if (empty($entity->external_reference)) {
    $return['commerce_hipay_tpp_virtual_iban_create'] = drupal_get_form('commerce_hipay_tpp_virtual_iban_view_hipay_create_form', $entity);
  }

  return $return;
}

/**
 * Implements hook_ENTITY_TYPE_view_alter() for commerce_hipay_tpp_virtual_iban.
 */
function commerce_hipay_tpp_commerce_hipay_tpp_virtual_iban_view_alter(&$build, $entity_type) {
  $build['uid']['#content'] = l($build['#entity']->uid, 'user/' . $build['#entity']->uid);
  $build['created']['#content'] = format_date($build['#entity']->created);
  $build['changed']['#content'] = format_date($build['#entity']->changed);
  $build['expires']['#content'] = format_date($build['#entity']->expires);
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
 * @see commerce_hipay_tpp_virtual_iban_menu()
 */
function commerce_hipay_tpp_virtual_iban_view_payload_info($entity, $view_mode = 'full') {
  $markup = '';

  if (!empty($entity->data['payload'])) {
    $markup = '<pre>' . check_plain(print_r($entity->data['payload'], TRUE)) . '</pre>';
  }

  return $markup;
}

/**
 * Menu callback: fetches and displays Hipay Virtual IBAN information.
 *
 * @param object $entity
 *   An entity to render.
 * @param string $view_mode
 *   A view mode as used by this entity type, e.g. 'full', 'teaser'...
 *
 * @return mixed
 *   A renderable array, or a rendered HTML.
 *
 * @see commerce_hipay_tpp_virtual_iban_menu()
 */
function commerce_hipay_tpp_virtual_iban_view_hipay_info($entity, $view_mode = 'full') {
  $return = '';

  if (!empty($entity->external_reference)) {
    $response = commerce_hipay_tpp_api_virtual_iban_search($entity->external_reference);

    if (isset($response['code']) && $response['code'] === COMMERCE_HIPAY_TPP_REST_STATUS_SUCCESS) {
      if (!empty($response['virtual_ibans'])) {
        $header = $rows = array();
        foreach ($response['virtual_ibans'] as $delta => $virtual_iban) {
          foreach ($virtual_iban as $key => $value) {
            $header[$key] = check_plain($key);
            $rows[$delta][$key] = check_plain($value);
          }
          $return .= theme('table', array(
            'header' => $header,
            'rows' => $rows,
          ));
        }

      }
      else {
        drupal_set_message(t($response['description']), 'warning');
      }
    }
  }
  else {
    $return['commerce_hipay_tpp_virtual_iban_create'] = drupal_get_form('commerce_hipay_tpp_virtual_iban_view_hipay_create_form', $entity);
  }

  return $return;
}

/**
 * Returns a form to create a Virtual IBAN in Hipay TPP.
 *
 * @see commerce_hipay_tpp_virtual_iban_view_hipay_info()
 * @see commerce_hipay_tpp_virtual_iban_view_hipay_create_form_submit()
 */
function commerce_hipay_tpp_virtual_iban_view_hipay_create_form($form, &$form_state, $entity) {
  $form_state['commerce_hipay_tpp_virtual_iban'] = $entity;

  $form['info'] = array(
    '#markup' => '<p>' . t('The Virtual IBAN has not yet been created for this entity in Hipay.') . '</p>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create Virtual IBAN in Hipay TPP'),
  );

  return $form;
}

/**
 * Form submit handler: creates a Virtual IBAN in Hipay TPP.
 *
 * @see commerce_hipay_tpp_virtual_iban_view_hipay_create_form()
 */
function commerce_hipay_tpp_virtual_iban_view_hipay_create_form_submit($form, $form_state) {
  $virtual_iban = $form_state['commerce_hipay_tpp_virtual_iban'];

  $response = commerce_hipay_tpp_api_virtual_iban_create($virtual_iban);

  if ($response['code'] !== COMMERCE_HIPAY_TPP_REST_STATUS_SUCCESS) {
    drupal_set_message(t('Error creating new Virtual IBAN in Hipay TPP: @error', array('@error' => $response['description'])), 'error');
  }
}

/**
 * Provides a wrapper on the edit form to add a new Virtual IBAN entity.
 *
 * @see commerce_hipay_tpp_virtual_iban_menu()
 */
function commerce_hipay_tpp_virtual_iban_add() {
  // Create a basic entity structure to be used and passed to the validation
  // and submission functions.
  $virtual_iban = entity_get_controller('commerce_hipay_tpp_virtual_iban')->create();
  return drupal_get_form('commerce_hipay_tpp_virtual_iban_form', $virtual_iban);
}

/**
 * Form function to edit a Virtual IBAN entity.
 *
 * @see commerce_hipay_tpp_virtual_iban_form_validate()
 * @see commerce_hipay_tpp_virtual_iban_form_submit()
 */
function commerce_hipay_tpp_virtual_iban_form($form, &$form_state, $virtual_iban) {
  global $user;

  // Ensure the owner name is accessible if the uid is set.
  if (isset($virtual_iban->uid) && $owner = user_load($virtual_iban->uid)) {
    $virtual_iban->name = $owner->name;
  }

  if (empty($virtual_iban->created)) {
    $virtual_iban->date = format_date(REQUEST_TIME, 'custom', 'Y-m-d H:i:s O');
  }

  $form['commerce_hipay_tpp_virtual_iban'] = array(
    '#type' => 'value',
    '#value' => $virtual_iban,
  );

  // Add the field related form elements.
  field_attach_form('commerce_hipay_tpp_virtual_iban', $virtual_iban, $form, $form_state);

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
    '#default_value' => isset($virtual_iban->name) ? $virtual_iban->name : $user->name,
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
    '#default_value' => empty($virtual_iban->is_new),
    '#access' => user_access('administer commerce_hipay_tpp_virtual_iban entities'),
  );
  $form['revision_information']['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Created on'),
    '#description' => t('Format: %time. The date format is YYYY-MM-DD and %timezone is the time zone offset from UTC. Leave blank to use the time of form submission.', array(
      '%time' => !empty($virtual_iban->date) ? date_format(date_create($virtual_iban->date), 'Y-m-d H:i:s O') : format_date($virtual_iban->created, 'custom', 'Y-m-d H:i:s O'),
      '%timezone' => !empty($virtual_iban->date) ? date_format(date_create($virtual_iban->date), 'O') : format_date($virtual_iban->created, 'custom', 'O'),
    )),
    '#maxlength' => 25,
    '#default_value' => !empty($virtual_iban->created) ? format_date($virtual_iban->created, 'custom', 'Y-m-d H:i:s O') : '',
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
    '#submit' => array('commerce_hipay_tpp_virtual_iban_delete'),
    '#weight' => 200,
  );

  return $form;
}

/**
 * Validation handler for Virtual IBAN edit form.
 *
 * @see commerce_hipay_tpp_virtual_iban_form()
 * @see commerce_hipay_tpp_virtual_iban_form_submit()
 */
function commerce_hipay_tpp_virtual_iban_form_validate($form, &$form_state) {
  $virtual_iban = $form_state['values']['commerce_hipay_tpp_virtual_iban'];

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
  field_attach_form_validate('commerce_hipay_tpp_virtual_iban', $form_state['values']['commerce_hipay_tpp_virtual_iban'], $form, $form_state);
}

/**
 * Submission handler for Virtual IBAN edit form.
 *
 * @see commerce_hipay_tpp_virtual_iban_form()
 * @see commerce_hipay_tpp_virtual_iban_form_validate()
 */
function commerce_hipay_tpp_virtual_iban_form_submit($form, &$form_state) {
  $virtual_iban = $form_state['values']['commerce_hipay_tpp_virtual_iban'];

  if ($form_state['values']['revision'] || !empty($form_state['values']['log'])) {
    $virtual_iban->revision = TRUE;
    $virtual_iban->log = $form_state['values']['log'];
  }

  // Set the profile's owner uid based on the supplied name.
  if (!empty($form_state['values']['name']) && $account = user_load_by_name($form_state['values']['name'])) {
    $virtual_iban->uid = $account->uid;
  }
  else {
    $virtual_iban->uid = 0;
  }

  $virtual_iban->created = !empty($form_state['values']['date']) ? strtotime($form_state['values']['date']) : REQUEST_TIME;

  // Notify field widgets.
  field_attach_submit('commerce_hipay_tpp_virtual_iban', $virtual_iban, $form, $form_state);

  // Save the Virtual IBAN.
  commerce_hipay_tpp_virtual_iban_save($virtual_iban);

  drupal_set_message(t('Hipay Virtual IBAN saved.'));
  $form_state['redirect'] = 'admin/commerce/hipay-tpp/virtual-ibans/' . $virtual_iban->virtual_iban_id;
}

/**
 * Menu callback: returns Virtual IBAN deletion form.
 *
 * @see commerce_hipay_tpp_virtual_iban_menu()
 */
function commerce_hipay_tpp_virtual_iban_delete($form, &$form_state) {
  return drupal_get_form('commerce_hipay_tpp_virtual_iban_delete_form', $form_state['values']['commerce_hipay_tpp_virtual_iban']);
}

/**
 * Form constructor for Virtual IBAN deletion form.
 *
 * @see commerce_hipay_tpp_virtual_iban_form_submit()
 */
function commerce_hipay_tpp_virtual_iban_delete_form($form, &$form_state, $virtual_iban) {
  $form_state['commerce_hipay_tpp_virtual_iban'] = $virtual_iban;

  $form = confirm_form($form,
    t('Are you sure you want to delete this Virtual IBAN?'),
    'admin/commerce/hipay-tpp/virtual-ibans',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submission handler for Virtual IBAN deletion form.
 *
 * @see commerce_hipay_tpp_virtual_iban_delete_form()
 */
function commerce_hipay_tpp_virtual_iban_delete_form_submit($form, &$form_state) {
  $form_state['commerce_hipay_tpp_virtual_iban']->delete();

  drupal_set_message(t('The Virtual IBAN has been deleted.'));
  $form_state['redirect'] = 'admin/commerce/hipay-tpp/virtual-ibans';
}

/**
 * Menu callback: edit a payment transaction.
 *
 * @see commerce_hipay_tpp_virtual_iban_menu()
 * @see commerce_hipay_tpp_virtual_iban_payment_assign_to_order_form_validate()
 * @see commerce_hipay_tpp_virtual_iban_payment_assign_to_order_form_submit()
 */
function commerce_hipay_tpp_virtual_iban_payment_assign_to_order_form($form, &$form_state, $transaction) {
  $form_state['transaction'] = $transaction;

  drupal_set_title(t('Assign payment transaction to an order'));

  $form['info']['#markup'] = '<p>' . t('Assign payment transaction @transaction_id (@amount) to an order.', array(
    '@transaction_id' => $transaction->transaction_id,
    '@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code),
  )) . '</p>';

  $query = db_select('commerce_order', 'o');
  $result = $query->fields('o', array('order_id', 'order_number'))
    ->condition('o.status', COMMERCE_HIPAY_TPP_ORDER_STATUS_PENDING_BANK_TRANSFER)
    ->execute()
    ->fetchAllKeyed();
  $orders = entity_load('commerce_order', $result);
  $form_state['orders'] = $orders;

  $header = array(
    t('Order ID'),
    t('Updated'),
    t('Owner'),
    t('Total'),
    t('Balance'),
    t('Status'),
  );

  $options = array();
  foreach ($orders as $order_id => $order) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $order_total = $order_wrapper->commerce_order_total->value();
    $order_balance = commerce_payment_order_balance($order);
    $options[$order_id] = array(
      l($order->order_id, 'admin/commerce/orders/' . $order->order_id),
      format_date($order->changed, 'short'),
      l($order_wrapper->owner->name->value(), 'user/' . $order->uid),
      commerce_currency_format($order_total['amount'], $order_total['currency_code']),
      commerce_currency_format($order_balance['amount'], $order_balance['currency_code']),
      commerce_order_status_get_title($order->status),
    );
  }
  $form['order_id'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#js_select' => FALSE,
    '#empty' => t('No orders pending Virtual IBAN transfer.'),
    '#prefix' => '<b>' . t('Orders') . '</b>',
    '#suffix' => '<div class="description">' . t("Select one or multiple orders to assign this payment transaction to. Please note that when selecting multiple orders, this payment transaction entity will be duplicated for each additional order, and the transactions' amounts will be updated to match the selected orders' balances. Also an additional payment transaction might be created for any remaining amount left after clearing selected orders' balances.") . '</div><br />',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Assign'),
  );

  return $form;
}

/**
 * Edit payment form validation handler.
 *
 * @see commerce_hipay_tpp_virtual_iban_payment_assign_to_order_form()
 * @see commerce_hipay_tpp_virtual_iban_payment_assign_to_order_form_submit()
 */
function commerce_hipay_tpp_virtual_iban_payment_assign_to_order_form_validate($form, &$form_state) {
  $order_ids = array_filter($form_state['values']['order_id']);
  $orders = entity_load('commerce_order', $order_ids);

  if (count($orders) != count($order_ids)) {
    form_set_error('order_id', t('Unable to load one or more of selected orders (@order_ids).', array(
      '@order_ids' => implode(', ', array_diff($form_state['values']['order_id'], array_keys($orders))),
    )));
  }

  foreach ($orders as $order_id => $order) {
    if (empty($order->data['payment_method']) || strpos($order->data['payment_method'], 'commerce_hipay_tpp_virtual_iban') !== 0) {
      form_set_error('order_id', t('Order @order_id was not paid for using Virtual IBAN payment method.', array('@order_id' => $order->order_id)));
    }

    if (!empty($order->data['commerce_payment_order_paid_in_full_invoked'])) {
      form_set_error('order_id', t('Order @order_id has already been paid in full.', array('@order_id' => $order->order_id)));
    }
  }
}

/**
 * Edit payment form submit handler: assigns a payment transaction to an order.
 *
 * @see commerce_hipay_tpp_virtual_iban_payment_assign_to_order_form()
 * @see commerce_hipay_tpp_virtual_iban_payment_assign_to_order_form_validate()
 */
function commerce_hipay_tpp_virtual_iban_payment_assign_to_order_form_submit($form, &$form_state) {
  $order_ids = array_filter($form_state['values']['order_id']);
  $orders = entity_load('commerce_order', $order_ids);
  $remaining_orders = $form_state['values']['order_id'];

  $original_transaction = commerce_payment_transaction_load($form_state['transaction']->transaction_id);
  $transaction = clone $original_transaction;
  $remaining_amount = $transaction->amount;

  foreach ($orders as $order_id => $order) {
    if ($remaining_amount <= 0) {
      break;
    }

    // Order balance needs to be calculated before any new payment transaction
    // is assigned to the order.
    $order_balance = commerce_payment_order_balance($order);

    if (!empty($original_transaction_used)) {
      $transaction = commerce_hipay_tpp_virtual_iban_payment_transaction_new($original_transaction, $order_id);
      $transaction->amount = $remaining_amount;
      $transaction->log = t('Created by duplicating transaction @transaction_id for order @order_id.', array(
        '@transaction_id' => $original_transaction->transaction_id,
        '@order_id' => $form_state['transactions'][$original_transaction->transaction_id]->order_id,
      ));
      commerce_payment_transaction_save($transaction);

      $message = 'Created new payment transaction @transaction_id by duplicating transaction @original_transaction_id.';
      $message_variables = array(
        '@transaction_id' => $transaction->transaction_id,
        '@original_transaction_id' => $original_transaction->transaction_id,
      );
      watchdog('commerce_hipay_tpp_virtual_iban', $message, $message_variables, WATCHDOG_INFO);
      drupal_set_message(t($message, $message_variables));
    }

    $transaction->amount = ($order_balance['amount'] > $transaction->amount) ? $transaction->amount : $order_balance['amount'];
    $remaining_amount -= $transaction->amount;
    $transaction->order_id = $order_id;
    $transaction->revision = TRUE;
    $transaction->log = t('Manually assigned to order @order_id.', array('@order_id' => $order_id));
    commerce_payment_transaction_save($transaction);

    // Mark that we have used the original transaction assigning it to an order,
    // so we need to start creating new transaction in the next loop.
    $original_transaction_used = TRUE;

    // Store updated transaction entity in the $form_state in case other modules
    // wanted to use it for further updates.
    $form_state['transactions'][$transaction->transaction_id] = $transaction;

    $order->revision = TRUE;
    $order->log = t('Virtual IBAN payment transaction @transaction_id manually assigned to the order.', array('@transaction_id' => $transaction->transaction_id));
    commerce_order_save($order);
    unset($remaining_orders[$order_id]);

    $order_balance = commerce_payment_order_balance($order);
    if ($order_balance['amount'] <= 0) {
      $order = commerce_order_status_update($order, 'pending', FALSE, TRUE, t('Customer successfully paid for the order externally via Virtual IBAN transfer.'));
    }

    // Store updated order entity in the $form_state in case other modules
    // wanted to use it for further updates.
    $form_state['orders'][$order_id] = $order;

    $message = 'Payment transaction @transaction_id has been assigned to order @order_id.';
    $message_variables = array(
      '@transaction_id' => $transaction->transaction_id,
      '@order_id' => $order_id,
    );
    watchdog('commerce_hipay_tpp_virtual_iban', $message, $message_variables, WATCHDOG_INFO);
    drupal_set_message(t($message, $message_variables));
  }

  if ($remaining_amount > 0) {
    $transaction = commerce_hipay_tpp_virtual_iban_payment_transaction_new($original_transaction);
    $transaction->amount = $remaining_amount;
    $transaction->log = t('Created for remaining amount by duplicating transaction @transaction_id for order @order_id.', array(
      '@transaction_id' => $original_transaction->transaction_id,
      '@order_id' => $form_state['transactions'][$original_transaction->transaction_id]->order_id,
    ));
    commerce_payment_transaction_save($transaction);

    $message = 'Created new payment transaction @transaction_id for remaining @amount by duplicating transaction @original_transaction_id.';
    $message_variables = array(
      '@transaction_id' => $transaction->transaction_id,
      '@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code),
      '@original_transaction_id' => $original_transaction->transaction_id,
    );
    watchdog('commerce_hipay_tpp_virtual_iban', $message, $message_variables, WATCHDOG_INFO);
    drupal_set_message(t($message, $message_variables));
  }

  $form_state['redirect'] = 'admin/commerce/hipay-tpp/virtual-ibans/payment-transactions';
}

/**
 * Creates a new payment transaction entity from provided existing transaction.
 *
 * @param object $original_transaction
 *   An existing payment transaction to create a new payment transaction from.
 * @param int $order_id
 *   An optional order ID to assign the new payment transaction to.
 *
 * @return object
 *   A new payment transaction entity, not saved yet.
 */
function commerce_hipay_tpp_virtual_iban_payment_transaction_new($original_transaction, $order_id = 0) {
  $transaction = commerce_payment_transaction_new($original_transaction->payment_method, $order_id);
  $transaction->instance_id = $original_transaction->instance_id;
  $transaction->uid = $original_transaction->uid;
  $transaction->amount = $original_transaction->amount;
  $transaction->currency_code = $original_transaction->currency_code;
  $transaction->status = $original_transaction->status;
  $transaction->data = $original_transaction->data;
  $transaction->data['original_transaction_id'] = $original_transaction->transaction_id;
  $transaction->message = $original_transaction->message;
  $transaction->message_variables = $original_transaction->message_variables;
  $transaction->payload = $original_transaction->payload;
  $transaction->remote_id = $original_transaction->remote_id;
  $transaction->remote_status = $original_transaction->remote_status;
  return $transaction;
}

/**
 * Provides Views data structure for the Virtual IBAN entities.
 *
 * @see commerce_hipay_tpp_views_data_alter()
 */
function commerce_hipay_tpp_virtual_iban_views_data_alter(&$data) {
  // Virtual IBANs.
  $data['commerce_hipay_tpp_virtual_iban']['virtual_iban_id']['field']['handler'] = 'commerce_hipay_tpp_handler_link_virtual_iban_id';
  $data['commerce_hipay_tpp_virtual_iban']['uid'] = array(
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
      'help' => t("Relate this Virtual IBAN to its owner's user account"),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('Virtual IBAN owner'),
    ),
  );
  // Expose BIC number.
  $data['commerce_hipay_tpp_virtual_iban']['bic'] = array(
    'title' => t('BIC'),
    'help' => t('Virtual IBAN BIC number.'),
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
  // Expose IBAN number.
  $data['commerce_hipay_tpp_virtual_iban']['iban'] = array(
    'title' => t('IBAN'),
    'help' => t('Virtual IBAN IBAN number.'),
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
  // Expose external reference.
  $data['commerce_hipay_tpp_virtual_iban']['external_reference'] = array(
    'title' => t('External reference'),
    'help' => t('Virtual IBAN external reference.'),
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
  $data['commerce_hipay_tpp_virtual_iban']['link'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the Virtual IBAN.'),
      'handler' => 'commerce_hipay_tpp_virtual_iban_handler_link_field',
    ),
  );
  $data['commerce_hipay_tpp_virtual_iban']['edit'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a link to the edit form for the Virtual IBAN.'),
      'handler' => 'commerce_hipay_tpp_virtual_iban_handler_edit_link_field',
    ),
  );
  $data['commerce_hipay_tpp_virtual_iban']['delete'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a link to delete the Virtual IBAN.'),
      'handler' => 'commerce_hipay_tpp_virtual_iban_handler_delete_link_field',
    ),
  );
  $data['commerce_hipay_tpp_virtual_iban']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this Virtual IBAN.'),
      'handler' => 'commerce_hipay_tpp_virtual_iban_handler_operations_field',
    ),
  );

  // Virtual IBAN revisions.
  $data['commerce_hipay_tpp_virtual_iban_revision']['revision_id'] = array(
    'title' => t('Revision ID'),
    'help' => t('The revision ID of the Virtual IBAN revision.'),
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
      'base' => 'commerce_hipay_tpp_virtual_iban',
      'base field' => 'revision_id',
      'title' => t('Virtual IBAN'),
      'label' => t('Latest Virtual IBAN revision'),
    ),
  );
  $data['commerce_hipay_tpp_virtual_iban_revision']['revision_uid'] = array(
    'title' => t('User'),
    'help' => t('Relate Virtual IBAN revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'revision_uid',
      'field_name' => 'revision_uid',
      'label' => t('Revision user'),
    ),
  );
  $data['commerce_hipay_tpp_virtual_iban_revision']['log'] = array(
    'title' => t('Log message'),
    'help' => t('The log message entered when the revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['commerce_hipay_tpp_virtual_iban_revision']['revision_timestamp'] = array(
    'title' => t('Revision date'),
    'help' => t('The date the Virtual IBAN revision was created.'),
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

  $data['commerce_payment_transaction']['data'] = array(
    'title' => t('Data'),
    'help' => t('The serialized data of the transaction.'),
    'field' => array(
      'handler' => 'views_handler_field_serialized',
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

}

/**
 * Provides default views for the Virtual IBAN entities.
 *
 * @see commerce_hipay_tpp_views_default_views()
 */
function commerce_hipay_tpp_virtual_iban_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'hipay_tpp_virtual_ibans';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_tpp_virtual_iban';
  $view->human_name = 'Hipay TPP: Virtual IBANs';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Virtual IBANs';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view any commerce_hipay_tpp_virtual_iban entity';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'virtual_iban_id' => 'virtual_iban_id',
    'bic' => 'bic',
    'iban' => 'iban',
    'external_reference' => 'external_reference',
    'expires' => 'expires',
    'order_id' => 'order_id',
    'name' => 'name',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = 'virtual_iban_id';
  $handler->display->display_options['style_options']['info'] = array(
    'virtual_iban_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'bic' => array(
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
    'external_reference' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'expires' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_id' => array(
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
    'operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  /* Relationship: Hipay Virtual IBAN: Owner */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'commerce_hipay_tpp_virtual_iban';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['virtual_iban_order_target_id']['id'] = 'virtual_iban_order_target_id';
  $handler->display->display_options['relationships']['virtual_iban_order_target_id']['table'] = 'field_data_virtual_iban_order';
  $handler->display->display_options['relationships']['virtual_iban_order_target_id']['field'] = 'virtual_iban_order_target_id';
  /* Field: Hipay Virtual IBAN: Virtual IBAN ID */
  $handler->display->display_options['fields']['virtual_iban_id']['id'] = 'virtual_iban_id';
  $handler->display->display_options['fields']['virtual_iban_id']['table'] = 'commerce_hipay_tpp_virtual_iban';
  $handler->display->display_options['fields']['virtual_iban_id']['field'] = 'virtual_iban_id';
  $handler->display->display_options['fields']['virtual_iban_id']['label'] = 'ID';
  $handler->display->display_options['fields']['virtual_iban_id']['link_to_entity'] = TRUE;
  /* Field: Hipay Virtual IBAN: BIC */
  $handler->display->display_options['fields']['bic']['id'] = 'bic';
  $handler->display->display_options['fields']['bic']['table'] = 'commerce_hipay_tpp_virtual_iban';
  $handler->display->display_options['fields']['bic']['field'] = 'bic';
  /* Field: Hipay Virtual IBAN: IBAN */
  $handler->display->display_options['fields']['iban']['id'] = 'iban';
  $handler->display->display_options['fields']['iban']['table'] = 'commerce_hipay_tpp_virtual_iban';
  $handler->display->display_options['fields']['iban']['field'] = 'iban';
  /* Field: Hipay Virtual IBAN: External reference */
  $handler->display->display_options['fields']['external_reference']['id'] = 'external_reference';
  $handler->display->display_options['fields']['external_reference']['table'] = 'commerce_hipay_tpp_virtual_iban';
  $handler->display->display_options['fields']['external_reference']['field'] = 'external_reference';
  /* Field: Hipay Virtual IBAN: Expire date */
  $handler->display->display_options['fields']['expires']['id'] = 'expires';
  $handler->display->display_options['fields']['expires']['table'] = 'commerce_hipay_tpp_virtual_iban';
  $handler->display->display_options['fields']['expires']['field'] = 'expires';
  $handler->display->display_options['fields']['expires']['label'] = 'Expires';
  $handler->display->display_options['fields']['expires']['date_format'] = 'short';
  $handler->display->display_options['fields']['expires']['second_date_format'] = 'long';
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
  $handler->display->display_options['fields']['order_id']['relationship'] = 'virtual_iban_order_target_id';
  $handler->display->display_options['fields']['order_id']['group_type'] = 'group_concat_distinct';
  $handler->display->display_options['fields']['order_id']['label'] = 'Order';
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 'admin';
  /* Field: Hipay Virtual IBAN: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_hipay_tpp_virtual_iban';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = '';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-tpp/virtual-ibans/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['description'] = 'Find and manage Hipay Virtual IBANs.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Virtual IBANs';
  $handler->display->display_options['tab_options']['description'] = 'Manage Hipay Virtual IBANs.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['hipay_tpp_virtual_ibans'] = array(
    t('Master'),
    t('Virtual IBANs'),
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
    t('Virtual IBAN owner'),
    t('Commerce Order entity referenced from virtual_iban_order'),
    t('ID'),
    t('.'),
    t(','),
    t('BIC'),
    t('IBAN'),
    t('External reference'),
    t('Expires'),
    t('Owner'),
    t('Order'),
    t('Page'),
  );
  $export['hipay_tpp_virtual_ibans'] = $view;

  $view = new view();
  $view->name = 'hipay_tpp_virtual_iban_revisions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_tpp_virtual_iban_revision';
  $view->human_name = 'Hipay TPP: Virtual IBAN revisions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Revisions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer commerce_hipay_tpp_virtual_iban entities';
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
    'bic' => 'bic',
    'iban' => 'iban',
    'external_reference' => 'external_reference',
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
    'bic' => array(
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
    'external_reference' => array(
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
  /* Relationship: Hipay Virtual IBAN Revisions: User */
  $handler->display->display_options['relationships']['revision_uid']['id'] = 'revision_uid';
  $handler->display->display_options['relationships']['revision_uid']['table'] = 'commerce_hipay_tpp_virtual_iban_revision';
  $handler->display->display_options['relationships']['revision_uid']['field'] = 'revision_uid';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['virtual_iban_order_target_id']['id'] = 'virtual_iban_order_target_id';
  $handler->display->display_options['relationships']['virtual_iban_order_target_id']['table'] = 'field_revision_virtual_iban_order';
  $handler->display->display_options['relationships']['virtual_iban_order_target_id']['field'] = 'virtual_iban_order_target_id';
  /* Field: Hipay Virtual IBAN Revisions: Revision ID */
  $handler->display->display_options['fields']['revision_id']['id'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['table'] = 'commerce_hipay_tpp_virtual_iban_revision';
  $handler->display->display_options['fields']['revision_id']['field'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['label'] = 'Revision';
  /* Field: Hipay Virtual IBAN Revisions: Revision date */
  $handler->display->display_options['fields']['revision_timestamp']['id'] = 'revision_timestamp';
  $handler->display->display_options['fields']['revision_timestamp']['table'] = 'commerce_hipay_tpp_virtual_iban_revision';
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
  /* Field: Hipay Virtual IBAN Revisions: BIC number */
  $handler->display->display_options['fields']['bic']['id'] = 'bic';
  $handler->display->display_options['fields']['bic']['table'] = 'commerce_hipay_tpp_virtual_iban_revision';
  $handler->display->display_options['fields']['bic']['field'] = 'bic';
  /* Field: Hipay Virtual IBAN Revisions: IBAN number */
  $handler->display->display_options['fields']['iban']['id'] = 'iban';
  $handler->display->display_options['fields']['iban']['table'] = 'commerce_hipay_tpp_virtual_iban_revision';
  $handler->display->display_options['fields']['iban']['field'] = 'iban';
  /* Field: Hipay Virtual IBAN Revisions: External reference */
  $handler->display->display_options['fields']['external_reference']['id'] = 'external_reference';
  $handler->display->display_options['fields']['external_reference']['table'] = 'commerce_hipay_tpp_virtual_iban_revision';
  $handler->display->display_options['fields']['external_reference']['field'] = 'external_reference';
  /* Field: Commerce Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['relationship'] = 'virtual_iban_order_target_id';
  $handler->display->display_options['fields']['order_id']['label'] = 'Order';
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 'admin';
  /* Field: Hipay Virtual IBAN Revisions: Log message */
  $handler->display->display_options['fields']['log']['id'] = 'log';
  $handler->display->display_options['fields']['log']['table'] = 'commerce_hipay_tpp_virtual_iban_revision';
  $handler->display->display_options['fields']['log']['field'] = 'log';
  /* Contextual filter: Hipay Virtual IBAN Revisions: Virtual IBAN ID */
  $handler->display->display_options['arguments']['virtual_iban_id']['id'] = 'virtual_iban_id';
  $handler->display->display_options['arguments']['virtual_iban_id']['table'] = 'commerce_hipay_tpp_virtual_iban_revision';
  $handler->display->display_options['arguments']['virtual_iban_id']['field'] = 'virtual_iban_id';
  $handler->display->display_options['arguments']['virtual_iban_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['virtual_iban_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['virtual_iban_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['virtual_iban_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['virtual_iban_id']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-tpp/virtual-ibans/%/revisions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'revisions';
  $handler->display->display_options['menu']['description'] = 'View revisions of this Virtual IBAN.';
  $handler->display->display_options['menu']['weight'] = '20';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['hipay_tpp_virtual_iban_revisions'] = array(
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
    t('Commerce Order entity referenced from virtual_iban_order'),
    t('Revision'),
    t('Created on'),
    t('Created by'),
    t('BIC number'),
    t('IBAN number'),
    t('External reference'),
    t('Order'),
    t('Log message'),
    t('All'),
    t('Page'),
  );
  $export['hipay_tpp_virtual_iban_revisions'] = $view;

  $view = new view();
  $view->name = 'hipay_tpp_virtual_iban_payments';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_payment_transaction';
  $view->human_name = 'Payment transactions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Unassigned payment transactions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer payments';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'transaction_id' => 'transaction_id',
    'status' => 'status',
    'created' => 'created',
    'remote_id' => 'remote_id',
    'data_1' => 'data_1',
    'data' => 'data',
    'message' => 'message',
    'amount' => 'amount',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = 'transaction_id';
  $handler->display->display_options['style_options']['info'] = array(
    'transaction_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'remote_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'data_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'data' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'message' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
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
  $handler->display->display_options['empty']['area']['content'] = 'No unassigned payment transactions found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Commerce Payment Transaction: Transaction ID */
  $handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['label'] = 'ID';
  /* Field: Commerce Payment Transaction: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Commerce Payment Transaction: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Commerce Payment Transaction: Remote ID */
  $handler->display->display_options['fields']['remote_id']['id'] = 'remote_id';
  $handler->display->display_options['fields']['remote_id']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['fields']['remote_id']['field'] = 'remote_id';
  /* Field: Commerce Payment Transaction: Data */
  $handler->display->display_options['fields']['data_1']['id'] = 'data_1';
  $handler->display->display_options['fields']['data_1']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['fields']['data_1']['field'] = 'data';
  $handler->display->display_options['fields']['data_1']['label'] = 'External reference';
  $handler->display->display_options['fields']['data_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['data_1']['format'] = 'key';
  $handler->display->display_options['fields']['data_1']['key'] = 'commerce_hipay_tpp_virtual_iban_external_reference';
  /* Field: Commerce Payment Transaction: Data */
  $handler->display->display_options['fields']['data']['id'] = 'data';
  $handler->display->display_options['fields']['data']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['fields']['data']['field'] = 'data';
  $handler->display->display_options['fields']['data']['label'] = 'Virtual IBAN';
  $handler->display->display_options['fields']['data']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['data']['alter']['text'] = '<a href="/admin/commerce/hipay-tpp/virtual-ibans/[data]">[data_1]</a> ([data])';
  $handler->display->display_options['fields']['data']['format'] = 'key';
  $handler->display->display_options['fields']['data']['key'] = 'commerce_hipay_tpp_virtual_iban_id';
  /* Field: Commerce Payment Transaction: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  /* Field: Commerce Payment Transaction: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<ul class="links inline operations">
<li><a href="' . $GLOBALS['base_url'] . '/admin/commerce/hipay-tpp/virtual-ibans/payment-transactions/[transaction_id]">view</a></li>
<li><a href="' . $GLOBALS['base_url'] . '/admin/commerce/hipay-tpp/virtual-ibans/payment-transactions/[transaction_id]/assign-to-order">assign to order</a></li>
</ul>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Filter criterion: Commerce Payment Transaction: Payment method */
  $handler->display->display_options['filters']['payment_method']['id'] = 'payment_method';
  $handler->display->display_options['filters']['payment_method']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['filters']['payment_method']['field'] = 'payment_method';
  $handler->display->display_options['filters']['payment_method']['value'] = array(
    'commerce_hipay_tpp_virtual_iban' => 'commerce_hipay_tpp_virtual_iban',
  );
  /* Filter criterion: Commerce Payment Transaction: Order ID */
  $handler->display->display_options['filters']['order_id']['id'] = 'order_id';
  $handler->display->display_options['filters']['order_id']['table'] = 'commerce_payment_transaction';
  $handler->display->display_options['filters']['order_id']['field'] = 'order_id';
  $handler->display->display_options['filters']['order_id']['value']['value'] = '0';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-tpp/virtual-ibans/payment-transactions/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['description'] = 'Find and manage Hipay Virtual IBAN payment transactions.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Unassigned payment transactions';
  $handler->display->display_options['tab_options']['description'] = 'Manage Hipay Virtual IBAN payment transactions not assigned to any order.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['hipay_tpp_virtual_iban_payments'] = array(
    t('Master'),
    t('Unassigned payment transactions'),
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
    t('No unassigned payment transactions found.'),
    t('ID'),
    t('.'),
    t(','),
    t('Status'),
    t('Date'),
    t('Remote ID'),
    t('External reference'),
    t('Virtual IBAN'),
    t('<a href="/admin/commerce/hipay-tpp/virtual-ibans/[data]">[data_1]</a> ([data])'),
    t('Message'),
    t('Amount'),
    t('<ul class="links inline operations">
<li><a href="/admin/commerce/hipay-tpp/virtual-ibans/payment-transactions/[transaction_id]">view</a></li>
<li><a href="/admin/commerce/hipay-tpp/virtual-ibans/payment-transactions/[transaction_id]/assign-to-order">assign to order</a></li>
</ul>'),
    t('Page'),
  );
  $export['hipay_tpp_virtual_iban_payments'] = $view;

  return $export;
}

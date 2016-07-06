<?php

/**
 * Returns Hipay transfer-related menu items.
 *
 * @see commerce_hipay_ws_menu()
 */
function commerce_hipay_ws_transfer_menu() {
  $items = array();

  $items['admin/commerce/hipay-wallet/transfers/%commerce_hipay_ws_transfer'] = array(
    'title callback' => 'commerce_hipay_ws_transfer_title',
    'title arguments' => array(4),
    'page callback' => 'commerce_hipay_ws_transfer_view',
    'page arguments' => array(4),
    'access callback' => 'commerce_hipay_ws_transfer_access',
    'access arguments' => array('view', 4),
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.transfer.inc',
  );

  $items['admin/commerce/hipay-wallet/transfers/%commerce_hipay_ws_transfer/view'] = array(
    'title' => 'view',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.transfer.inc',
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
function commerce_hipay_ws_transfer_access($op, $entity = NULL, $account = NULL) {
  switch ($op) {
    case 'create':
      return user_access('create commerce_hipay_ws_transfer entities');
    case 'view':
      return user_access('view any commerce_hipay_ws_transfer entity');
    case 'edit':
      return user_access('edit any commerce_hipay_ws_transfer entity');
    case 'delete':
      return user_access('administer commerce_hipay_ws_transfer entities');
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
 * @see commerce_hipay_ws_transfer_menu()
 */
function commerce_hipay_ws_transfer_view($entity, $view_mode = 'full') {
  return entity_view('commerce_hipay_ws_transfer', array($entity->transfer_id => $entity), $view_mode, NULL, TRUE);
}

<?php
/**
 * @file
 * watchtower_notifications.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function watchtower_notifications_user_default_roles() {
  $roles = array();

  // Exported role: watchtower.
  $roles['watchtower'] = array(
    'name' => 'watchtower',
    'weight' => 3,
  );

  return $roles;
}

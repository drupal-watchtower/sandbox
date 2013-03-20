<?php

define('WATCHTOWER_MODULES_VOCABULARY', 'modules');
define('WATCHTOWER_PACKAGES_VOCABULARY', 'package');

define('WATCHTOWER_MODULES_VID', taxonomy_vocabulary_machine_name_load(WATCHTOWER_MODULES_VOCABULARY)->vid);
define('WATCHTOWER_PACKAGES_VID', taxonomy_vocabulary_machine_name_load(WATCHTOWER_PACKAGES_VOCABULARY)->vid);

/**
 * Returns instance's node or creates one if need.
 *
 * @param string $watchtower_key
 *   String identifying drupal instance.
 * @param string $server_ip
 *   Watchtower client ip address.
 * @param string $drupal_site_root
 *   Watchtower Client drupal instance site directory.
 *
 * @return Object
 *   Corresponing node object.
 */
function _dw_server_get_instance($watchtower_key, $server_ip, $drupal_site_root) {
  // Lookup for existing instance's node
  $query = new EntityFieldQuery();
  $results = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'drupal_instance')
    ->fieldCondition('field_watchtower_key', 'value', $watchtower_key)
    ->fieldCondition('field_server_ip', 'value', $server_ip)
    ->fieldCondition('field_drupal_root', 'value', $drupal_site_root)
    ->execute();

  if (!empty($results['node'])) {
    $node = node_load(reset($results['node'])->nid);
  }
  else {
    $node = NULL;
    $node->is_new = TRUE;
    $node->type = 'drupal_instance';
    $node->title = $server_ip . '/' . $drupal_site_root;
    $node->language = LANGUAGE_NONE;
    $node->field_watchtower_key[LANGUAGE_NONE][] = array('value' => $watchtower_key);
    $node->field_server_ip[LANGUAGE_NONE][] = array('value' => $server_ip);
    $node->field_drupal_root[LANGUAGE_NONE][] = array('value' => $drupal_site_root);
    node_save($node);
  }

  return $node;
}

/**
 * Returns tids of modules terms.
 * Creates terms if need.
 *
 * @param $modules array
 *
 * @return array
 */
function _dw_server_get_modules_tids($modules) {
  $modules_tids = array();

  foreach ($modules as $module) {
    if (!$package = reset(taxonomy_get_term_by_name($module['package'], WATCHTOWER_PACKAGES_VOCABULARY))) {
      $package = NULL;
      $package->vid = WATCHTOWER_PACKAGES_VID;
      $package->name = $module['package'];
      taxonomy_term_save($package);
    }
    if (!$module_term = reset(taxonomy_get_term_by_name($module['machine_name'], WATCHTOWER_MODULES_VOCABULARY))) {
      $module_term = NULL;
      $module_term->vid = WATCHTOWER_MODULES_VID;
      $module_term->name = $module['machine_name'];
      $module_term->description = $module['description'];
      $module_term->field_package[LANGUAGE_NONE][] = array('tid' => $package->tid);
      taxonomy_term_save($module_term);
    }

    $module_version_name = $module['machine_name'] . '-' . $module['version'];
    if (!$module_version_term = reset(taxonomy_get_term_by_name($module_version_name, WATCHTOWER_MODULES_VOCABULARY))) {
      $module_version_term = NULL;
      $module_version_term->vid = WATCHTOWER_MODULES_VID;
      $module_version_term->name = $module_version_name;
      $module_version_term->parent = $module_term->tid;
      taxonomy_term_save($module_version_term);
    }

    $modules_tids[] = $module_version_term->tid;
  }

  return $modules_tids;
}

/**
 * Retrieve patches nids by their description.
 *
 * @param array $patches
 *
 * @return array
 */
function _dw_server_get_patches_nids($patches) {
  $patches_nids = array();

  foreach ($patches as $patch_name => $module_name_and_version) {
    // Lookup for existing instance's node
    $query = new EntityFieldQuery();
    $results = $query
      ->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'patch')
      ->propertyCondition('title', $patch_name)
      ->execute();

    if (!empty($results['node'])) {
      $node = node_load(reset($results['node'])->nid);
    }
    else {
      $node = NULL;
      $node->is_new = TRUE;
      $node->type = 'patch';
      $node->title = $patch_name;
      $node->language = LANGUAGE_NONE;
      $node->field_priority[LANGUAGE_NONE][] = array('value' => 'normal');
      $node->field_status[LANGUAGE_NONE][] = array('value' => 'new');
      list($module_name, $module_version_name) = explode(':', $module_name_and_version);
      if ($module_term = reset(taxonomy_get_term_by_name($module_name, WATCHTOWER_MODULES_VOCABULARY))) {
        $node->field_applicable_to[LANGUAGE_NONE][] = array('tid' => $module_term->tid);
      }
      if ($module_term_version = reset(taxonomy_get_term_by_name($module_version_name, WATCHTOWER_MODULES_VOCABULARY))) {
        $node->field_applicable_to[LANGUAGE_NONE][] = array('tid' => $module_term_version->tid);
      }
      node_save($node);
    }

    $patches_nids[] = $node->nid;
  }

  return $patches_nids;
}
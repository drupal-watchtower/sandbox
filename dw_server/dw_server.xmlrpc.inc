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
 * @param string $drupal_root
 *   Watchtower Client drupal instance site directory.
 * @param string $conf_path
 *   Watchtower Client drupal instance site directory.
 * @param string $server_ip
 *   Watchtower client ip address.
 *
 * @return Object
 *   Corresponing node object.
 */
function _dw_server_get_instance($watchtower_key, $drupal_root, $conf_path, $server_ip) {
  // Lookup for existing instance's node
  $query = new EntityFieldQuery();
  $results = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'drupal_instance')
    ->fieldCondition('field_watchtower_key', 'value', $watchtower_key)
    ->fieldCondition('field_drupal_root', 'value', $drupal_root)
    ->fieldCondition('field_configuration_path', 'value', $conf_path)
    // @todo Suppose makes no sense.
    ->fieldCondition('field_server_ip', 'value', $server_ip)
    ->execute();

  if (!empty($results['node'])) {
    $node = entity_metadata_wrapper('node', reset($results['node'])->nid);
    //$node = node_load(reset($results['node'])->nid);
  }
  else {
    $node = entity_create('node', array(
      'type' => 'drupal_instance',
      'title' => $server_ip . '/' . $drupal_root . '/' . $conf_path,
    ));
    $node = entity_metadata_wrapper('node', $node);
    $node->get('field_watchtower_key')->set($watchtower_key);
    $node->get('field_drupal_root')->set($drupal_root);
    $node->get('field_configuration_path')->set($conf_path);
    $node->get('field_server_ip')->set($server_ip);
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

  foreach ($modules as $name => $module) {
    $terms = taxonomy_get_term_by_name($module['package'], WATCHTOWER_PACKAGES_VOCABULARY);
    if (!$package = reset($terms)) {
      $package = new stdClass();
      $package->vid = WATCHTOWER_PACKAGES_VID;
      $package->name = $module['package'];
      taxonomy_term_save($package);
    }
    $terms = taxonomy_get_term_by_name($name, WATCHTOWER_MODULES_VOCABULARY);
    if (!$module_term = reset($terms)) {
      $module_term = new stdClass();
      $module_term->vid = WATCHTOWER_MODULES_VID;
      $module_term->name = $name;
      $module_term->description = $module['description'];
      $module_term->field_package[LANGUAGE_NONE][] = array('tid' => $package->tid);
      taxonomy_term_save($module_term);
    }

    $module_version_name = $name . '-' . $module['version'];
    $terms = taxonomy_get_term_by_name($module_version_name, WATCHTOWER_MODULES_VOCABULARY);
    if (!$module_version_term = reset($terms)) {
      $module_version_term = new stdClass();
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
 * Return IDs of nodes that store patches.
 *
 * @param array $module_patches
 *   Keyed array of modules with patches.
 *
 * @return array
 *  An array of node IDs of patches.
 */
function _dw_server_get_patches_nids($module_patches) {
  if (!$module_patches) {
    // Early exit to prevent DB access.
    return array();
  }

  $modules = array();
  foreach (array_keys($module_patches) as $module_name_and_version) {
    list($module_name, $module_version_name) = explode(':', $module_name_and_version);
    $modules[$module_name] = 0;
    $modules[$module_version_name] = 0;
  }

  // Assign IDs to modules, each module should be saved already.
  $query = new EntityFieldQuery();
  $results = $query
    ->entityCondition('entity_type', 'taxonomy_term')
    ->entityCondition('bundle', WATCHTOWER_MODULES_VOCABULARY)
    ->propertyCondition('name', array_keys($modules), 'IN')
    ->execute();
  if (!empty($results['taxonomy_term'])) {
    $terms = entity_load('taxonomy_term', array_keys($results['taxonomy_term']));
    foreach ($terms as $id => $term) {
      // Assign actual ID.
      $modules[$term->name] = $id;
    }
  }

  $patch_ids = array();
  foreach ($module_patches as $module_name_and_version => $patches) {
    list($module_name, $module_version_name) = explode(':', $module_name_and_version);
    // Get patches per module.
    $patches_create = drupal_map_assoc(array_values($patches));
    $query = new EntityFieldQuery();
    $results = $query
      ->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'patch')
      ->propertyCondition('title', $patches, 'IN')
      ->fieldCondition('field_fixed_in', 'tid', $modules[$module_name])
      ->execute();
    if (!empty($results['node'])) {
      $nodes = entity_load('node', array_keys($results['node']));
      foreach ($nodes as $id => $node) {
        $patch_ids[] = $id;
        unset($patches_create[$node->title]);
      }
    }
    // Create needed patches.
    foreach ($patches_create as $name => $v) {
      $node = entity_create('node', array(
        'type' => 'patch',
        'title' => $name,
      ));
      $node = entity_metadata_wrapper('node', $node);
      $node->get('field_fixed_in')->set($modules[$module_name]);
      $node->get('field_applicable_to')->set(array($modules[$module_version_name]));
      $node->save();
      $patch_ids[] = $node->get('nid')->value();
    }
  }

  return $patch_ids;
}

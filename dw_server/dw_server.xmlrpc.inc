<?php

/**
 * @file
 * Remains of drupal 7 server xmlrpc endpoints.
 */

/**
 * Implements hook_xmlrpc().
 *
 * @todo Get rid of https://www.drupal.org/node/1632592 use REST.
 */
function dw_server_xmlrpc() {
  return array(
    'drupal_watchtower.report' => 'dw_server_process_report',
  );
}

/**
 * Processes XML RPC request
 *
 * @param mixed $report_data
 *   The data reported by client.
 *
 * @return string
 */
function dw_server_process_report($report_data) {
  module_load_include('inc', 'dw_server', 'dw_server.xmlrpc');

  $default = array(
    'watchtower_key' => '',
    'drupal_root' => '',
    'modules' => array(),
    'metrics' => array(),
    'patches' => array(),
  );
  $data = array_merge($default, $report_data);

  $instance = _dw_server_get_instance($data['watchtower_key'], \Drupal::request()->getClientIp(), $data['drupal_root']);

  // Fills field_modules field.
  ksort($data['modules']);
  $instance->field_modules[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED] = array();
  if ($modules = _dw_server_get_modules_tids($data['modules'])) {
    foreach ($modules as $tid) {
      $instance->field_modules[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED][] = array('tid' => $tid);
    }
  }

  // Fills field_metrics field.
  ksort($data['metrics']);
  $instance->field_metrics[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED] = array();
  foreach ($data['metrics'] as $key => $value) {
    $instance->field_metrics[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED][] = array(
      'first' => $key,
      'second' => $value,
    );
  }

  // Fills field_patches field.
  ksort($data['patches']);
  $instance->field_patches[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED] = array();
  if ($patches = _dw_server_get_patches_nids($data['patches'])) {
    foreach ($patches as $nid) {
      $instance->field_patches[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED][] = array('target_id' => $nid);
    }
  }

  $instance->revision = TRUE;
  $instance->save();

  // @FIXME
// url() expects a route name or an external URI.
// return url('node/' . $instance->nid, array('absolute' => TRUE));

}

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
    $node = \Drupal::entityManager()->getStorage('node')->load(reset($results['node'])->nid);
  }
  else {
    $node = NULL;
    $node->is_new = TRUE;
    $node->type = 'drupal_instance';
    $node->title = $server_ip . '/' . $drupal_site_root;
    $node->language = \Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED;
    $node->field_watchtower_key[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED][] = array('value' => $watchtower_key);
    $node->field_server_ip[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED][] = array('value' => $server_ip);
    $node->field_drupal_root[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED][] = array('value' => $drupal_site_root);
    $node->save();
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
      $package->save();
    }
    if (!$module_term = reset(taxonomy_get_term_by_name($module['machine_name'], WATCHTOWER_MODULES_VOCABULARY))) {
      $module_term = NULL;
      $module_term->vid = WATCHTOWER_MODULES_VID;
      $module_term->name = $module['machine_name'];
      $module_term->description = $module['description'];
      $module_term->field_package[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED][] = array('tid' => $package->tid);
      $module_term->save();
    }

    $module_version_name = $module['machine_name'] . '-' . $module['version'];
    if (!$module_version_term = reset(taxonomy_get_term_by_name($module_version_name, WATCHTOWER_MODULES_VOCABULARY))) {
      $module_version_term = NULL;
      $module_version_term->vid = WATCHTOWER_MODULES_VID;
      $module_version_term->name = $module_version_name;
      $module_version_term->parent = $module_term->tid;
      $module_version_term->save();
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
      $node = \Drupal::entityManager()->getStorage('node')->load(reset($results['node'])->nid);
    }
    else {
      $node = NULL;
      $node->is_new = TRUE;
      $node->type = 'patch';
      $node->title = $patch_name;
      $node->language = \Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED;
      $node->field_priority[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED][] = array('value' => 'normal');
      $node->field_status[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED][] = array('value' => 'new');
      list($module_name, $module_version_name) = explode(':', $module_name_and_version);
      if ($module_term = reset(taxonomy_get_term_by_name($module_name, WATCHTOWER_MODULES_VOCABULARY))) {
        $node->field_applicable_to[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED][] = array('tid' => $module_term->tid);
      }
      if ($module_term_version = reset(taxonomy_get_term_by_name($module_version_name, WATCHTOWER_MODULES_VOCABULARY))) {
        $node->field_applicable_to[\Drupal\Core\Language\Language::LANGCODE_NOT_SPECIFIED][] = array('tid' => $module_term_version->tid);
      }
      $node->save();
    }

    $patches_nids[] = $node->nid;
  }

  return $patches_nids;
}

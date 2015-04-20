<?php
/**
 * @file
 * watchtower_storage.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function watchtower_storage_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function watchtower_storage_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function watchtower_storage_node_info() {
  $items = array(
    'drupal_instance' => array(
      'name' => t('Drupal instance'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'patch' => array(
      'name' => t('Patch'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

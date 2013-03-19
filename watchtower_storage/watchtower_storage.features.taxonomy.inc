<?php
/**
 * @file
 * watchtower_storage.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function watchtower_storage_taxonomy_default_vocabularies() {
  return array(
    'modules' => array(
      'name' => 'Modules',
      'machine_name' => 'modules',
      'description' => 'The dictionary about modules and versions of modules',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
    'package' => array(
      'name' => 'Package',
      'machine_name' => 'package',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
  );
}

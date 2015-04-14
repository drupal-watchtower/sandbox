<?php

/**
 * @file
 * Watchtower Drush commands.
 */

/**
 * Implements hook_drush_command().
 */
function dw_client_drush_command() {
  $items = array(
    'dw_report' => array(
      'aliases' => array('dwr'),
      'callback' => 'dw_client_drush_report',
      'core' => array('7'),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'description' => 'Report configured metrics to Watchtower server.',
      'arguments' => array(
      ),
      'options' => array(
        'url' => 'Server url to report.',
        'metrics' => 'Categories to report (not supported).',
        'key' => 'Auth key (site machine name).',
      ),
      'examples' => array(
        'drush dwr' => 'Reports metrics to configured server.',
        'drush dwr --url=https://example.com/xmlrpc.php' => 'Same as above, but using the server.',
      ),
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function dw_client_drush_help($section) {
  switch ($section) {
    case 'meta:dw_client:title':
      return dt('Watchtower commands');
    case 'meta:dw_client:summary':
      return dt('Watchtower drush commands.');
  }
}

/**
 * Drush callback: dw_report.
 */
function dw_client_drush_report() {
  $msg_success = dt('Report has been send.');
  $msg_error = dt('Report has not been send.');

  $url = drush_get_option('url');
  if (!valid_url($url, TRUE)) {
    if (module_exists('dw_client')) {
      if (_dw_client_report(TRUE)) {
        drush_log($msg_success, 'success');
      }
      else {
        drush_log($msg_error, 'error');
      }
      return;
    }
    $msg = dt('Watchtower server url missing and no client configured.');
    drush_log($msg, 'notice');
    return;
  }

  $private_key = drupal_get_private_key();
  $hash_salt = drupal_get_hash_salt();
  // @todo Use better hash.
  $watchtower_site_key = drupal_hmac_base64('Watchtower', $private_key . $hash_salt);
  $drupal_root = DRUPAL_ROOT;
  $conf_path = conf_path();
  $instance_report = array(
    'watchtower_key' => $watchtower_site_key,
    'drupal_root' => $drupal_root,
    'conf_path' => $conf_path,
    'metrics' => array(
      'drush:version' => DRUSH_VERSION,
      // Add standard metrics.
      'php:version' => phpversion(),
      'php:max_execution_time' => ini_get('max_execution_time'),
      'php:memory_limit' => ini_get('memory_limit'),
      'site:core:version' => drush_drupal_version(),
    ),
  );

  if ($key = drush_get_option('key')) {
    // @todo Decide on bundle
    $metrics['key'] = $key;
  }
  else {
    $metrics['key'] = 'Anonymous';
  }

  // Extend standard metrics.
  $metrics = &$instance_report['metrics'];

  $metrics['site:user:max'] = db_query("SELECT MAX(uid) FROM {users}")->fetchField();
  $metrics['site:user:count'] = db_query("SELECT COUNT(uid) FROM {users}")->fetchField();

  if (module_exists('dblog')) {
    $metrics['site:dblog:max'] = db_query("SELECT MAX(wid) FROM {watchdog}")->fetchField();
    $metrics['site:dblog:count'] = db_query("SELECT COUNT(wid) FROM {watchdog}")->fetchField();
  }

  // Add modules and patches.
  $modules_save = array();
  // Get current list of modules.
  $modules = system_rebuild_module_data();

  foreach ($modules as $machine_name => $module) {
    if (empty($module->info['hidden']) && $module->status && $module->info['package'] !== 'Core') {
      // Send only not hidden, enabled and non-core modules from display list.
      $modules_save[$machine_name] = array(
        'name' => $module->info['name'],
        'description' => $module->info['description'],
        'package' => $module->info['package'],
        'version' => $module->info['version'],
      );
    }
  }
  // @todo Configure as options.
  $metrics['site:modules'] = $modules_save;

  if ($result = xmlrpc($url, array('drupal_watchtower.report' => array($instance_report)))) {
    $msg = dt('Report has been send. Response !r', array('!r' => $result));
    drush_log($msg, 'success');
  }
  else {
    drush_log($msg_error, 'error');
  }
}

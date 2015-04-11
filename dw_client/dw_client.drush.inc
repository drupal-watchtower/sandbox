<?php

/**
 * @file
 * Watchtower Drush commands.
 */

use Drupal\Component\Utility\Crypt;
use Drupal\Component\Utility\UrlHelper;
use Drupal\Core\Site\Settings;

/**
 * Implements hook_drush_command().
 */
function dw_client_drush_command() {
  $items = array(
    'dw_report' => array(
      'aliases' => array('dwr'),
      'callback' => 'dw_client_drush_report',
      'core' => array('8+'),
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
  $module_hander = \Drupal::moduleHandler();

  $url = drush_get_option('url');
  if (!UrlHelper::isValid($url, TRUE)) {
    if ($module_hander->moduleExists('dw_client')) {
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

  $private_key = \Drupal::service('private_key')->get();
  $hash_salt = Settings::getHashSalt();
  $drupal_root = DRUPAL_ROOT;
  $conf_path = conf_path();
  $instance_report = [
    // @todo Use better hash.
    'watchtower_key' => Crypt::hmacBase64('Watchtower', $private_key . $hash_salt),
    // @todo Revisit deprecated.
    'drupal_root' => $drupal_root,
    'conf_path' => $conf_path,
    'metrics' => [
      'drush:version' => drush_core_version(),
      // Add standard metrics.
      'php:version' => phpversion(),
      'php:max_execution_time' => ini_get('max_execution_time'),
      'php:memory_limit' => ini_get('memory_limit'),
      'site:core:version' => drush_drupal_version(),
    ],
  ];

  if ($key = drush_get_option('key')) {
    // @todo Decide on bundle
    $metrics['key'] = $key;
  }
  else {
    $metrics['key'] = 'Anonymous';
  }

  // Add modules and patches.
  $modules = $module_hander->getModuleList();
  // @todo Configure as options.
  $metrics['site:modules'] = $modules;
  // $metrics['site:patches'] = $modules;

  if ($module_hander->moduleExists('dblog')) {
    $metrics['site:dblog:max'] = \Drupal::database()
      ->query("SELECT MAX(wid) FROM {watchdog}")
      ->fetchField();
  }

  $metrics['site:user:count'] = \Drupal::entityQuery('user')
    ->count()
    ->range(0, 1)
    ->execute();

  if ($result = xmlrpc($url, ['drupal_watchtower.report' => [$instance_report]])) {
    $msg = dt('Report has been send. Response !r', ['!r' => $result]);
    drush_log($msg, 'success');
  }
  else {
    drush_log($msg_error, 'error');
  }
}


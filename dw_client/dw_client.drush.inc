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
      'description' => 'Report configured statisticts to Watchtower server.',
      'arguments' => array(
        'url'    => 'Server url to report.',
        'key'    => 'Auth key (not supported).',
        'types'  => 'Categories to report (not supported).',
      ),
      'options' => array(
      ),
      'examples' => array(
        'drush dwr' => 'Reports metrics to configured server.',
        'drush dwr --key=some_hash' => 'Same as above, but using a hash to authorize.',
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
function dw_client_drush_report($url = '', $key = '', $types = array()) {
  $result = _dw_client_report(TRUE);
  if ($result) {
    $msg = dt('Report has been send.');
    drush_log($msg, 'success');
  }
  else {
    $msg = dt('Report has not been send.');
    drush_log($msg, 'error');
  }
}

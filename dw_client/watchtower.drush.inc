<?php

/**
 * @file
 * Watchtower Drush commands.
 */

/**
 * Implements hook_drush_command().
 */
function watchtower_drush_command() {
  return array(
    'dw_report' => array(
      'aliases' => array('dwr'),
      'callback' => 'watchtower_drush_report',
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
    'dw_report_nginx' => array(
      'aliases' => array('dwn'),
      'callback' => 'watchtower_drush_report_nginx',
      'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
      'description' => 'Report configured metrics to Watchtower server for all nginx sites defined.',
      'arguments' => array(
        'url' => 'The url of report server',
        'folder' => 'Nginx folder to scan, default to /etc/nginx/sites-enabled',
      ),
      'options' => array(
      ),
      'examples' => array(
        'drush dwn https://example.com/xml.rpc' => 'Reports metrics to server.',
      ),
    ),
  );
}

/**
 * Implements hook_drush_help().
 */
function watchtower_drush_help($section) {
  switch ($section) {
    case 'meta:watchtower:title':
      return dt('Watchtower commands');
    case 'meta:watchtower:summary':
      return dt('Watchtower drush commands.');
  }
}

/**
 * Drush callback: dw_report.
 */
function watchtower_drush_report() {
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
    drush_log($msg, 'error');
    return;
  }

  if ($key = drush_get_option('key')) {
    // @todo Decide on bundle
    $metrics['key'] = $key;
  }
  else {
    $metrics['key'] = 'Anonymous';
  }

  $report = watchtower_get_default_report();
  // Add drush metrics.
  $report['metrics']['drush:version'] = DRUSH_VERSION;

  if ($result = xmlrpc($url, array('drupal_watchtower.report' => array($report)))) {
    $msg = dt('Report has been send. Response !r', array('!r' => $result));
    drush_log($msg, 'success');
  }
  else {
    drush_log($msg_error, 'error');
  }
}

/**
 * Returns the drupal instance site hash.
 *
 * @return string
 *   The key for the drupal instance.
 */
function watchtower_get_drupal_site_key() {
  $private_key = drupal_get_private_key();
  $hash_salt = drupal_get_hash_salt();
  // @todo Use better hash.
  return drupal_hmac_base64('Watchtower', $private_key . $hash_salt);
}

/**
 * Returns array of default report with metrics.
 *
 * @return array
 *   Keyed array with metrics and their values.
 */
function watchtower_get_default_report() {
  $watchtower_site_key = watchtower_get_drupal_site_key();
  $drupal_root = DRUPAL_ROOT;
  $conf_path = conf_path();
  $memory_limit = ini_get('memory_limit');
  $report = array(
    'watchtower_key' => $watchtower_site_key,
    'drupal_root' => $drupal_root,
    'conf_path' => $conf_path,
    'metrics' => array(
      // Add standard metrics.
      'php:version' => phpversion(),
      'php:max_execution_time' => ini_get('max_execution_time'),
      'php:memory_limit' => $memory_limit,
      'php:memory_limit_formatted' => parse_size($memory_limit),

    ),
  );
  $metrics = &$report['metrics'];

  if (defined('VERSION')) {
    $metrics['site:core:version'] = VERSION;
  }
  $metrics['site:core:base_url'] = url(NULL, array('absolute' => TRUE));

  $metrics['site:user:max'] = db_query("SELECT MAX(uid) FROM {users}")->fetchField();
  $metrics['site:user:count'] = db_query("SELECT COUNT(uid) FROM {users}")->fetchField();

  if (module_exists('dblog')) {
    $metrics['site:dblog:max'] = db_query("SELECT MAX(wid) FROM {watchdog}")->fetchField();
    $metrics['site:dblog:count'] = db_query("SELECT COUNT(wid) FROM {watchdog}")->fetchField();
  }

  if (module_exists('dw_client')) {
    // Gather metrics with hook.
    if ($hook_metrics = module_invoke_all('dw_metric')) {
      drupal_array_merge_deep($metrics, $hook_metrics);
    }
  }

  // Add modules.
  $modules_save = array();
  // Get current list of all modules.
  $modules = system_rebuild_module_data();

  foreach ($modules as $machine_name => $module) {
    if (empty($module->info['hidden']) && $module->status && $module->info['package'] !== 'Core') {
      // Send only not hidden, enabled and non-core modules from display list.
      $modules_save[$machine_name] = array(
        'name' => $module->info['name'],
        'description' => $module->info['description'],
        'package' => $module->info['package'],
        // Convert NULL to string because server will convert them to spaces and
        // hash calculation could be broken.
        'version' => empty($module->info['version']) ? '' : $module->info['version'],
      );
    }
  }
  $metrics['site:modules'] = $modules_save;

  // Add patches.
  // @todo Configure recursive option.
  if ($patches = watchtower_search_patches($modules)) {
    $metrics['site:patches'] = $patches;
  }

  return $report;
}

/**
 * Retrieves patches for a list of modules grouped by module version name.
 *
 * @param array $modules
 *   List of modules in format of system_rebuild_module_data() function.
 * @param bool $recursive
 *   Flag defining that patches should be searched recursively inside modules'
 *   directories.
 *
 * @return array
 *   Keyed array of module's patches.
 */
function watchtower_search_patches($modules, $recursive = FALSE) {
  $patches = array();
  foreach ($modules as $module_name => $module) {
    $module_version_name = $module_name . '-' . $module->info['version'];
    $module_path = dirname($module->filename);
    $module_patches = array();
    if ($recursive) {
      // Recursive version - looks thru whole module directory structure
      $directory = new \RecursiveDirectoryIterator($module_path);
      $flattened = new \RecursiveIteratorIterator($directory);
      $files = new \RegexIterator($flattened, '/.*\.patch/i');
      foreach ($files as $file) {
        $module_patches[] = $file->getPath();
      }
    }
    else {
      // Non-recursive version - only modules root directory to be scanned
      $module_patches = glob($module_path . '/*.patch');
    }

    if ($module_patches) {
      foreach ($module_patches as &$patch_path) {
        $patch_path = basename($patch_path);
      }
      $patches[$module_name . ':' . $module_version_name] = $module_patches;
    }
  }

  return $patches;
}

/**
 * Scans nginx sites and try to rend report on each.
 */
function watchtower_drush_report_nginx($url = '', $folder = '/etc/nginx/sites-enabled') {
  if (!_watchtower_valid_url($url)) {
    drush_log('Please provide valid url of report server.', 'error');
    return FALSE;
  }
  // Collect all files.
  $files = glob(rtrim($folder, '/') . '/*');
  foreach ($files as $file) {
    // Parse file content (server_name & root)
    $content = file_get_contents($file);
    $matches = array();
    if (preg_match('/root\s*(\S*)/', $content, $matches)) {
      $root = trim($matches[1], ';');
      if (preg_match('/server_name\s*(\S*)/', $content, $matches)) {
        // @todo Parse port.
        $host = 'http://' . trim($matches[1], ';');

        // @todo Make compatible with 6+ drush version.
        if (drush_valid_drupal_root($root)) {
          echo "Trying send report: drush -l $host -r $root dwr --url=$url \n";

          // @todo Use the same executable.
          $command = 'drush -l ' . escapeshellarg($host) .
            ' -r ' . escapeshellarg($root) .
            ' dwr --url=' .  escapeshellarg($url);

          $descriptors = array(
            // stdin.
            0 => array('pipe', 'r'),
            // stdout.
            1 => array('pipe', 'w'),
            // stderr.
            2 => array('pipe', 'w'),
          );
          $pipes = array();
          if ($h = proc_open($command, $descriptors, $pipes, $root)) {
            $output = '';
            while (!feof($pipes[1])) {
              $output .= fgets($pipes[1]);
            }
            $error = '';
            while (!feof($pipes[2])) {
              $error .= fgets($pipes[2]);
            }

            fclose($pipes[0]);
            fclose($pipes[1]);
            fclose($pipes[2]);

            $return_code = proc_close($h);

            // @todo Drush outputs to err somehow.
            if ($output) {
              drush_print("Output:\n" . $output);
            }
            if ($error) {
              drush_print("Error:\n" . $error);
            }
          }
        }
        else {
          drush_log("Found unknown host = $host with root = $root", 'warning');
        }
      }
      else {
        drush_log('No server_name found', 'error');
      }
    }
    else {
      drush_log('No root found', 'error');
    }
  }
}

/**
 * Helper to validate URL when no core available.
 *
 * This is a copy-paste of core because this no core could exists.
 *
 * @param string $url
 *   The URL to validate
 *
 * @return bool
 *   TRUE if URL is valid.
 */
function _watchtower_valid_url($url) {
  return (bool)preg_match("
      /^                                                      # Start at the beginning of the text
      (?:ftp|https?|feed):\/\/                                # Look for ftp, http, https or feed schemes
      (?:                                                     # Userinfo (optional) which is typically
        (?:(?:[\w\.\-\+!$&'\(\)*\+,;=]|%[0-9a-f]{2})+:)*      # a username or a username and password
        (?:[\w\.\-\+%!$&'\(\)*\+,;=]|%[0-9a-f]{2})+@          # combination
      )?
      (?:
        (?:[a-z0-9\-\.]|%[0-9a-f]{2})+                        # A domain name or a IPv4 address
        |(?:\[(?:[0-9a-f]{0,4}:)*(?:[0-9a-f]{0,4})\])         # or a well formed IPv6 address
      )
      (?::[0-9]+)?                                            # Server port number (optional)
      (?:[\/|\?]
        (?:[\w#!:\.\?\+=&@$'~*,;\/\(\)\[\]\-]|%[0-9a-f]{2})   # The path and query (optional)
      *)?
    $/xi", $url);
}

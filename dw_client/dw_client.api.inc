<?php

/**
 * Collects website metrics to report.
 *
 * @return array
 */
function hook_dw_metric() {
  $metrics = array(
    'current_time' => time(),
    'php_memory_limit' => ini_get('memory_limit'),
  );
  return $metrics;
}

function dw_client_loaded_modules() {
  // Get current list of modules.
  $files = system_rebuild_module_data();

  // Remove hidden modules from display list.
  $visible_files = $files;
  foreach ($visible_files as $filename => $file) {
    if (!empty($file->info['hidden']) || !$file->status || $file->info['package'] == 'Core') {
      unset($visible_files[$filename]);
    }
  }

  // remove unused data
  array_walk($visible_files, 'dw_client_modules_array_format');

  return $visible_files;
}

function dw_client_modules_array_format(&$item, $key) {
  $item = (object) array(
    'name' => $item->info['name'],
    'machine_name' => $key,
    'description' => $item->info['description'],
    'package' => $item->info['package'],
    'version' => $item->info['version']
  );
}

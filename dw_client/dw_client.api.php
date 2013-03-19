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

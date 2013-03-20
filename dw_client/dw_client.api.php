<?php

/**
 * @file
 * Hooks provided by the Drupal Watchtower client module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Collects website metrics to report.
 *
 * @return array
 * An array of metric values, keyed by the metric name.
 */
function hook_dw_metric() {
  $metrics = array(
    'current_time' => time(),
    'php_memory_limit' => ini_get('memory_limit'),
  );
  return $metrics;
}

/**
 * @} End of "addtogroup hooks".
 */

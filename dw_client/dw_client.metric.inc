<?php

/**
 * @file
 * Defines metrics calculations and their algorithms.
 */

/**
 * Implements hook_dw_metric().
 */
function dw_client_dw_metric() {
  $metrics = array(
    'site:variables:cron_last' => variable_get('cron_last', 0),
  );
  return $metrics;
}

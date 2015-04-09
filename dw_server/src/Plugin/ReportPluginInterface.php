<?php
/**
 * @file
 * Contains \Drupal\dw_server\Plugin\ReportPluginInterface.
 */

namespace Drupal\dw_server\Plugin;

/**
 * Provides an interface for all report plugins.
 *
 * @todo Figure needed methods.
 */
interface ReportPluginInterface {

  /**
   * Renders report.
   *
   * @param array $context
   *   An array with the following keys:
   *   - report_id: a report entity ID
   *   - view_mode: a view mode the entity rendered
   *   - plugin_id: a report pligin ID
   *   - site_id: a site of the report
   *
   * @return array
   *   A renderable array.
   */
  public function renderReport(array $context = []);

}

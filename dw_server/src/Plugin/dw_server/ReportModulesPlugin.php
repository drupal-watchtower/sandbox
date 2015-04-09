<?php

/**
 * @file
 * Contains \Drupal\dw_server\Plugin\dw_server\ReportModulesPlugin.
 */

namespace Drupal\dw_server\Plugin\dw_server;

use Drupal\Component\Utility\SafeMarkup;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\dw_server\Plugin\ReportPluginBase;

/**
 * Displays some text as a tip.
 *
 * @ReportPlugin(
 *   id = "modules",
 *   title = @Translation("Module reports"),
 *   description = @Translation("Provides drupal module related metrics"),
 *   capabilities = {
 *     "server_render",
 *     "drupal_collect_data",
 *   },
 * )
 */
class ReportModulesPlugin extends ReportPluginBase implements ContainerFactoryPluginInterface {

  /**
   * {@inheritdoc}
   */
  public function renderReport(array $context = []) {
    /** @var \Drupal\dw_server\ReportInterface $report */
    $report = $this->entityManager
      ->getStorage('watchtower_report')
      ->load($context['report_id']);
    if ($report && $report->access('view', $this->currentUser)) {
      $data = $report->getReportData();
      // @todo Parse and render data.
      return [
        '#markup' => SafeMarkup::format('Report data: @data', ['@data' => $data]),
      ];
    }
    // Fallback to default render.
    return parent::renderReport($context);
  }

}

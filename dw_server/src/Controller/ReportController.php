<?php

/**
 * @file
 * Contains \Drupal\dw_server\Controller\ReportController.
 */

namespace Drupal\dw_server\Controller;

use Drupal\Component\Utility\Xss;
use Drupal\Core\Controller\ControllerBase;
use Drupal\dw_server\ReportInterface;
use Drupal\dw_server\SiteInterface;

/**
 * Provides route responses for dw_server.module.
 */
class ReportController extends ControllerBase {

  /**
   * Title callback for report pages.
   *
   * @param \Drupal\dw_server\ReportInterface $report
   *   A report entity.
   *
   * @return string
   *   The report label to be used as the page title.
   */
  public function reportTitle(ReportInterface $report) {
    return Xss::filter($report->label());
  }

  /**
   * Returns a rendered edit form to create a new report for the given site.
   *
   * @param \Drupal\dw_server\SiteInterface $watchtower_site
   *   The site this report will be added to.
   *
   * @return array
   *   The report add form.
   */
  public function addForm(SiteInterface $watchtower_site = NULL) {
    $report = $this->entityManager()
      ->getStorage('watchtower_report')
      ->create(['site' => $watchtower_site->id()]);
    return $this->entityFormBuilder()->getForm($report, 'add');
  }

}

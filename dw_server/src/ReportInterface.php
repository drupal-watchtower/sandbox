<?php

/**
 * @file
 * Contains \Drupal\dw_server\ReportInterface.
 */

namespace Drupal\dw_server;

use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\Core\Entity\EntityChangedInterface;
use Drupal\user\EntityOwnerInterface;

/**
 * Provides an interface defining a site report entity.
 */
interface ReportInterface extends ContentEntityInterface, EntityChangedInterface, EntityOwnerInterface {

  /**
   * Returns the report data.
   *
   * @return string
   *   The report data.
   */
  public function getReportData();

  /**
   * Sets the report data.
   *
   * @param string $description
   *   A text-compatible data for report.
   *
   * @return $this
   */
  public function setReportData($description);

  /**
   * Returns the site machine name.
   *
   * @return string
   *   The site hash string.
   */
  public function getSiteId();

  /**
   * Returns the site url.
   *
   * @return \Drupal\Core\Url
   *   The site url object.
   */
  public function getSiteUrl();

}

<?php

/**
 * @file
 * Contains \Drupal\dw_server\SiteInterface.
 */

namespace Drupal\dw_server;

use Drupal\Core\Config\Entity\ConfigEntityInterface;

/**
 * Provides an interface defining a site entity.
 */
interface SiteInterface extends ConfigEntityInterface {

  /**
   * Returns the site description.
   *
   * @return string
   *   The site description.
   */
  public function getDescription();

  /**
   * Returns the site url.
   *
   * @return \Drupal\Core\Url
   *   The site url object.
   */
  public function getSiteUrl();

}

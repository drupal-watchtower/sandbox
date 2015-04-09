<?php

/**
 * @file
 * Contains \Drupal\dw_server\ReportManagerInterface.
 */

namespace Drupal\dw_server;

use Drupal\Component\Plugin\Discovery\CachedDiscoveryInterface;
use Drupal\Component\Plugin\PluginManagerInterface;

/**
 * Provides an interface for the discovery and instantiation of report plugins.
 */
interface ReportManagerInterface extends PluginManagerInterface, CachedDiscoveryInterface {

  /**
   * Returns a list of plugins usable for options display.
   *
   * @return array
   *   Array keyed by plugin ID with title value.
   */
  public function getPluginsAsOptions();

}

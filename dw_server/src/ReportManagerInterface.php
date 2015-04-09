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

}

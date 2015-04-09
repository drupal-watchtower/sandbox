<?php

/**
 * @file
 * Contains \Drupal\dw_server\ReportManager.
 */

namespace Drupal\dw_server;

use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Plugin\DefaultPluginManager;
use Drupal\Core\Cache\CacheBackendInterface;

/**
 * Provides the default manager for report plugins.
 */
class ReportManager extends DefaultPluginManager implements ReportManagerInterface {

  /**
   * Constructs ReportManager.
   *
   * {@inheritdoc}
   */
  public function __construct(\Traversable $namespaces, CacheBackendInterface $cache_backend, ModuleHandlerInterface $module_handler) {
    parent::__construct('Plugin/dw_server', $namespaces, $module_handler, 'Drupal\dw_server\Plugin\ReportPluginInterface', 'Drupal\dw_server\Annotation\ReportPlugin');
    $this->setCacheBackend($cache_backend, 'dw_server_plugins');
    $this->alterInfo('dw_server_plugins');
  }
}

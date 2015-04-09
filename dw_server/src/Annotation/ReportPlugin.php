<?php

/**
 * @file
 * Contains \Drupal\dw_server\Annotation\ReportPlugin.
 */

namespace Drupal\dw_server\Annotation;

use Drupal\Component\Annotation\Plugin;

/**
 * Defines a report annotation object.
 *
 * Plugin Namespace: Plugin\dw_server
 *
 * For a working example, see \Drupal\dw_server\Plugin\ReportModulesPlugin
 *
 * @see \Drupal\dw_server\Plugin\ReportPluginBase
 * @see \Drupal\dw_server\Plugin\ReportPluginInterface
 * @see \Drupal\dw_server\ReportManager
 * @see plugin_api
 *
 * @Annotation
 */
class ReportPlugin extends Plugin {

  /**
   * The plugin ID.
   *
   * @var string
   */
  public $id;

  /**
   * The title of the plugin.
   *
   * @var \Drupal\Core\Annotation\Translation
   *
   * @ingroup plugin_translatable
   */
  public $title;

  /**
   * A short description of the report type.
   *
   * @ingroup plugin_translatable
   *
   * @var \Drupal\Core\Annotation\Translation
   */
  public $description;

  /**
   * An array of capabilities the plugin supports.
   *
   * @var string[]
   */
  public $capabilities = [];

}

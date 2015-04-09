<?php

/**
 * @file
 * Contains \Drupal\dw_server\PostRenderCache.
 */

namespace Drupal\dw_server;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Database\Connection;
use Drupal\Core\Database\Query\SelectInterface;
use Drupal\Core\Database\Query\Condition;
use Drupal\Core\DependencyInjection\ContainerInjectionInterface;
use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Language\LanguageManagerInterface;
use Drupal\Core\Render\Renderer;
use Drupal\Core\Render\RendererInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\user\Entity\User;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Defines a service to render reports.
 */
class PostRenderCache {

  /**
   * The entity manager service.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface
   */
  protected $entityManager;

  /**
   * The entity form builder service.
   *
   * @var \Drupal\Core\Entity\EntityFormBuilderInterface
   */
  protected $entityFormBuilder;

  /**
   * Report manager service.
   *
   * @var \Drupal\dw_server\ReportManagerInterface
   */
  protected $reportManager;

  /**
   * Current logged in user.
   *
   * @var \Drupal\Core\Session\AccountInterface
   */
  protected $currentUser;

  /**
   * The module handler service.
   *
   * @var \Drupal\Core\Extension\ModuleHandlerInterface
   */
  protected $moduleHandler;

  /**
   * The renderer service.
   *
   * @var \Drupal\Core\Render\RendererInterface
   */
  protected $renderer;

  /**
   * Constructs a new PostRenderCache object.
   *
   * @param \Drupal\Core\Entity\EntityManagerInterface $entity_manager
   *   The entity manager service.
   * @param \Drupal\Core\Session\AccountInterface $current_user
   *   The current logged in user.
   * @param \Drupal\dw_server\ReportManagerInterface $report_manager
   *   The report manager.
   * @param \Drupal\Core\Extension\ModuleHandlerInterface $module_handler
   *   The module handler service.
   * @param \Drupal\Core\Render\RendererInterface $renderer
   *   The renderer service.
   */
  public function __construct(EntityManagerInterface $entity_manager, AccountInterface $current_user, ReportManagerInterface $report_manager, ModuleHandlerInterface $module_handler, RendererInterface $renderer) {
    $this->entityManager = $entity_manager;
    $this->currentUser = $current_user;
    $this->reportManager = $report_manager;
    $this->moduleHandler = $module_handler;
    $this->renderer = $renderer;
  }

  /**
   * #post_render_cache callback; replaces placeholder with rendered report.
   *
   * @param array $element
   *   The renderable array that contains the to be replaced placeholder.
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
  public function renderReport(array $element, array $context) {
    $callback = 'dw_server.post_render_cache:renderReport';
    $placeholder = $this->renderer->generateCachePlaceholder($callback, $context);
    $markup = '';
    /** @var \Drupal\dw_server\Plugin\ReportPluginInterface $plugin */
    //$plugin = $this->reportManager->getInstance($context);
    $plugin = $this->reportManager->createInstance($context['plugin_id'], $context);

    if ($plugin) {
      // @todo Check access.
      $markup = $plugin->renderReport($context);
      $markup = $this->renderer->render($markup);
    }
    $element['#markup'] = str_replace($placeholder, $markup, $element['#markup']);
    return $element;
  }

}

<?php

/**
 * @file
 * Contains \Drupal\dw_server\ReportViewBuilder.
 */

namespace Drupal\dw_server;

use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Entity\EntityViewBuilder;
use Drupal\Core\Language\LanguageManagerInterface;
use Drupal\Core\Render\RendererInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a Report view builder.
 */
class ReportViewBuilder extends EntityViewBuilder {

  /**
   * The renderer service.
   *
   * @var \Drupal\Core\Render\RendererInterface
   */
  protected $renderer;

  /**
   * Constructs a new ReportViewBuilder.
   *
   * @param \Drupal\Core\Entity\EntityTypeInterface $entity_type
   *   The entity type definition.
   * @param \Drupal\Core\Entity\EntityManagerInterface $entity_manager
   *   The entity manager service.
   * @param \Drupal\Core\Language\LanguageManagerInterface $language_manager
   *   The language manager.
   * @param \Drupal\Core\Render\RendererInterface $renderer
   *   The renderer service.
   */
  public function __construct(EntityTypeInterface $entity_type, EntityManagerInterface $entity_manager, LanguageManagerInterface $language_manager, RendererInterface $renderer) {
    parent::__construct($entity_type, $entity_manager, $language_manager);
    $this->renderer = $renderer;
  }

  /**
   * {@inheritdoc}
   */
  public static function createInstance(ContainerInterface $container, EntityTypeInterface $entity_type) {
    return new static(
      $entity_type,
      $container->get('entity.manager'),
      $container->get('language_manager'),
      $container->get('renderer')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function buildComponents(array &$build, array $entities, array $displays, $view_mode, $langcode = NULL) {
    if (empty($entities)) {
      return;
    }
    parent::buildComponents($build, $entities, $displays, $view_mode, $langcode);

    /** @var \Drupal\dw_server\ReportInterface[] $entities */
    foreach ($entities as $id => $entity) {
      $display = $displays[$entity->bundle()];

      if ($display->getComponent('description')) {
        // Override render of description field.
        $callback = 'dw_server.post_render_cache:renderReport';
        $context = [
          'report_id' => $entity->id(),
          'view_mode' => $view_mode,
          'site_id'   => $entity->getSiteId(),
          // @todo get rid of.
          'plugin_id' => 'modules',
        ];
        $placeholder = $this->renderer->generateCachePlaceholder($callback, $context);
        $build[$id]['plugin'] = [
          '#post_render_cache' => [
            $callback => [
              $context,
            ],
          ],
          '#markup' => $placeholder,
        ];
        // @todo Remove this hack when entities will have default template
        //   https://drupal.org/node/2186653
        //$build[$id]['#theme_wrappers'] = array('container');
        $build[$id]['#attributes']['data-entity-id'] = $entity->id();
        $build[$id]['#attributes']['data-entity-type'] = $entity->getEntityTypeId();
        $build[$id]['#attributes']['data-entity-bundle'] = $entity->bundle();
      }
    }
  }

}

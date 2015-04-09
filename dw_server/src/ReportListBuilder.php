<?php

/**
 * @file
 * Contains \Drupal\dw_server\ReportListBuilder.
 */

namespace Drupal\dw_server;

use Drupal\Component\Utility\SafeMarkup;
use Drupal\Core\Datetime\DateFormatter;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityListBuilder;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Language\LanguageInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Defines a class to build a listing of report entities.
 *
 * @see \Drupal\dw_server\Entity\Report
 */
class ReportListBuilder extends EntityListBuilder {

  /**
   * The date formatter service.
   *
   * @var \Drupal\Core\Datetime\DateFormatter
   */
  protected $dateFormatter;

  /**
   * Constructs a new NodeListBuilder object.
   *
   * @param \Drupal\Core\Entity\EntityTypeInterface $entity_type
   *   The entity type definition.
   * @param \Drupal\Core\Entity\EntityStorageInterface $storage
   *   The entity storage class.
   * @param \Drupal\Core\Datetime\DateFormatter $date_formatter
   *   The date formatter service.
   */
  public function __construct(EntityTypeInterface $entity_type, EntityStorageInterface $storage, DateFormatter $date_formatter) {
    parent::__construct($entity_type, $storage);

    $this->dateFormatter = $date_formatter;
  }

  /**
   * {@inheritdoc}
   */
  public static function createInstance(ContainerInterface $container, EntityTypeInterface $entity_type) {
    return new static(
      $entity_type,
      $container->get('entity.manager')->getStorage($entity_type->id()),
      $container->get('date.formatter')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function buildHeader() {
    $header = array(
      'created' => $this->t('Created'),
      'plugin_id' => $this->t('Plugin'),
      'label' => $this->t('Label'),
      'site' => array(
        'data' => $this->t('Site'),
        'class' => array(RESPONSIVE_PRIORITY_MEDIUM),
      ),
      'changed' => array(
        'data' => $this->t('Updated'),
        'class' => array(RESPONSIVE_PRIORITY_LOW),
      ),
    );
    return $header + parent::buildHeader();
  }

  /**
   * {@inheritdoc}
   */
  public function buildRow(EntityInterface $entity) {
    /** @var \Drupal\dw_server\ReportInterface $entity */
    $row['created'] = $this->dateFormatter->format($entity->get('created')->value, 'short');
    $row['plugin_id'] = $entity->getPluginId();
    $row['label']['data'] = array(
      '#type' => 'link',
      '#title' => $entity->label(),
      '#url' => $entity->urlInfo(),
    );
    $site = $entity->getSiteEntity();
    $row['site']['data'] = array(
      '#type' => 'link',
      '#title' => $site->label(),
      '#url' => $site->urlInfo(),
    );
    $row['changed'] = $this->dateFormatter->format($entity->getChangedTime(), 'short');
    $row['operations']['data'] = $this->buildOperations($entity);
    return $row + parent::buildRow($entity);
  }

  /**
   * {@inheritdoc}
   */
  protected function getDefaultOperations(EntityInterface $entity) {
    $operations = parent::getDefaultOperations($entity);

    $destination = drupal_get_destination();
    foreach ($operations as $key => $operation) {
      $operations[$key]['query'] = $destination;
    }
    return $operations;
  }

}

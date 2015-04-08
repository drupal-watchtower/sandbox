<?php

/**
 * @file
 * Contains \Drupal\dw_server\SiteListBuilder.
 */

namespace Drupal\dw_server;

use Drupal\Component\Utility\SafeMarkup;
use Drupal\Core\Config\Entity\ConfigEntityListBuilder;
use Drupal\Core\Entity\EntityInterface;

/**
 * Defines a class to build a listing of site entities.
 *
 * @see \Drupal\dw_server\Entity\Site
 */
class SiteListBuilder extends ConfigEntityListBuilder {

  /**
   * {@inheritdoc}
   */
  public function buildHeader() {
    $header['label'] = t('Site label');
    $header['url'] = t('Site url');
    return $header + parent::buildHeader();
  }

  /**
   * {@inheritdoc}
   */
  public function buildRow(EntityInterface $entity) {
    $row['label'] = $this->getLabel($entity);
    $row['url'] = SafeMarkup::checkPlain($entity->get('url'));
    return $row + parent::buildRow($entity);
  }

}

<?php

/**
 * @file
 * Contains \Drupal\taxonomy\Entity\Report.
 */

namespace Drupal\dw_server\Entity;

use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Entity\FieldableEntityInterface;
use Drupal\Core\Field\BaseFieldDefinition;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Url;
use Drupal\dw_server\ReportInterface;
use Drupal\user\Entity\User;
use Drupal\user\EntityOwnerInterface;
use Drupal\user\UserInterface;

/**
 * Defines the site report entity.
 *
 * @ContentEntityType(
 *   id = "watchtower_report",
 *   label = @Translation("Watchtower report"),
 *   bundle_label = @Translation("Watchtower site"),
 *   handlers = {
 *     "view_builder" = "Drupal\dw_server\ReportViewBuilder",
 *     "list_builder" = "Drupal\dw_server\ReportListBuilder",
 *     "form" = {
 *       "add" = "Drupal\dw_server\ReportForm",
 *       "edit" = "Drupal\dw_server\ReportForm",
 *       "delete" = "\Drupal\Core\Entity\ContentEntityDeleteForm",
 *     }
 *   },
 *   admin_permission = "administer watchtower reports",
 *   base_table = "watchtower_report_data",
 *   translatable = FALSE,
 *   entity_keys = {
 *     "id" = "id",
 *     "bundle" = "site",
 *     "label" = "label",
 *     "revision" = "revision_id",
 *     "uuid" = "uuid"
 *   },
 *   common_reference_target = FALSE,
 *   permission_granularity = "bundle",
 *   links = {
 *     "collection" = "/watchtower/reports",
 *     "add-form" = "/watchtower/add",
 *     "canonical" = "/watchtower/report/{watchtower_site}",
 *     "edit-form" = "/watchtower/manage/{watchtower_site}/edit",
 *     "delete-form" = "/watchtower/manage/{watchtower_site}/delete",
 *   },
 *   bundle_entity_type = "watchtower_site",
 *   field_ui_base_route  = "entity.watchtower_site.edit_form",
 * )
 */
class Report extends ContentEntityBase implements ReportInterface {

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {
    $fields['id'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('Report ID'))
      ->setDescription(t('The Report ID.'))
      ->setReadOnly(TRUE)
      ->setSetting('unsigned', TRUE);

    $fields['uuid'] = BaseFieldDefinition::create('uuid')
      ->setLabel(t('UUID'))
      ->setDescription(t('The report UUID.'))
      ->setReadOnly(TRUE);

    $fields['site'] = BaseFieldDefinition::create('entity_reference')
      // @todo Add setting for formatter and form autocomplete.
      ->setLabel(t('Site'))
      ->setDescription(t('The site to which the report is assigned.'))
      ->setSetting('target_type', 'watchtower_site');

    $fields['hash_key'] = BaseFieldDefinition::create('string')
      ->setLabel(t('Hash key'))
      ->setDescription(t('Used to identify report.'))
      ->setSetting('max_length', 255);

    $fields['revision_id'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('Revision ID'))
      ->setDescription(t('The Report revision ID.'))
      ->setReadOnly(TRUE)
      ->setSetting('unsigned', TRUE);

    $fields['label'] = BaseFieldDefinition::create('string')
      ->setLabel(t('Label'))
      ->setDescription(t('The report label.'))
      ->setSetting('max_length', 255)
      ->setDisplayOptions('view', array(
        'label' => 'hidden',
        'type' => 'string',
        'weight' => -5,
      ))
      // @todo Decide visibility.
      //->setDisplayConfigurable('view', TRUE)
      ->setDisplayOptions('form', array(
        'type' => 'string_textfield',
        'weight' => -5,
      ))
      ->setDisplayConfigurable('form', TRUE);

    $fields['description'] = BaseFieldDefinition::create('string_long')
      ->setLabel(t('Report data'))
      ->setDescription(t('A content of the report.'))
      ->setRevisionable(TRUE)
      ->setDisplayOptions('view', array(
        'label' => 'hidden',
        'type' => 'basic_string',
        'weight' => 10,
      ))
      ->setDisplayConfigurable('view', TRUE)
      ->setDisplayOptions('form', array(
        'type' => 'string_textarea',
        'weight' => 10,
      ))
      ->setDisplayConfigurable('form', TRUE);

    $fields['created'] = BaseFieldDefinition::create('created')
      ->setLabel(t('Created on'))
      ->setDescription(t('The time that the report was created.'))
      ->setDisplayOptions('view', array(
        'label' => 'hidden',
        'type' => 'timestamp',
        'weight' => -50,
      ))
      ->setDisplayConfigurable('view', TRUE)
      ->setDisplayOptions('form', array(
        'type' => 'datetime_timestamp',
        'weight' => 50,
      ))
      ->setDisplayConfigurable('form', TRUE);

    $fields['changed'] = BaseFieldDefinition::create('changed')
      ->setLabel(t('Changed'))
      ->setDescription(t('The time that the report was last edited.'))
      ->setRevisionable(TRUE)
      ->setDisplayOptions('view', array(
        'label' => 'hidden',
        'type' => 'timestamp',
        'weight' => -40,
      ))
      ->setDisplayConfigurable('view', TRUE);

    $fields['author_id'] = BaseFieldDefinition::create('entity_reference')
      // @todo Add setting for formatter .
      ->setLabel(t('Author'))
      ->setSetting('target_type', 'user')
      ->setDefaultValue(0)
      ->setDisplayOptions('form', array(
        'type' => 'entity_reference_autocomplete',
        'weight' => -10,
        'settings' => array(
          'match_operator' => 'CONTAINS',
          'size' => '30',
          'placeholder' => '',
        ),
      ))
      ->setDisplayConfigurable('form', TRUE);

    return $fields;
  }

  /**
   * {@inheritdoc}
   */
  public function getReportData() {
    return $this->get('description')->value;
  }

  /**
   * {@inheritdoc}
   */
  public function setReportData($description) {
    $this->set('description', $description);
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function getSiteId() {
    return $this->get('site')->target_id;
  }

  /**
   * {@inheritdoc}
   */
  public function getSiteEntity() {
    return $this->get('site')->entity;
  }

  /**
   * {@inheritdoc}
   */
  public function getSiteUrl() {
    return Url::fromUri($this->getSiteEntity()->get('url'));
  }

  /**
   * {@inheritdoc}
   */
  public function getChangedTime() {
    return $this->get('changed')->value;
  }

  /**
   * {@inheritdoc}
   */
  public function getOwner() {
    $user = $this->get('author_uid')->entity;
    if (!$user || $user->isAnonymous()) {
      $user = User::getAnonymousUser();
    }
    return $user;
  }

  /**
   * {@inheritdoc}
   */
  public function getOwnerId() {
    return $this->get('author_uid')->target_id;
  }

  /**
   * {@inheritdoc}
   */
  public function setOwnerId($uid) {
    $this->set('author_uid', $uid);
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function setOwner(UserInterface $account) {
    $this->set('author_uid', $account->id());
    return $this;
  }

}

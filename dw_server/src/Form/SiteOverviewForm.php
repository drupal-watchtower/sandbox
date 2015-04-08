<?php

/**
 * @file
 * Contains \Drupal\dw_server\Form\SiteOverviewForm.
 */

namespace Drupal\dw_server\Form;

use Drupal\Component\Utility\SafeMarkup;
use Drupal\Core\Entity\EntityForm;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\dw_server\SiteInterface;
use Drupal\taxonomy\VocabularyInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/*
 * Provides site overview form to manage it.
 */
class SiteOverviewForm extends EntityForm {

  /**
   * The entity manager.
   *
   * @var \Drupal\Core\Entity\EntityManagerInterface
   */
  protected $entityManager;

  /**
   * Constructs the NodeTypeForm object.
   *
   * @param \Drupal\Core\Entity\EntityManagerInterface $entity_manager
   *   The entity manager
   */
  public function __construct(EntityManagerInterface $entity_manager) {
    $this->entityManager = $entity_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('entity.manager')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function form(array $form, FormStateInterface $form_state) {
    $form = parent::form($form, $form_state);

    /** @var \Drupal\dw_server\SiteInterface $site */
    $site = $this->entity;
    $form['#title'] = SafeMarkup::checkPlain($site->label());

    $form['url'] = array(
      '#title' => $this->t('Site url: @url', ['@url' => $site->get('url')]),
      '#type' => 'link',
      '#url' => $site->getSiteUrl(),
    );

    // Configure enabled plugins.
    $enabled_plugins = $site->getThirdPartySetting('dw_client', 'plugins', []);
    // @todo Get from manager.
    $plugins = [
      'modules' => 'Modules information plugin',
      'entity' => 'Site new/edited entity statistics',
      'system' => 'Site performance statistics',
    ];
    $form['plugins'] = [
      '#type' => 'checkboxes',
      '#options' => $plugins,
      '#title' => $this->t('Enabled reports'),
      '#default_value' => $enabled_plugins,
    ];
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function save(array $form, FormStateInterface $form_state) {
    /** @var \Drupal\dw_server\SiteInterface $site */
    $site = $this->entity;
    $site
      ->setThirdPartySetting('dw_client', 'plugins', $form_state->getValue('plugins'))
      ->save();
  }

}

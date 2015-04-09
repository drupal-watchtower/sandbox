<?php

/**
 * @file
 * Contains \Drupal\dw_server\Form\SiteOverviewForm.
 */

namespace Drupal\dw_server\Form;

use Drupal\Component\Utility\SafeMarkup;
use Drupal\Core\Entity\EntityForm;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\dw_server\ReportManagerInterface;
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
   * The report manager.
   *
   * @var \Drupal\dw_server\ReportManagerInterface
   */
  protected $reportManager;

  /**
   * Constructs the NodeTypeForm object.
   *
   * @param \Drupal\Core\Entity\EntityManagerInterface $entity_manager
   *   The entity manager
   * @param \Drupal\dw_server\ReportManagerInterface $report_manager
   *   The report manager.
   */
  public function __construct(EntityManagerInterface $entity_manager, ReportManagerInterface $report_manager) {
    $this->entityManager = $entity_manager;
    $this->reportManager = $report_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('entity.manager'),
      $container->get('plugin.manager.dw_server.report')
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
    $form['plugins'] = [
      '#type' => 'checkboxes',
      '#options' => $this->reportManager->getPluginsAsOptions(),
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

  /**
   * {@inheritdoc}
   */
  protected function actions(array $form, FormStateInterface $form_state) {
    // Do not show delete on overview.
    $actions = parent::actions($form, $form_state);
    $actions['delete']['#access'] = FALSE;
    return $actions;
  }

}

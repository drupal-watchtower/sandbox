<?php

/**
 * @file
 * Contains \Drupal\dw_server\ReportForm.
 */

namespace Drupal\dw_server;

use Drupal\Core\Entity\ContentEntityForm;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Form\FormStateInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Form controller for the report add/edit forms.
 */
class ReportForm extends ContentEntityForm {

  /**
   * The report manager.
   *
   * @var \Drupal\dw_server\ReportManagerInterface
   */
  protected $reportManager;

  /**
   * Constructs a BlockContentForm object.
   *
   * @param \Drupal\Core\Entity\EntityManagerInterface $entity_manager
   *   The entity manager.
   * @param \Drupal\dw_server\ReportManagerInterface $report_manager
   *   The report manager.
   */
  public function __construct(EntityManagerInterface $entity_manager, ReportManagerInterface $report_manager) {
    parent::__construct($entity_manager);
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
    /** @var \Drupal\dw_server\ReportInterface $report */
    $report = $this->entity;

    if ($this->operation == 'edit') {
      $form['#title'] = $this->t('Edit report %label', array('%label' => $report->label()));
    }
    $options = $this->reportManager->getPluginsAsOptions();
    // @todo Limit plugins per site.

    $form['plugin'] = [
      '#title' => $this->t('Select plugin'),
      '#type' => 'select',
      '#options' => $options,
    ];

    return parent::form($form, $form_state, $report);
  }

  /**
   * {@inheritdoc}
   *
   * @todo Implement a validation constraint for REST to check allowed plugins.
   */
  public function validate(array $form, FormStateInterface $form_state) {
    /** @var \Drupal\dw_server\ReportInterface $entity */
    $entity = parent::validate($form, $form_state);
    $site = $entity->getSiteEntity();
    if (!$site->status()) {
      // Do not accept reports for disabled sites.
      $form_state->setErrorByName('plugin', $this->t('Site is disabled'));
    }
    else {
      // Make sure plugin enabled for the site.
      $enabled_plugins = $site->getThirdPartySetting('dw_client', 'plugins', []);
      if (empty($enabled_plugins[$form_state->getValue('plugin')])) {
        // @todo Clean-up into access for field.
        $form_state->setErrorByName('plugin', $this->t('Plugin is not enabled for site'));
      }
    }
    return $entity;
  }

  /**
   * {@inheritdoc}
   */
  public function save(array $form, FormStateInterface $form_state) {
    $this->entity->set('plugin_id', $form_state->getValue('plugin'));
    $status = parent::save($form, $form_state);

    $t_args = array('%label' => $this->entity->label());

    if ($status == SAVED_UPDATED) {
      drupal_set_message($this->t('The report %label has been updated.', $t_args));
    }
    elseif ($status == SAVED_NEW) {
      drupal_set_message(t('The report %label has been added.', $t_args));
      $context = array_merge($t_args, array('link' => $this->entity->link($this->t('View'))));
      $this->logger('dw_server')->notice('Added report %label.', $context);
    }

    $form_state->setRedirectUrl($this->entity->urlInfo());
  }

}

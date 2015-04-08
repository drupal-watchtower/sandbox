<?php

/**
 * @file
 * Contains \Drupal\dw_server\SiteForm.
 */

namespace Drupal\dw_server;

use Drupal\Component\Utility\SafeMarkup;
use Drupal\Core\Entity\EntityForm;
use Drupal\Core\Entity\EntityManagerInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Form controller for site forms.
 */
class SiteForm extends EntityForm {

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

    $site = $this->entity;
    if ($this->operation == 'add') {
      $form['#title'] = SafeMarkup::checkPlain($this->t('Add site'));
    }
    else {
      $form['#title'] = $this->t('Edit %label content type', array('%label' => $site->label()));
    }

    $form['label'] = array(
      '#title' => t('Label'),
      '#type' => 'textfield',
      '#default_value' => $site->label(),
      '#description' => t('The human-readable name of this site.'),
      '#required' => TRUE,
      '#size' => 30,
    );

    $form['id'] = array(
      '#type' => 'machine_name',
      '#default_value' => $site->id(),
      '#maxlength' => EntityTypeInterface::BUNDLE_MAX_LENGTH,
      '#machine_name' => array(
        'exists' => [get_class($this), 'exists'],
        'source' => array('label'),
      ),
    );

    $form['url'] = array(
      '#title' => t('Url'),
      '#type' => 'textfield',
      '#default_value' => $site->get('url'),
      '#required' => TRUE,
      '#size' => 30,
    );

    $form['description'] = array(
      '#title' => t('Description'),
      '#type' => 'textarea',
      '#default_value' => $site->get('description'),
      '#description' => t('Describe this site.'),
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validate(array $form, FormStateInterface $form_state) {
    parent::validate($form, $form_state);

    try {
      // Validate the URL.
      $url = Url::fromUri($form_state->getValue('url'));
      if (!$url->isExternal()) {
        $form_state->setErrorByName('url', $this->t('Url should be external'));
      }
    }
    catch (\InvalidArgumentException $e) {
      $form_state->setErrorByName('url', $e->getMessage());
    }
  }

  /**
   * {@inheritdoc}
   */
  public function save(array $form, FormStateInterface $form_state) {
    /** @var \Drupal\Core\Config\Entity\ConfigEntityInterface $type */
    $type = $this->entity;
    $type
      ->set('id', trim($type->id()))
      ->set('label', trim($type->label()));

    $status = $type->save();

    $t_args = array('%label' => $type->label());

    if ($status == SAVED_UPDATED) {
      drupal_set_message(t('The site %label has been updated.', $t_args));
    }
    elseif ($status == SAVED_NEW) {
      drupal_set_message(t('The site %label has been added.', $t_args));
      // $context = array_merge($t_args, array('link' => $type->link($this->t('View'), 'collection')));
      // $this->logger('dw_server')->notice('Added site %label.', $context);
    }

    // $this->entityManager->clearCachedFieldDefinitions();
    $form_state->setRedirectUrl($type->urlInfo('collection'));
  }

  /**
   * Checks that site with name already exists.
   *
   * @todo Remove after https://www.drupal.org/node/2091871 fixed.
   */
  public static function exists($entity_id, array $element, FormStateInterface $form_state) {
    /** @var \Drupal\Core\Entity\EntityTypeInterface $entity_type */
    $entity_type = $form_state->getFormObject()->getEntity()->getEntityType();
    return (bool) \Drupal::entityQuery($entity_type->id())
      ->condition($entity_type->getKey('id'), $entity_id)
      ->execute();
  }

}

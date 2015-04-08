<?php

/**
 * @file
 * Contains \Drupal\dw_server\Form\SiteEnableForm.
 */

namespace Drupal\dw_server\Form;

use Drupal\Core\Entity\EntityConfirmFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Provides the site enable form.
 */
class SiteEnableForm extends EntityConfirmFormBase {

  /**
   * {@inheritdoc}
   */
  public function getQuestion() {
    return $this->t('Are you sure you want to enable the site %site?', array('%site' => $this->entity->label()));
  }

  /**
   * {@inheritdoc}
   */
  public function getCancelUrl() {
    return $this->entity->urlInfo('collection');
  }

  /**
   * {@inheritdoc}
   */
  public function getConfirmText() {
    return $this->t('Enable');
  }

  /**
   * {@inheritdoc}
   */
  public function getDescription() {
    return $this->t('Enabled site will start accept reports.');
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->entity->enable()->save();
    drupal_set_message($this->t('Enabled site %site.', array('%site' => $this->entity->label())));

    $form_state->setRedirectUrl($this->getCancelUrl());
  }

}

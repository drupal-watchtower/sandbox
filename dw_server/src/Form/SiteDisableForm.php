<?php

/**
 * @file
 * Contains \Drupal\dw_server\Form\SiteDisableForm.
 */

namespace Drupal\dw_server\Form;

use Drupal\Core\Entity\EntityConfirmFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Provides the site disable form.
 */
class SiteDisableForm extends EntityConfirmFormBase {

  /**
   * {@inheritdoc}
   */
  public function getQuestion() {
    return $this->t('Are you sure you want to disable the site %site?', array('%site' => $this->entity->label()));
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
    return $this->t('Disable');
  }

  /**
   * {@inheritdoc}
   */
  public function getDescription() {
    return $this->t('Disabled site will not accept reports.');
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->entity->disable()->save();
    drupal_set_message($this->t('Disabled site %site.', array('%site' => $this->entity->label())));

    $form_state->setRedirectUrl($this->getCancelUrl());
  }

}

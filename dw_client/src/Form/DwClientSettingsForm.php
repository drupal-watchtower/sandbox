<?php

/**
 * @file
 * Contains \Drupal\dw_client\Form\DwClientSettingsForm.
 */

namespace Drupal\dw_client\Form;

use Drupal\Component\Utility\UrlHelper;
use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;

class DwClientSettingsForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'dw_client_settings_form';
  }

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return ['dw_client.settings'];
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('dw_client.settings');

    if (($url = \Drupal::state()->get('dw_client.report_url')) && UrlHelper::isExternal($url)) {
      // Show where to find results.
      // @todo Use as enabled state.
      $url = Url::fromUri($url);
      $form['report_url'] = [
        '#type' => 'link',
        '#prefix' => $this->t('Report URL '),
        '#title' => $url->toString(),
        '#url' =>  $url,
      ];
    }

    $example_url = 'http://example.com/xmlrpc.php';
    $form['url'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Watchtower server address'),
      '#default_value' => $config->get('url'),
      '#placeholder' => $example_url,
      '#description' => $this->t('Enter address of XML-RPC interface, for example: %server', ['%server' => $example_url]),
    );

    $form['schedule'] = array(
     '#type' => 'checkbox',
     '#title' => $this->t('Enable Watchtower reporting'),
     '#description' => $this->t('Checking this enabling regular reporting to watchtower server about current drupal installation state.'),
     '#default_value' => $config->get('schedule'),
    );

    $intervals = [3600, 10800, 21600, 43200, 86400, 604800];
    $form['report_threshold'] = array(
      '#type' => 'select',
      '#title' => $this->t('Send report every'),
      '#options' => array_combine($intervals, $intervals),
      '#default_value' => $config->get('report_threshold'),
      '#description' => $this->t('Watchtower client will interact with watchtower server when cron executed so frequent as you set.'),
    );

    $events = array_keys(\Drupal::moduleHandler()->invokeAll('dw_metric', []));
    $form['events'] = array(
      '#type' => 'checkboxes',
      '#options' => array_combine($events, $events),
      '#title' => $this->t('Send report on module enable or disable events'),
      '#default_value' => $config->get('events'),
      '#description' => $this->t('Watchtower client will interact with watchtower server every time when modules become enabled or disabled.'),
    );

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('dw_client.settings');

    if ($config->get('schedule') && $config->get('url') != $form_state->getValue('url')) {
      // Validate new server.
      if (!_dw_client_report(TRUE, $form_state->getValue('url'))) {
        $form_state->setErrorByName('url', $this->t('Error connection to server'));
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->config('dw_client.settings')
      ->set('url', $form_state->getValue('url'))
      ->set('schedule', $form_state->getValue('schedule'))
      ->set('report_threshold', $form_state->getValue('report_threshold'))
      ->set('events', $form_state->getValue('events'))
      ->save();
    parent::submitForm($form, $form_state);
  }

}

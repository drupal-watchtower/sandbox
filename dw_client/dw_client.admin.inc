<?php

/**
 * @file
 * Module settings UI.
 */

/**
 * Return form for Drupal Watchtower Client settings.
 */
function dw_client_settings_form($form, $form_state) {
  $form['client'] = array(
    '#type' => 'fieldset',
    '#title' => t('Client configuration'),
  );
  $form['client']['watchtower_reporting'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Watchtower reporting'),
    '#description' => t('Checking this enabling regular reporting to watchtower server about current drupal installation state.'),
    '#default_value' => variable_get('watchtower_reporting', FALSE),
  );
  $form['client']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="watchtower_reporting"]' => array('checked' => TRUE),
      ),
    ),
  );
  if ($instance_url = variable_get('watchtower_instance_info_url', FALSE)) {
    $link = l($instance_url, $instance_url, array('external' => TRUE));
    $form['client']['settings']['watchtower_instance_info_url'] = array(
      '#type' => 'markup',
      '#prefix' => '<div>',
      '#markup' => t('Your site information could be found on !url.', array('!url' => $link)),
      '#suffix' => '</div>',
    );
  }
  $form['client']['settings']['watchtower_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Watchtower server address'),
    '#default_value' => variable_get('watchtower_server', ''),
    '#description' => t('Enter address of XML-RPC interface, for example: <i>%server</i>', array('%server' => 'http://example.com/xmlrpc.php')),
  );

  $intervals = array(3600, 10800, 21600, 43200, 86400, 604800);
  $form['client']['settings']['watchtower_report_threshold'] = array(
    '#type' => 'select',
    '#title' => t('Send report every'),
    '#options' => drupal_map_assoc($intervals, 'format_interval'),
    '#default_value' => variable_get('watchtower_report_threshold', WATCHTOWER_DEFAULT_REPORT_THRESHOLD),
    '#description' => t('Watchtower client will interact with watchtower server when cron executed so frequent as you set.'),
  );
  $form['client']['settings']['watchtower_on_modules_events'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send report on module enable or disable events'),
    '#default_value' => variable_get('watchtower_on_modules_events', FALSE),
    '#description' => t('Watchtower client will interact with watchtower server every time when modules become enabled or disabled.'),
  );
  $form =  system_settings_form($form);
  $form['#submit'][] = 'dw_client_settings_form_submit';
  return $form;
}

/**
 * Submit callback.
 */
function dw_client_settings_form_submit($form, $form_state) {
  // @todo Add validation.
  _dw_client_report(TRUE);
}

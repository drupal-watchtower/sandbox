<?php

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
    )
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

  $form['client']['settings']['watchtower_on_modules_events'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send report on module enable or disable events'),
    '#default_value' => variable_get('watchtower_on_modules_events', FALSE),
    '#description' => t('Watchtower client will interact with watchtower server every time when modules become enabled or disabled.'),
  );

  $form['server'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server configuration'),
  );

  return system_settings_form($form);
}
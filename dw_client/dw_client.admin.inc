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
  $reporting_enabled = variable_get('watchtower_reporting', FALSE);
  $form['client']['watchtower_reporting'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Watchtower reporting'),
    '#description' => t('Checking this enabling regular reporting to watchtower server about current drupal installation state.'),
    '#default_value' => $reporting_enabled,
  );
  $form['client']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="watchtower_reporting"]' => array('checked' => TRUE),
      ),
    ),
  );

  // If reporting enabled and we have valid response.
  if ($reporting_enabled && $instance_url = variable_get('watchtower_instance_info_url', FALSE)) {
    $link = l($instance_url, $instance_url, array('external' => TRUE));
    $form['client']['settings']['watchtower_instance_info_url'] = array(
      '#type' => 'markup',
      '#prefix' => '<div>',
      '#markup' => t('Your site information could be found on !url.', array('!url' => $link)),
      '#suffix' => '</div>',
    );
  }

  $reporting_server = variable_get('watchtower_server', '');
  $form['client']['settings']['watchtower_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Watchtower server address'),
    '#default_value' => $reporting_server,
    '#description' => t('Enter base address of watchtower server, for example: %server', array('%server' => 'http://example.com/')),
  );

  if (!function_exists('watchtower_get_drupal_site_key')) {
    // No drush running.
    module_load_include('inc', 'dw_client', 'watchtower.drush');
  }
  $server_link = rtrim($reporting_server, '/') . '/?q=watchtower/key/' . watchtower_get_drupal_site_key();
  $api_key = variable_get('watchtower_api_key', '');
  $form['client']['settings']['watchtower_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Your API key for %server', array('%server' => $reporting_server)),
    '#default_value' => $api_key,
    '#description' => t('Get a API key at <a href="!server" target="_blank">!server</a>', array(
      '!server' => check_url($server_link),
    ))
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
  $form = system_settings_form($form);
  $form['#validate'][] = 'dw_client_settings_form_validate';
  $form['#submit'][] = 'dw_client_settings_form_submit';
  return $form;
}

/**
 * Validate callback.
 */
function dw_client_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['watchtower_reporting']) {
    // Ensure trailing slash.
    $server_base_url = trim($values['watchtower_server']);
    $server_base_url = rtrim($server_base_url, '/') . '/';

    $server_uri = $server_base_url . '/xmlrpc.php';
    if (valid_url($server_uri)) {
      if (!function_exists('watchtower_get_drupal_site_key')) {
        // No drush running.
        module_load_include('inc', 'dw_client', 'watchtower.drush');
      }
      // Check server status and auth key.
      $current_site_key = watchtower_get_drupal_site_key();
      $api_key = trim($values['watchtower_api_key']);

      // @todo Add signature.
      $response = xmlrpc($server_uri, array(
        'drupal_watchtower.status' => array('1.0', $current_site_key, $api_key),
      ));
      if ($response && !empty($response['name']) && !empty($response['version'])) {
        if (empty($response['supported'])) {
          form_set_error('watchtower_server', t('The given server could not handle the v1.0 remote submission API.'));
        }
        else {
          drupal_set_message(t('Verified that the specified server can handle report submissions. Supported features: %features.', array('%features' => $response['features'])));
          if (empty($response['auth'])) {
            form_set_error('watchtower_api_key', t('Provide valid API key.'));
          }
          else {
            // Save valid trimmed values.
            form_set_value($form['client']['settings']['watchtower_server'], $server_base_url, $form_state);
            form_set_value($form['client']['settings']['watchtower_api_key'], $api_key, $form_state);
          }
        }
      }
      else {
        form_set_error('watchtower_server', t('Invalid watchtower server address specified. Make sure you specified the right server address.'));
      }
    }
    else {
      form_set_error('watchtower_server', t('You should provide a server address, such as http://example.com/xmlrpc.php'));
    }
  }
}

/**
 * Submit callback.
 */
function dw_client_settings_form_submit($form, $form_state) {
  if (_dw_client_report(TRUE)) {
    drupal_set_message(t('Report sent to configured server.'));
  }
  else {
    drupal_set_message(t('Error sending report: @error', array('@error' => xmlrpc_error_msg())), 'error');
  }
}

<?php
/**
 * @file
 * watchtower_storage.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function watchtower_storage_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'drupal_instances';
  $view->description = 'List of reports from drupal site instances';
  $view->tag = 'dw_server';
  $view->base_table = 'node';
  $view->human_name = 'Drupal instances';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Reports';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['text'] = 'node/[nid]';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'drupal_instance' => 'drupal_instance',
  );
  /* Filter criterion: Content: Modules (field_modules) */
  $handler->display->display_options['filters']['field_modules_tid']['id'] = 'field_modules_tid';
  $handler->display->display_options['filters']['field_modules_tid']['table'] = 'field_data_field_modules';
  $handler->display->display_options['filters']['field_modules_tid']['field'] = 'field_modules_tid';
  $handler->display->display_options['filters']['field_modules_tid']['value'] = '';
  $handler->display->display_options['filters']['field_modules_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_modules_tid']['expose']['operator_id'] = 'field_modules_tid_op';
  $handler->display->display_options['filters']['field_modules_tid']['expose']['label'] = 'Modules';
  $handler->display->display_options['filters']['field_modules_tid']['expose']['description'] = 'Filter reports by module usage';
  $handler->display->display_options['filters']['field_modules_tid']['expose']['operator'] = 'field_modules_tid_op';
  $handler->display->display_options['filters']['field_modules_tid']['expose']['identifier'] = 'field_modules_tid';
  $handler->display->display_options['filters']['field_modules_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_modules_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_modules_tid']['vocabulary'] = 'modules';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'reports_page');
  $handler->display->display_options['path'] = 'reports';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Drupal instances';
  $handler->display->display_options['menu']['description'] = 'List of reports from drupal site instances';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'reports');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Drupal instances';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '6';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'jump_menu';
  $handler->display->display_options['style_options']['path'] = 'nid';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $export['drupal_instances'] = $view;

  $view = new view();
  $view->name = 'patches';
  $view->description = 'List of patches';
  $view->tag = 'dw_server';
  $view->base_table = 'node';
  $view->human_name = 'Patches';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Patches';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'patch' => 'patch',
  );
  /* Filter criterion: Content: Fixed in (field_fixed_in) */
  $handler->display->display_options['filters']['field_fixed_in_tid']['id'] = 'field_fixed_in_tid';
  $handler->display->display_options['filters']['field_fixed_in_tid']['table'] = 'field_data_field_fixed_in';
  $handler->display->display_options['filters']['field_fixed_in_tid']['field'] = 'field_fixed_in_tid';
  $handler->display->display_options['filters']['field_fixed_in_tid']['value'] = '';
  $handler->display->display_options['filters']['field_fixed_in_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['operator_id'] = 'field_fixed_in_tid_op';
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['label'] = 'Modules';
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['description'] = 'Add one or more module names';
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['operator'] = 'field_fixed_in_tid_op';
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['identifier'] = 'field_fixed_in_tid';
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_fixed_in_tid']['vocabulary'] = 'modules';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'patches_page');
  $handler->display->display_options['path'] = 'patches';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Patches';
  $handler->display->display_options['menu']['description'] = 'List of patches from reports';
  $handler->display->display_options['menu']['weight'] = '11';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'patches_feed');
  $handler->display->display_options['enabled'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'patch' => 'patch',
  );
  /* Filter criterion: Content: Fixed in (field_fixed_in) */
  $handler->display->display_options['filters']['field_fixed_in_tid']['id'] = 'field_fixed_in_tid';
  $handler->display->display_options['filters']['field_fixed_in_tid']['table'] = 'field_data_field_fixed_in';
  $handler->display->display_options['filters']['field_fixed_in_tid']['field'] = 'field_fixed_in_tid';
  $handler->display->display_options['filters']['field_fixed_in_tid']['value'] = '';
  $handler->display->display_options['filters']['field_fixed_in_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['operator_id'] = 'field_fixed_in_tid_op';
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['label'] = 'Modules';
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['description'] = 'Add one or more module names';
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['operator'] = 'field_fixed_in_tid_op';
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['identifier'] = 'field_fixed_in_tid';
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_fixed_in_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_fixed_in_tid']['vocabulary'] = 'modules';
  $handler->display->display_options['path'] = 'patches.xml';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );
  $export['patches'] = $view;

  return $export;
}

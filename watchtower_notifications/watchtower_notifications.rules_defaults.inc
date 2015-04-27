<?php
/**
 * @file
 * watchtower_notifications.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function watchtower_notifications_default_rules_configuration() {
  $items = array();
  $items['rules_watchtower_alert_new_patches'] = entity_import('rules_config', '{ "rules_watchtower_alert_new_patches" : {
      "LABEL" : "Alert new patches",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "watchtower_notify" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "patch" : { "label" : "Patch node", "type" : "node" } },
      "ACTION SET" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "4" : "4" } },
            "subject" : "Watchtower new patch",
            "message" : "[patch:field-applicable-to] [patch:title]\\r\\nModule [patch:field-fixed-in]\\r\\nView patch [patch:url]\\r\\n\\r\\nWatchtower [site:url]\\r\\n",
            "from" : [ "site:mail" ]
          }
        }
      ]
    }
  }');
  $items['rules_watchtower_patch_new'] = entity_import('rules_config', '{ "rules_watchtower_patch_new" : {
      "LABEL" : "Watchtower patch - new",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "watchtower_notify" ],
      "REQUIRES" : [ "rules", "rules_scheduler" ],
      "ON" : { "node_insert--patch" : { "bundle" : "patch" } },
      "DO" : [
        { "schedule" : {
            "component" : "rules_watchtower_alert_new_patches",
            "date" : { "select" : "site:current-date", "date_offset" : { "value" : 5 } },
            "identifier" : "watchtower_alert_patch",
            "param_patch" : [ "node" ]
          }
        }
      ]
    }
  }');
  return $items;
}

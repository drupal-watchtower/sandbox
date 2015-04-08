<?php

/**
 * @file
 * Contains \Drupal\dw_server\Entity\Site.
 */

namespace Drupal\dw_server\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBundleBase;

/**
 * Defines the watchtower site entity.
 *
 * @ConfigEntityType(
 *   id = "watchtower_site",
 *   label = @Translation("Watchtower site"),
 *   handlers = {
 *     "list_builder" = "Drupal\dw_server\SiteListBuilder",
 *     "form" = {
 *       "add" = "Drupal\dw_server\SiteForm",
 *       "edit" = "Drupal\dw_server\SiteForm",
 *       "delete" = "\Drupal\Core\Entity\EntityDeleteForm"
 *     }
 *   },
 *   admin_permission = "administer watchtower",
 *   config_prefix = "site",
 *   bundle_of = "watchtower_report",
 *   entity_keys = {
 *     "id" = "id",
 *     "label" = "label",
 *   },
 *   links = {
 *     "add-form" = "/admin/config/system/manage/{watchtower_site}/add",
 *     "delete-form" = "/admin/config/system/manage/{watchtower_site}/delete",
 *     "edit-form" = "/admin/config/system/manage/{watchtower_site}",
 *     "collection" = "/admin/config/system",
 *   }
 * )
 */
class Site extends ConfigEntityBundleBase {

  /**
   * The site ID.
   *
   * @var string
   */
  protected $id;

  /**
   * Label of the site.
   *
   * @var string
   */
  protected $label;

  /**
   * Description of the site.
   *
   * @var string
   */
  protected $description;

  /**
   * Url of the site.
   *
   * @var string
   */
  protected $url;

}

<?php

/**
 * @file
 * Contains \Drupal\dw_server\Entity\Site.
 */

namespace Drupal\dw_server\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBundleBase;
use Drupal\Core\Url;
use Drupal\dw_server\SiteInterface;

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
 *       "delete" = "\Drupal\Core\Entity\EntityDeleteForm",
 *       "overview" = "\Drupal\dw_server\Form\SiteOverviewForm",
 *       "enable" = "\Drupal\dw_server\Form\SiteEnableForm",
 *       "disable" = "\Drupal\dw_server\Form\SiteDisableForm",
 *     }
 *   },
 *   admin_permission = "administer watchtower",
 *   config_prefix = "site",
 *   bundle_of = "watchtower_report",
 *   entity_keys = {
 *     "id" = "id",
 *     "label" = "label",
 *     "status" = "status",
 *   },
 *   links = {
 *     "collection" = "/admin/config/system/watchtower_server",
 *     "add-form" = "/admin/config/system/watchtower_server/add",
 *     "edit-form" = "/admin/config/system/watchtower_server/manage/{watchtower_site}",
 *     "overview-form" = "/admin/config/system/watchtower_server/manage/{watchtower_site}/overview",
 *     "disable" = "/admin/config/system/watchtower_server/manage/{watchtower_site}/disable",
 *     "enable" = "/admin/config/system/watchtower_server/manage/{watchtower_site}/enable",
 *     "delete-form" = "/admin/config/system/watchtower_server/manage/{watchtower_site}/delete",
 *   },
 * )
 */
class Site extends ConfigEntityBundleBase implements SiteInterface {

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

  /**
   * {@inheritdoc}
   */
  public function getDescription() {
    return $this->get('description');
  }

  /**
   * {@inheritdoc}
   */
  public function getSiteUrl() {
    return Url::fromUri($this->get('url'));
  }

}

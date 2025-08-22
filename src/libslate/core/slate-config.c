/* slate-config.c
 *
 * Copyright 2024 Geoff Johnson <geoff.jay@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "slate-config.h"
#include "../ui/slate-box.h"
#include <hcl.h>

struct _SlateConfig
{
  GObject parent_instance;

  HclDocument *document;
  gboolean loaded;
};

G_DEFINE_FINAL_TYPE (SlateConfig, slate_config, G_TYPE_OBJECT)

static void
slate_config_finalize (GObject *object)
{
  SlateConfig *self = SLATE_CONFIG (object);

  g_clear_object (&self->document);

  G_OBJECT_CLASS (slate_config_parent_class)->finalize (object);
}

static void
slate_config_class_init (SlateConfigClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = slate_config_finalize;
}

static void
slate_config_init (SlateConfig *self)
{
  self->document = NULL;
  self->loaded = FALSE;
}

/**
 * slate_config_new:
 *
 * Creates a new #SlateConfig.
 *
 * Returns: (transfer full): a new #SlateConfig
 */
SlateConfig *
slate_config_new (void)
{
  return g_object_new (SLATE_TYPE_CONFIG, NULL);
}

/**
 * slate_config_load_file:
 * @config: a #SlateConfig
 * @filename: path to the HCL configuration file
 * @error: return location for a #GError, or %NULL
 *
 * Loads configuration from an HCL file.
 *
 * Returns: %TRUE on success, %FALSE on error
 */
gboolean
slate_config_load_file (SlateConfig  *config,
                        const char   *filename,
                        GError      **error)
{
  g_return_val_if_fail (SLATE_IS_CONFIG (config), FALSE);
  g_return_val_if_fail (filename != NULL, FALSE);

  g_clear_object (&config->document);

  config->document = hcl_parse_file (filename, error);
  if (config->document == NULL)
    {
      config->loaded = FALSE;
      return FALSE;
    }

  config->loaded = TRUE;
  return TRUE;
}

/**
 * slate_config_load_string:
 * @config: a #SlateConfig
 * @hcl_string: HCL configuration string
 * @error: return location for a #GError, or %NULL
 *
 * Loads configuration from an HCL string.
 *
 * Returns: %TRUE on success, %FALSE on error
 */
gboolean
slate_config_load_string (SlateConfig  *config,
                          const char   *hcl_string,
                          GError      **error)
{
  g_return_val_if_fail (SLATE_IS_CONFIG (config), FALSE);
  g_return_val_if_fail (hcl_string != NULL, FALSE);

  g_clear_object (&config->document);

  config->document = hcl_parse_string (hcl_string, error);
  if (config->document == NULL)
    {
      config->loaded = FALSE;
      return FALSE;
    }

  config->loaded = TRUE;
  return TRUE;
}

/**
 * slate_config_get_document:
 * @config: a #SlateConfig
 *
 * Gets the loaded HCL document.
 *
 * Returns: (transfer none) (nullable): the HCL document, or %NULL if not loaded
 */
HclDocument *
slate_config_get_document (SlateConfig *config)
{
  g_return_val_if_fail (SLATE_IS_CONFIG (config), NULL);
  return config->document;
}

/**
 * slate_config_get_string_property:
 * @config: a #SlateConfig
 * @property: property name
 *
 * Gets a string property from the configuration.
 *
 * Returns: (transfer none) (nullable): the property value, or %NULL if not found
 */
const char *
slate_config_get_string_property (SlateConfig *config,
                                  const char  *property)
{
  HclValue *value;

  g_return_val_if_fail (SLATE_IS_CONFIG (config), NULL);
  g_return_val_if_fail (property != NULL, NULL);
  g_return_val_if_fail (config->loaded, NULL);

  if (!hcl_document_has_attribute (config->document, property))
    return NULL;

  value = hcl_document_get_attribute (config->document, property);
  if (!hcl_value_is_string (value))
    return NULL;

  return hcl_value_get_string (value);
}

/**
 * slate_config_get_boolean_property:
 * @config: a #SlateConfig
 * @property: property name
 *
 * Gets a boolean property from the configuration.
 *
 * Returns: the property value, or %FALSE if not found
 */
gboolean
slate_config_get_boolean_property (SlateConfig *config,
                                   const char  *property)
{
  HclValue *value;

  g_return_val_if_fail (SLATE_IS_CONFIG (config), FALSE);
  g_return_val_if_fail (property != NULL, FALSE);
  g_return_val_if_fail (config->loaded, FALSE);

  if (!hcl_document_has_attribute (config->document, property))
    return FALSE;

  value = hcl_document_get_attribute (config->document, property);
  if (!hcl_value_is_bool (value))
    return FALSE;

  return hcl_value_get_bool (value);
}

/**
 * slate_config_get_int_property:
 * @config: a #SlateConfig
 * @property: property name
 *
 * Gets an integer property from the configuration.
 *
 * Returns: the property value, or 0 if not found
 */
gint64
slate_config_get_int_property (SlateConfig *config,
                               const char  *property)
{
  HclValue *value;

  g_return_val_if_fail (SLATE_IS_CONFIG (config), 0);
  g_return_val_if_fail (property != NULL, 0);
  g_return_val_if_fail (config->loaded, 0);

  if (!hcl_document_has_attribute (config->document, property))
    return 0;

  value = hcl_document_get_attribute (config->document, property);
  if (!hcl_value_is_number (value))
    return 0;

  return hcl_value_get_int (value);
}

/**
 * slate_config_get_double_property:
 * @config: a #SlateConfig
 * @property: property name
 *
 * Gets a double property from the configuration.
 *
 * Returns: the property value, or 0.0 if not found
 */
gdouble
slate_config_get_double_property (SlateConfig *config,
                                  const char  *property)
{
  HclValue *value;

  g_return_val_if_fail (SLATE_IS_CONFIG (config), 0.0);
  g_return_val_if_fail (property != NULL, 0.0);
  g_return_val_if_fail (config->loaded, 0.0);

  if (!hcl_document_has_attribute (config->document, property))
    return 0.0;

  value = hcl_document_get_attribute (config->document, property);
  if (!hcl_value_is_number (value))
    return 0.0;

  return hcl_value_get_double (value);
}

/**
 * slate_config_get_objects_by_type:
 * @config: a #SlateConfig
 * @type: object type to search for
 *
 * Gets all HCL blocks of a specific type.
 *
 * Returns: (transfer container) (element-type HclBlock): list of HCL blocks
 */
GList *
slate_config_get_objects_by_type (SlateConfig *config,
                                  const char  *type)
{
  g_return_val_if_fail (SLATE_IS_CONFIG (config), NULL);
  g_return_val_if_fail (type != NULL, NULL);
  g_return_val_if_fail (config->loaded, NULL);

  return hcl_document_get_blocks_by_type (config->document, type);
}

/**
 * slate_config_create_object_from_block:
 * @config: a #SlateConfig
 * @block: HCL block to create object from
 * @error: return location for a #GError, or %NULL
 *
 * Creates a buildable object from an HCL block.
 *
 * Returns: (transfer full) (nullable): the created object, or %NULL on error
 */
SlateBuildable *
slate_config_create_object_from_block (SlateConfig  *config,
                                       HclBlock     *block,
                                       GError      **error)
{
  const char *block_type;
  SlateBuildable *object = NULL;

  g_return_val_if_fail (SLATE_IS_CONFIG (config), NULL);
  g_return_val_if_fail (HCL_IS_BLOCK (block), NULL);

    block_type = hcl_block_get_block_type (block);

  /* Create objects based on block type */
  if (g_strcmp0 (block_type, "object") == 0)
    {
      /* For object blocks, check the label (which is the object type) */
      const char *label = hcl_block_get_label (block);
      if (label != NULL)
        {
          if (g_strcmp0 (label, "box") == 0)
            {
              object = SLATE_BUILDABLE (slate_box_new ());
              slate_buildable_build_from_hcl_block (object, block);
            }
          /* Add more object types here as they are implemented */
        }
      else
        {
          /* Check for type attribute as fallback */
          HclValue *type_value = hcl_block_get_attribute (block, "type");
          if (type_value != NULL && hcl_value_is_string (type_value))
            {
              const char *object_type = hcl_value_get_string (type_value);

              if (g_strcmp0 (object_type, "box") == 0)
                {
                  object = SLATE_BUILDABLE (slate_box_new ());
                  slate_buildable_build_from_hcl_block (object, block);
                }
              /* Add more object types here as they are implemented */
            }
        }
    }
  else if (g_strcmp0 (block_type, "box") == 0)
    {
      /* Direct box block */
      object = SLATE_BUILDABLE (slate_box_new ());
      slate_buildable_build_from_hcl_block (object, block);
    }

  if (object == NULL && error != NULL)
    {
      g_set_error (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
                   "Unknown block type: %s", block_type);
    }

  return object;
}

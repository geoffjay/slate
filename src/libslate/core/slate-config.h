/* slate-config.h
 *
 * Copyright 2024 Slate Contributors
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

#pragma once

#include <glib-object.h>
#include <hcl.h>
#include "slate-buildable.h"

G_BEGIN_DECLS

#define SLATE_TYPE_CONFIG (slate_config_get_type())

G_DECLARE_FINAL_TYPE (SlateConfig, slate_config, SLATE, CONFIG, GObject)

/**
 * SlateConfig:
 *
 * Configuration manager that loads HCL configuration files and creates
 * buildable objects from them.
 */

/* Constructor */
SlateConfig   *slate_config_new                    (void);

/* Loading configuration */
gboolean       slate_config_load_file              (SlateConfig  *config,
                                                    const char   *filename,
                                                    GError      **error);

gboolean       slate_config_load_string            (SlateConfig  *config,
                                                    const char   *hcl_string,
                                                    GError      **error);

/* Accessing configuration */
HclDocument   *slate_config_get_document           (SlateConfig  *config);

/* Property access */
const char    *slate_config_get_string_property    (SlateConfig  *config,
                                                    const char   *property);

gboolean       slate_config_get_boolean_property   (SlateConfig  *config,
                                                    const char   *property);

gint64         slate_config_get_int_property        (SlateConfig  *config,
                                                    const char   *property);

gdouble        slate_config_get_double_property     (SlateConfig  *config,
                                                    const char   *property);

/* Object creation */
GList         *slate_config_get_objects_by_type    (SlateConfig  *config,
                                                    const char   *type);

SlateBuildable *slate_config_create_object_from_block (SlateConfig  *config,
                                                       HclBlock     *block,
                                                       GError      **error);

G_END_DECLS

/* slate-plugin-manager.h
 *
 * Copyright 2025 Slate Contributors
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
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <glib-object.h>
#include <libpeas.h>
#include "slate-plugin-interface.h"
#include "slate-header-bar-extension.h"

G_BEGIN_DECLS

#define SLATE_TYPE_PLUGIN_MANAGER (slate_plugin_manager_get_type())

G_DECLARE_FINAL_TYPE (SlatePluginManager, slate_plugin_manager, SLATE, PLUGIN_MANAGER, GObject)

/**
 * SlatePluginManager:
 *
 * Manages the loading, unloading, and lifecycle of Slate plugins.
 */

SlatePluginManager * slate_plugin_manager_new (void);
SlatePluginManager * slate_plugin_manager_get_default (void);

void slate_plugin_manager_add_search_path (SlatePluginManager *manager,
                                           const char         *path);
void slate_plugin_manager_remove_search_path (SlatePluginManager *manager,
                                              const char         *path);

gboolean slate_plugin_manager_load_plugin (SlatePluginManager *manager,
                                           const char         *plugin_name);
gboolean slate_plugin_manager_unload_plugin (SlatePluginManager *manager,
                                             const char         *plugin_name);

gboolean slate_plugin_manager_is_plugin_loaded (SlatePluginManager *manager,
                                                const char         *plugin_name);

GList * slate_plugin_manager_get_loaded_plugins (SlatePluginManager *manager);
GList * slate_plugin_manager_get_available_plugins (SlatePluginManager *manager);

GList * slate_plugin_manager_get_header_bar_extensions (SlatePluginManager *manager);

void slate_plugin_manager_refresh (SlatePluginManager *manager);

G_END_DECLS

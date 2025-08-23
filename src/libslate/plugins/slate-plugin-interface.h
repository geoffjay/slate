/* slate-plugin-interface.h
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

G_BEGIN_DECLS

#define SLATE_TYPE_PLUGIN_INTERFACE (slate_plugin_interface_get_type())

G_DECLARE_INTERFACE (SlatePluginInterface, slate_plugin_interface, SLATE, PLUGIN_INTERFACE, GObject)

/**
 * SlatePluginInterface:
 *
 * The main interface that all Slate plugins must implement.
 */
struct _SlatePluginInterfaceInterface
{
  GTypeInterface parent_interface;

  /**
   * activate:
   * @plugin: the plugin instance
   *
   * Called when the plugin is activated.
   */
  void (*activate) (SlatePluginInterface *plugin);

  /**
   * deactivate:
   * @plugin: the plugin instance
   *
   * Called when the plugin is deactivated.
   */
  void (*deactivate) (SlatePluginInterface *plugin);

  /**
   * get_name:
   * @plugin: the plugin instance
   *
   * Returns the human-readable name of the plugin.
   *
   * Returns: (transfer none): the plugin name
   */
  const char * (*get_name) (SlatePluginInterface *plugin);

  /**
   * get_description:
   * @plugin: the plugin instance
   *
   * Returns a description of what the plugin does.
   *
   * Returns: (transfer none): the plugin description
   */
  const char * (*get_description) (SlatePluginInterface *plugin);
};

void slate_plugin_interface_activate (SlatePluginInterface *plugin);
void slate_plugin_interface_deactivate (SlatePluginInterface *plugin);
const char * slate_plugin_interface_get_name (SlatePluginInterface *plugin);
const char * slate_plugin_interface_get_description (SlatePluginInterface *plugin);

G_END_DECLS

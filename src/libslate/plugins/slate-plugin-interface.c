/* slate-plugin-interface.c
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

#include "slate-plugin-interface.h"

G_DEFINE_INTERFACE (SlatePluginInterface, slate_plugin_interface, G_TYPE_OBJECT)

static void
slate_plugin_interface_default_init (SlatePluginInterfaceInterface *iface)
{
  /* Default implementations can be provided here if needed */
}

/**
 * slate_plugin_interface_activate:
 * @plugin: a #SlatePluginInterface
 *
 * Activates the plugin.
 */
void
slate_plugin_interface_activate (SlatePluginInterface *plugin)
{
  SlatePluginInterfaceInterface *iface;

  g_return_if_fail (SLATE_IS_PLUGIN_INTERFACE (plugin));

  iface = SLATE_PLUGIN_INTERFACE_GET_IFACE (plugin);
  if (iface->activate)
    iface->activate (plugin);
}

/**
 * slate_plugin_interface_deactivate:
 * @plugin: a #SlatePluginInterface
 *
 * Deactivates the plugin.
 */
void
slate_plugin_interface_deactivate (SlatePluginInterface *plugin)
{
  SlatePluginInterfaceInterface *iface;

  g_return_if_fail (SLATE_IS_PLUGIN_INTERFACE (plugin));

  iface = SLATE_PLUGIN_INTERFACE_GET_IFACE (plugin);
  if (iface->deactivate)
    iface->deactivate (plugin);
}

/**
 * slate_plugin_interface_get_name:
 * @plugin: a #SlatePluginInterface
 *
 * Gets the human-readable name of the plugin.
 *
 * Returns: (transfer none): the plugin name
 */
const char *
slate_plugin_interface_get_name (SlatePluginInterface *plugin)
{
  SlatePluginInterfaceInterface *iface;

  g_return_val_if_fail (SLATE_IS_PLUGIN_INTERFACE (plugin), NULL);

  iface = SLATE_PLUGIN_INTERFACE_GET_IFACE (plugin);
  if (iface->get_name)
    return iface->get_name (plugin);

  return "Unknown Plugin";
}

/**
 * slate_plugin_interface_get_description:
 * @plugin: a #SlatePluginInterface
 *
 * Gets a description of what the plugin does.
 *
 * Returns: (transfer none): the plugin description
 */
const char *
slate_plugin_interface_get_description (SlatePluginInterface *plugin)
{
  SlatePluginInterfaceInterface *iface;

  g_return_val_if_fail (SLATE_IS_PLUGIN_INTERFACE (plugin), NULL);

  iface = SLATE_PLUGIN_INTERFACE_GET_IFACE (plugin);
  if (iface->get_description)
    return iface->get_description (plugin);

  return "No description available.";
}

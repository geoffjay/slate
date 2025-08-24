/* hello-world-plugin.h
 *
 * Copyright 2025 Slate Development Team
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
#include <libpeas.h>
#include <slate.h>

G_BEGIN_DECLS

#define HELLO_WORLD_TYPE_PLUGIN (hello_world_plugin_get_type())

G_DECLARE_FINAL_TYPE (HelloWorldPlugin, hello_world_plugin, HELLO_WORLD, PLUGIN, PeasExtensionBase)

/**
 * HelloWorldPlugin:
 *
 * A simple example plugin that demonstrates Slate's plugin system.
 * This plugin adds a "Hello World" button to the header bar and provides
 * a simple dashboard widget.
 */

/**
 * hello_world_plugin_new:
 *
 * Creates a new #HelloWorldPlugin instance.
 *
 * Returns: (transfer full): A new #HelloWorldPlugin
 */
HelloWorldPlugin *hello_world_plugin_new (void);

/**
 * hello_world_plugin_get_name:
 * @plugin: A #HelloWorldPlugin
 *
 * Gets the display name of the plugin.
 *
 * Returns: (transfer none): The plugin name
 */
const char *hello_world_plugin_get_name (HelloWorldPlugin *plugin);

/**
 * hello_world_plugin_get_description:
 * @plugin: A #HelloWorldPlugin
 *
 * Gets the description of the plugin.
 *
 * Returns: (transfer none): The plugin description
 */
const char *hello_world_plugin_get_description (HelloWorldPlugin *plugin);

G_END_DECLS

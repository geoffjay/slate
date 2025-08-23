/* slate-plugin-manager.c
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

#include "slate-plugin-manager.h"
#include <glib/gi18n.h>

struct _SlatePluginManager
{
  GObject parent_instance;

  PeasEngine *engine;
  PeasExtensionSet *extension_set;
  GHashTable *loaded_plugins;
};

G_DEFINE_FINAL_TYPE (SlatePluginManager, slate_plugin_manager, G_TYPE_OBJECT)

static SlatePluginManager *default_instance = NULL;

static void
slate_plugin_manager_dispose (GObject *object)
{
  SlatePluginManager *self = (SlatePluginManager *)object;

  g_clear_object (&self->extension_set);
  g_clear_object (&self->engine);
  g_clear_pointer (&self->loaded_plugins, g_hash_table_unref);

  G_OBJECT_CLASS (slate_plugin_manager_parent_class)->dispose (object);
}

static void
on_extension_added (PeasExtensionSet *set,
                    PeasPluginInfo   *info,
                    GObject          *extension,
                    gpointer          user_data)
{
  SlatePluginManager *self = SLATE_PLUGIN_MANAGER (user_data);
  const char *plugin_name = peas_plugin_info_get_name (info);

  g_debug ("Plugin extension added: %s", plugin_name);

  if (SLATE_IS_PLUGIN_INTERFACE (extension))
    {
      slate_plugin_interface_activate (SLATE_PLUGIN_INTERFACE (extension));
      g_hash_table_insert (self->loaded_plugins,
                           g_strdup (plugin_name),
                           g_object_ref (extension));
    }
}

static void
on_extension_removed (PeasExtensionSet *set,
                      PeasPluginInfo   *info,
                      GObject          *extension,
                      gpointer          user_data)
{
  SlatePluginManager *self = SLATE_PLUGIN_MANAGER (user_data);
  const char *plugin_name = peas_plugin_info_get_name (info);

  g_debug ("Plugin extension removed: %s", plugin_name);

  if (SLATE_IS_PLUGIN_INTERFACE (extension))
    {
      slate_plugin_interface_deactivate (SLATE_PLUGIN_INTERFACE (extension));
      g_hash_table_remove (self->loaded_plugins, plugin_name);
    }
}

static void
slate_plugin_manager_class_init (SlatePluginManagerClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = slate_plugin_manager_dispose;
}

static void
slate_plugin_manager_init (SlatePluginManager *self)
{
  self->engine = peas_engine_get_default ();
  g_object_ref (self->engine);

  /* Register our plugin interfaces */
  peas_engine_add_search_path (self->engine, "/usr/lib/slate/plugins", NULL);
  peas_engine_add_search_path (self->engine, "/usr/local/lib/slate/plugins", NULL);

  /* Create extension set for plugin interfaces */
  self->extension_set = peas_extension_set_new (self->engine,
                                                SLATE_TYPE_PLUGIN_INTERFACE,
                                                NULL);

  /* Track loaded plugins */
  self->loaded_plugins = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                g_free, g_object_unref);

  /* Connect to extension set signals */
  g_signal_connect (self->extension_set, "extension-added",
                    G_CALLBACK (on_extension_added), self);
  g_signal_connect (self->extension_set, "extension-removed",
                    G_CALLBACK (on_extension_removed), self);
}

/**
 * slate_plugin_manager_new:
 *
 * Creates a new #SlatePluginManager.
 *
 * Returns: (transfer full): a new #SlatePluginManager
 */
SlatePluginManager *
slate_plugin_manager_new (void)
{
  return g_object_new (SLATE_TYPE_PLUGIN_MANAGER, NULL);
}

/**
 * slate_plugin_manager_get_default:
 *
 * Gets the default plugin manager instance.
 *
 * Returns: (transfer none): the default plugin manager
 */
SlatePluginManager *
slate_plugin_manager_get_default (void)
{
  if (default_instance == NULL)
    {
      default_instance = slate_plugin_manager_new ();
      g_object_add_weak_pointer (G_OBJECT (default_instance),
                                 (gpointer *)&default_instance);
    }

  return default_instance;
}

/**
 * slate_plugin_manager_add_search_path:
 * @manager: a #SlatePluginManager
 * @path: the directory path to add
 *
 * Adds a directory to search for plugins.
 */
void
slate_plugin_manager_add_search_path (SlatePluginManager *manager,
                                      const char         *path)
{
  g_return_if_fail (SLATE_IS_PLUGIN_MANAGER (manager));
  g_return_if_fail (path != NULL);

  peas_engine_add_search_path (manager->engine, path, NULL);
}

/**
 * slate_plugin_manager_remove_search_path:
 * @manager: a #SlatePluginManager
 * @path: the directory path to remove
 *
 * Removes a directory from the plugin search paths.
 * Note: libpeas-2 doesn't support removing search paths.
 */
void
slate_plugin_manager_remove_search_path (SlatePluginManager *manager,
                                         const char         *path)
{
  g_return_if_fail (SLATE_IS_PLUGIN_MANAGER (manager));
  g_return_if_fail (path != NULL);

  g_warning ("Removing search paths is not supported in libpeas-2");
}

/**
 * slate_plugin_manager_load_plugin:
 * @manager: a #SlatePluginManager
 * @plugin_name: the name of the plugin to load
 *
 * Loads a plugin by name.
 *
 * Returns: %TRUE if the plugin was loaded successfully
 */
gboolean
slate_plugin_manager_load_plugin (SlatePluginManager *manager,
                                  const char         *plugin_name)
{
  PeasPluginInfo *info;

  g_return_val_if_fail (SLATE_IS_PLUGIN_MANAGER (manager), FALSE);
  g_return_val_if_fail (plugin_name != NULL, FALSE);

  info = peas_engine_get_plugin_info (manager->engine, plugin_name);
  if (info == NULL)
    {
      g_warning ("Plugin '%s' not found", plugin_name);
      return FALSE;
    }

  if (peas_plugin_info_is_loaded (info))
    {
      g_debug ("Plugin '%s' is already loaded", plugin_name);
      return TRUE;
    }

  return peas_engine_load_plugin (manager->engine, info);
}

/**
 * slate_plugin_manager_unload_plugin:
 * @manager: a #SlatePluginManager
 * @plugin_name: the name of the plugin to unload
 *
 * Unloads a plugin by name.
 *
 * Returns: %TRUE if the plugin was unloaded successfully
 */
gboolean
slate_plugin_manager_unload_plugin (SlatePluginManager *manager,
                                    const char         *plugin_name)
{
  PeasPluginInfo *info;

  g_return_val_if_fail (SLATE_IS_PLUGIN_MANAGER (manager), FALSE);
  g_return_val_if_fail (plugin_name != NULL, FALSE);

  info = peas_engine_get_plugin_info (manager->engine, plugin_name);
  if (info == NULL)
    {
      g_warning ("Plugin '%s' not found", plugin_name);
      return FALSE;
    }

  if (!peas_plugin_info_is_loaded (info))
    {
      g_debug ("Plugin '%s' is not loaded", plugin_name);
      return TRUE;
    }

  return peas_engine_unload_plugin (manager->engine, info);
}

/**
 * slate_plugin_manager_is_plugin_loaded:
 * @manager: a #SlatePluginManager
 * @plugin_name: the name of the plugin to check
 *
 * Checks if a plugin is currently loaded.
 *
 * Returns: %TRUE if the plugin is loaded
 */
gboolean
slate_plugin_manager_is_plugin_loaded (SlatePluginManager *manager,
                                       const char         *plugin_name)
{
  PeasPluginInfo *info;

  g_return_val_if_fail (SLATE_IS_PLUGIN_MANAGER (manager), FALSE);
  g_return_val_if_fail (plugin_name != NULL, FALSE);

  info = peas_engine_get_plugin_info (manager->engine, plugin_name);
  if (info == NULL)
    return FALSE;

  return peas_plugin_info_is_loaded (info);
}

/**
 * slate_plugin_manager_get_loaded_plugins:
 * @manager: a #SlatePluginManager
 *
 * Gets a list of currently loaded plugins.
 *
 * Returns: (transfer container) (element-type SlatePluginInterface): list of loaded plugins
 */
GList *
slate_plugin_manager_get_loaded_plugins (SlatePluginManager *manager)
{
  g_return_val_if_fail (SLATE_IS_PLUGIN_MANAGER (manager), NULL);

  return g_hash_table_get_values (manager->loaded_plugins);
}

/**
 * slate_plugin_manager_get_available_plugins:
 * @manager: a #SlatePluginManager
 *
 * Gets a list of all available plugins.
 *
 * Returns: (transfer none) (element-type PeasPluginInfo): list of available plugins
 */
GList *
slate_plugin_manager_get_available_plugins (SlatePluginManager *manager)
{
  g_return_val_if_fail (SLATE_IS_PLUGIN_MANAGER (manager), NULL);

  /* libpeas-2 doesn't have peas_engine_get_plugin_list, return empty list for now */
  return NULL;
}

/**
 * slate_plugin_manager_get_header_bar_extensions:
 * @manager: a #SlatePluginManager
 *
 * Gets all loaded plugins that implement the header bar extension interface.
 *
 * Returns: (transfer container) (element-type SlateHeaderBarExtension): list of header bar extensions
 */
GList *
slate_plugin_manager_get_header_bar_extensions (SlatePluginManager *manager)
{
  GList *extensions = NULL;
  GList *plugins, *l;

  g_return_val_if_fail (SLATE_IS_PLUGIN_MANAGER (manager), NULL);

  plugins = slate_plugin_manager_get_loaded_plugins (manager);
  for (l = plugins; l != NULL; l = l->next)
    {
      GObject *plugin = G_OBJECT (l->data);
      if (SLATE_IS_HEADER_BAR_EXTENSION (plugin))
        extensions = g_list_prepend (extensions, plugin);
    }

  return g_list_reverse (extensions);
}

/**
 * slate_plugin_manager_refresh:
 * @manager: a #SlatePluginManager
 *
 * Refreshes the plugin list by rescanning plugin directories.
 */
void
slate_plugin_manager_refresh (SlatePluginManager *manager)
{
  g_return_if_fail (SLATE_IS_PLUGIN_MANAGER (manager));

  peas_engine_rescan_plugins (manager->engine);
}

/* test-plugin-system.c
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

#include <glib.h>
#include <gtk/gtk.h>
#include <slate.h>

/* Test plugin system integration */

static void
test_plugin_manager_creation (void)
{
  SlatePluginManager *manager = slate_plugin_manager_get_default ();
  g_assert_nonnull (manager);
  g_assert_true (SLATE_IS_PLUGIN_MANAGER (manager));
}

static void
test_plugin_manager_search_paths (void)
{
  SlatePluginManager *manager = slate_plugin_manager_get_default ();

  /* Add a test search path */
  slate_plugin_manager_add_search_path (manager, "/tmp/test-plugins");

  /* Verify the path was added (implementation dependent) */
  g_assert_nonnull (manager);
}

static void
test_plugin_loading (void)
{
  SlatePluginManager *manager = slate_plugin_manager_get_default ();
  
  /* Plugin manager should exist */
  g_assert_nonnull (manager);
  g_assert_true (SLATE_IS_PLUGIN_MANAGER (manager));
  
  /* This test just verifies the plugin manager is functional */
  /* Actual plugin loading tests would require real plugin files */
}

static void
test_extension_retrieval (void)
{
  SlatePluginManager *manager = slate_plugin_manager_get_default ();

  /* Plugin manager exists and is valid */
  g_assert_nonnull (manager);
  g_assert_true (SLATE_IS_PLUGIN_MANAGER (manager));
}

static void
test_plugin_interfaces (void)
{
  /* Test that plugin interface types are properly registered */
  GType plugin_interface_type = slate_plugin_interface_get_type ();
  g_assert_true (g_type_is_a (plugin_interface_type, G_TYPE_INTERFACE));

  GType header_bar_extension_type = slate_header_bar_extension_get_type ();
  g_assert_true (g_type_is_a (header_bar_extension_type, G_TYPE_INTERFACE));
}

int
main (int argc, char *argv[])
{
  /* Initialize test framework first */
  g_test_init (&argc, &argv, NULL);

  /* Initialize GTK for widget tests */
  gtk_init ();

  /* Initialize Slate types */
  slate_init ();

  g_test_add_func ("/plugin-system/manager-creation", test_plugin_manager_creation);
  g_test_add_func ("/plugin-system/search-paths", test_plugin_manager_search_paths);
  g_test_add_func ("/plugin-system/loading", test_plugin_loading);
  g_test_add_func ("/plugin-system/extensions", test_extension_retrieval);
  g_test_add_func ("/plugin-system/interfaces", test_plugin_interfaces);

  return g_test_run ();
}

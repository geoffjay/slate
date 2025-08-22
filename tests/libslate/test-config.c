/* test-config.c
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

#include <glib.h>
#include <gtk/gtk.h>
#include "../../src/libslate/core/slate-config.h"
#include "../../src/libslate/ui/slate-box.h"

static void
test_config_basic (void)
{
  SlateConfig *config;
  GError *error = NULL;
  const char *hcl_config =
    "app = \"Test App\"\n"
    "dark_theme = true\n"
    "\n"
    "object \"box\" {\n"
    "  id = \"test-box\"\n"
    "  orientation = \"horizontal\"\n"
    "  spacing = 10\n"
    "  homogeneous = true\n"
    "}";

    config = slate_config_new ();
  g_assert_nonnull (config);

  /* Load configuration from string */
  g_assert_true (slate_config_load_string (config, hcl_config, &error));
  g_assert_no_error (error);

  /* Test property access */
  g_assert_cmpstr (slate_config_get_string_property (config, "app"), ==, "Test App");
  g_assert_true (slate_config_get_boolean_property (config, "dark_theme"));

  /* Test object creation */
  GList *objects = slate_config_get_objects_by_type (config, "object");
  g_assert_nonnull (objects);
  g_assert_cmpint (g_list_length (objects), ==, 1);

  HclBlock *block = HCL_BLOCK (objects->data);
  SlateBuildable *buildable = slate_config_create_object_from_block (config, block, &error);
  g_assert_nonnull (buildable);
  g_assert_no_error (error);
  g_assert_true (SLATE_IS_BOX (buildable));

  /* Sink the floating reference */
  g_object_ref_sink (buildable);

  SlateBox *box = SLATE_BOX (buildable);
  g_assert_cmpstr (slate_box_get_id (box), ==, "test-box");
  g_assert_cmpint (slate_box_get_slate_orientation (box), ==, SLATE_ORIENTATION_HORIZONTAL);
  g_assert_true (slate_box_get_homogeneous (box));

  g_list_free (objects);
  g_object_unref (buildable);
  g_object_unref (config);
}

static void
test_config_nested_objects (void)
{
  SlateConfig *config;
  GError *error = NULL;
  const char *hcl_config =
    "object \"box\" {\n"
    "  id = \"parent-box\"\n"
    "  orientation = \"vertical\"\n"
    "  \n"
    "  object \"box\" {\n"
    "    id = \"child-box\"\n"
    "    orientation = \"horizontal\"\n"
    "  }\n"
    "}";

  config = slate_config_new ();
  g_assert_nonnull (config);

  g_assert_true (slate_config_load_string (config, hcl_config, &error));
  g_assert_no_error (error);

  /* Test that we can find the parent object */
  GList *objects = slate_config_get_objects_by_type (config, "object");
  g_assert_nonnull (objects);
  g_assert_cmpint (g_list_length (objects), ==, 1);

  g_list_free (objects);
  g_object_unref (config);
}

int
main (int argc, char *argv[])
{
  gtk_test_init (&argc, &argv, NULL);

  g_test_add_func ("/slate/config/basic", test_config_basic);
  g_test_add_func ("/slate/config/nested-objects", test_config_nested_objects);

  return g_test_run ();
}

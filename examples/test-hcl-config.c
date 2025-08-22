/* test-hcl-config.c - Simple demonstration of HCL configuration
 *
 * Copyright 2024 Geoff Johnson <geoff.jay@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <gtk/gtk.h>
#include "../src/libslate/core/slate-config.h"
#include "../src/libslate/ui/slate-box.h"

static const char *sample_config =
  "app_name = \"Slate Demo\"\n"
  "version = \"1.0\"\n"
  "debug = true\n"
  "\n"
  "object \"box\" {\n"
  "  id = \"main-container\"\n"
  "  orientation = \"vertical\"\n"
  "  spacing = 10\n"
  "  homogeneous = false\n"
  "}\n"
  "\n"
  "object \"box\" {\n"
  "  id = \"toolbar\"\n"
  "  orientation = \"horizontal\"\n"
  "  spacing = 5\n"
  "  homogeneous = true\n"
  "}";

int main(int argc, char *argv[])
{
  gtk_init ();

  g_print ("=== Slate HCL Configuration Demo ===\n\n");

  /* Create and load configuration */
  SlateConfig *config = slate_config_new ();
  GError *error = NULL;

  if (!slate_config_load_string (config, sample_config, &error)) {
    g_printerr ("Failed to load configuration: %s\n", error->message);
    g_error_free (error);
    return 1;
  }

  g_print ("✓ Configuration loaded successfully\n");

  /* Access configuration properties */
  g_print ("✓ App Name: %s\n", slate_config_get_string_property (config, "app_name"));
  g_print ("✓ Version: %s\n", slate_config_get_string_property (config, "version"));
  g_print ("✓ Debug Mode: %s\n", slate_config_get_boolean_property (config, "debug") ? "enabled" : "disabled");

  /* Create objects from configuration */
  GList *objects = slate_config_get_objects_by_type (config, "object");
  g_print ("✓ Found %u object(s) in configuration\n", g_list_length (objects));

  for (GList *iter = objects; iter != NULL; iter = iter->next) {
    HclBlock *block = HCL_BLOCK (iter->data);
    SlateBuildable *buildable = slate_config_create_object_from_block (config, block, &error);

    if (buildable && SLATE_IS_BOX (buildable)) {
      g_object_ref_sink (buildable);  /* Sink floating reference */

      SlateBox *box = SLATE_BOX (buildable);
      g_print ("  → Created SlateBox with ID: %s\n", slate_box_get_id (box));
      g_print ("    Orientation: %s\n",
               slate_box_get_slate_orientation (box) == SLATE_ORIENTATION_HORIZONTAL ?
               "horizontal" : "vertical");
      g_print ("    Homogeneous: %s\n", slate_box_get_homogeneous (box) ? "yes" : "no");

      g_object_unref (buildable);
    }
  }

  g_list_free (objects);
  g_object_unref (config);

  g_print ("\n=== Demo completed successfully! ===\n");
  return 0;
}

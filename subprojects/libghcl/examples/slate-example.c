/* slate-example.c - Example showing HCL integration with GObject
 *
 * Copyright 2024 The Slate Project
 */

#include <glib.h>
#include <hcl.h>

/* Example Slate configuration objects */

typedef struct _SlateAppConfig {
  GObject parent_instance;

  gchar *name;
  gchar *title;
  gboolean debug;
} SlateAppConfig;

#define SLATE_TYPE_APP_CONFIG (slate_app_config_get_type())
G_DECLARE_FINAL_TYPE (SlateAppConfig, slate_app_config, SLATE, APP_CONFIG, GObject)

G_DEFINE_FINAL_TYPE (SlateAppConfig, slate_app_config, G_TYPE_OBJECT)

static void
slate_app_config_finalize (GObject *object)
{
  SlateAppConfig *self = SLATE_APP_CONFIG (object);

  g_free (self->name);
  g_free (self->title);

  G_OBJECT_CLASS (slate_app_config_parent_class)->finalize (object);
}

static void
slate_app_config_class_init (SlateAppConfigClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = slate_app_config_finalize;
}

static void
slate_app_config_init (SlateAppConfig *self)
{
  (void)self; /* Suppress unused parameter warning */
}

SlateAppConfig *
slate_app_config_new (void)
{
  return g_object_new (SLATE_TYPE_APP_CONFIG, NULL);
}

/* Helper function to populate GObject from HCL document */
static gboolean
slate_app_config_from_hcl_document (SlateAppConfig *config, HclDocument *document, GError **error)
{
  /* Get application blocks */
  GList *app_blocks = hcl_document_get_blocks_by_type (document, "application");
  if (!app_blocks) {
    g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_INVAL,
                 "No application block found in configuration");
    return FALSE;
  }

  HclBlock *app_block = app_blocks->data;
  const gchar *app_name = hcl_block_get_label (app_block);

  if (app_name) {
    config->name = g_strdup (app_name);
  }

  /* Get title attribute */
  HclValue *title_value = hcl_block_get_attribute (app_block, "title");
  if (title_value && hcl_value_is_string (title_value)) {
    config->title = g_strdup (hcl_value_get_string (title_value));
  }

  /* Get debug attribute */
  HclValue *debug_value = hcl_block_get_attribute (app_block, "debug");
  if (debug_value && hcl_value_is_bool (debug_value)) {
    config->debug = hcl_value_get_bool (debug_value);
  }

  /* Process dashboard configuration */
  GList *dashboard_blocks = hcl_block_get_blocks_by_type (app_block, "dashboard");
  if (dashboard_blocks) {
    HclBlock *dashboard_block = dashboard_blocks->data;
    g_print ("Found dashboard block\n");

    /* Process box widgets */
    GList *box_blocks = hcl_block_get_blocks_by_type (dashboard_block, "box");
    for (GList *l = box_blocks; l; l = l->next) {
      HclBlock *box_block = l->data;
      const gchar *box_label = hcl_block_get_label (box_block);

      g_print ("Found box widget: %s\n", box_label ? box_label : "(unlabeled)");

      /* Get box attributes */
      HclValue *width = hcl_block_get_attribute (box_block, "width");
      HclValue *height = hcl_block_get_attribute (box_block, "height");

      if (width && hcl_value_is_string (width)) {
        g_print ("  width: %s\n", hcl_value_get_string (width));
      }

      if (height && hcl_value_is_string (height)) {
        g_print ("  height: %s\n", hcl_value_get_string (height));
      }
    }

    g_list_free (box_blocks);
    g_list_free (dashboard_blocks);
  }

  g_list_free (app_blocks);
  return TRUE;
}

int
main (int argc, char **argv)
{
  (void)argc; /* Suppress unused parameter warning */
  (void)argv; /* Suppress unused parameter warning */
  /* Example HCL configuration */
  static const char *config =
    "application \"slate-example\" {\n"
    "  title = \"My Slate Application\"\n"
    "  debug = true\n"
    "  \n"
    "  dashboard {\n"
    "    title = \"Main Dashboard\"\n"
    "    \n"
    "    box \"container\" {\n"
    "      width = \"full\"\n"
    "      height = \"full\"\n"
    "      orientation = \"vertical\"\n"
    "      \n"
    "      box \"header\" {\n"
    "        width = \"full\"\n"
    "        height = \"auto\"\n"
    "      }\n"
    "      \n"
    "      box \"content\" {\n"
    "        width = \"full\"\n"
    "        height = \"expand\"\n"
    "        orientation = \"horizontal\"\n"
    "      }\n"
    "    }\n"
    "  }\n"
    "}\n";

  g_autoptr(GError) error = NULL;

  /* Parse HCL configuration */
  g_print ("Parsing HCL configuration...\n");
  g_autoptr(HclDocument) document = hcl_parse_string (config, &error);

  if (!document) {
    g_printerr ("Failed to parse HCL: %s\n", error->message);
    return 1;
  }

  g_print ("HCL parsing successful!\n\n");

  /* Create application configuration object */
  g_autoptr(SlateAppConfig) app_config = slate_app_config_new ();

  /* Populate from HCL document */
  if (!slate_app_config_from_hcl_document (app_config, document, &error)) {
    g_printerr ("Failed to configure from HCL: %s\n", error->message);
    return 1;
  }

  /* Print configuration */
  g_print ("Application Configuration:\n");
  g_print ("  Name: %s\n", app_config->name ? app_config->name : "(none)");
  g_print ("  Title: %s\n", app_config->title ? app_config->title : "(none)");
  g_print ("  Debug: %s\n", app_config->debug ? "true" : "false");

  return 0;
}

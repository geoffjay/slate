/* hello-world-plugin.c
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

#include "hello-world-plugin.h"
#include "hello-world-widget.h"
#include <adwaita.h>

struct _HelloWorldPlugin
{
  PeasExtensionBase parent_instance;

  /* Plugin state */
  gboolean active;
  GtkWidget *header_button;
  HelloWorldWidget *dashboard_widget;
};

static void slate_plugin_interface_init (SlatePluginInterfaceInterface *iface);
static void slate_header_bar_extension_interface_init (SlateHeaderBarExtensionInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE (HelloWorldPlugin, hello_world_plugin, PEAS_TYPE_EXTENSION_BASE,
                               G_IMPLEMENT_INTERFACE (SLATE_TYPE_PLUGIN_INTERFACE, slate_plugin_interface_init)
                               G_IMPLEMENT_INTERFACE (SLATE_TYPE_HEADER_BAR_EXTENSION, slate_header_bar_extension_interface_init))

/* Plugin Interface Implementation */

static void
hello_world_plugin_activate (SlatePluginInterface *plugin)
{
  HelloWorldPlugin *self = HELLO_WORLD_PLUGIN (plugin);

  g_debug ("HelloWorld plugin activated");
  self->active = TRUE;

  /* Create dashboard widget */
  self->dashboard_widget = hello_world_widget_new ();

  /* Plugin is now ready */
  g_signal_emit_by_name (plugin, "ready");
}

static void
hello_world_plugin_deactivate (SlatePluginInterface *plugin)
{
  HelloWorldPlugin *self = HELLO_WORLD_PLUGIN (plugin);

  g_debug ("HelloWorld plugin deactivated");
  self->active = FALSE;

  /* Clean up resources */
  if (self->header_button) {
    gtk_widget_unparent (self->header_button);
    self->header_button = NULL;
  }

  if (self->dashboard_widget) {
    g_object_unref (self->dashboard_widget);
    self->dashboard_widget = NULL;
  }
}

static const char *
hello_world_plugin_get_plugin_name (SlatePluginInterface *plugin)
{
  return "Hello World";
}

static const char *
hello_world_plugin_get_plugin_description (SlatePluginInterface *plugin)
{
  return "A simple example plugin demonstrating Slate's plugin system";
}

/* Header Bar Extension Implementation */

static void
on_hello_button_clicked (GtkButton *button, gpointer user_data)
{
  /* Show a simple toast notification */
  AdwToast *toast = adw_toast_new ("Hello from the Hello World plugin!");
  adw_toast_set_timeout (toast, 3);

  /* Find the main window to show the toast */
  GtkRoot *toplevel = gtk_widget_get_root (GTK_WIDGET (button));
  if (ADW_IS_APPLICATION_WINDOW (toplevel)) {
    adw_toast_overlay_add_toast (
      ADW_TOAST_OVERLAY (gtk_widget_get_first_child (GTK_WIDGET (toplevel))),
      toast
    );
  }
}

static GList *
hello_world_plugin_create_start_widgets (SlateHeaderBarExtension *extension,
                                         SlateHeaderBar          *header_bar)
{
  /* No start widgets for this plugin */
  return NULL;
}

static GList *
hello_world_plugin_create_end_widgets (SlateHeaderBarExtension *extension,
                                       SlateHeaderBar          *header_bar)
{
  HelloWorldPlugin *self = HELLO_WORLD_PLUGIN (extension);

  /* Create a simple button for the header bar */
  self->header_button = gtk_button_new_with_label ("Hello");
  gtk_widget_set_tooltip_text (self->header_button, "Click to say hello!");

  /* Connect the click signal */
  g_signal_connect (self->header_button, "clicked",
                    G_CALLBACK (on_hello_button_clicked), self);

  /* Return a list with our button */
  return g_list_append (NULL, self->header_button);
}

static void
hello_world_plugin_on_project_changed (SlateHeaderBarExtension *extension,
                                       SlateHeaderBar          *header_bar,
                                       const char              *project_path)
{
  /* Update button state based on project */
  HelloWorldPlugin *self = HELLO_WORLD_PLUGIN (extension);

  if (self->header_button) {
    gtk_widget_set_sensitive (self->header_button, project_path != NULL);
  }
}

/* Interface Implementations */

static void
slate_plugin_interface_init (SlatePluginInterfaceInterface *iface)
{
  iface->activate = hello_world_plugin_activate;
  iface->deactivate = hello_world_plugin_deactivate;
  iface->get_name = hello_world_plugin_get_plugin_name;
  iface->get_description = hello_world_plugin_get_plugin_description;
}

static void
slate_header_bar_extension_interface_init (SlateHeaderBarExtensionInterface *iface)
{
  iface->create_start_widgets = hello_world_plugin_create_start_widgets;
  iface->create_end_widgets = hello_world_plugin_create_end_widgets;
  iface->on_project_changed = hello_world_plugin_on_project_changed;
}

/* Class Implementation */

static void
hello_world_plugin_dispose (GObject *object)
{
  HelloWorldPlugin *self = HELLO_WORLD_PLUGIN (object);

  /* Clean up any remaining resources */
  if (self->header_button) {
    g_object_unref (self->header_button);
    self->header_button = NULL;
  }

  if (self->dashboard_widget) {
    g_object_unref (self->dashboard_widget);
    self->dashboard_widget = NULL;
  }

  G_OBJECT_CLASS (hello_world_plugin_parent_class)->dispose (object);
}

static void
hello_world_plugin_class_init (HelloWorldPluginClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = hello_world_plugin_dispose;
}

static void
hello_world_plugin_init (HelloWorldPlugin *self)
{
  self->active = FALSE;
  self->header_button = NULL;
  self->dashboard_widget = NULL;
}

/* Public API */

HelloWorldPlugin *
hello_world_plugin_new (void)
{
  return g_object_new (HELLO_WORLD_TYPE_PLUGIN, NULL);
}

const char *
hello_world_plugin_get_name (HelloWorldPlugin *plugin)
{
  g_return_val_if_fail (HELLO_WORLD_IS_PLUGIN (plugin), NULL);
  return hello_world_plugin_get_plugin_name (SLATE_PLUGIN_INTERFACE (plugin));
}

const char *
hello_world_plugin_get_description (HelloWorldPlugin *plugin)
{
  g_return_val_if_fail (HELLO_WORLD_IS_PLUGIN (plugin), NULL);
  return hello_world_plugin_get_plugin_description (SLATE_PLUGIN_INTERFACE (plugin));
}

/* Plugin Entry Point */
G_MODULE_EXPORT void
peas_register_types (PeasObjectModule *module)
{
  peas_object_module_register_extension_type (module,
                                              SLATE_TYPE_PLUGIN_INTERFACE,
                                              HELLO_WORLD_TYPE_PLUGIN);

  peas_object_module_register_extension_type (module,
                                              SLATE_TYPE_HEADER_BAR_EXTENSION,
                                              HELLO_WORLD_TYPE_PLUGIN);
}

/* hello-world-widget.c
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

#include "hello-world-widget.h"
#include <adwaita.h>

struct _HelloWorldWidget
{
  SlateDashboardCard parent_instance;
  
  /* UI Elements */
  GtkLabel *greeting_label;
  GtkButton *change_button;
  GtkEntry *greeting_entry;
  GtkBox *content_box;
  
  /* State */
  char *greeting;
  gboolean editing;
};

G_DEFINE_FINAL_TYPE (HelloWorldWidget, hello_world_widget, SLATE_TYPE_DASHBOARD_CARD)

enum {
  PROP_0,
  PROP_GREETING,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

/* Signal handlers */

static void
on_change_button_clicked (GtkButton *button, gpointer user_data)
{
  HelloWorldWidget *self = HELLO_WORLD_WIDGET (user_data);
  
  if (!self->editing) {
    /* Switch to editing mode */
    gtk_widget_set_visible (GTK_WIDGET (self->greeting_label), FALSE);
    gtk_widget_set_visible (GTK_WIDGET (self->greeting_entry), TRUE);
    gtk_entry_set_text (self->greeting_entry, self->greeting);
    gtk_widget_grab_focus (GTK_WIDGET (self->greeting_entry));
    gtk_button_set_label (button, "Save");
    self->editing = TRUE;
  } else {
    /* Save and switch back to display mode */
    const char *new_greeting = gtk_entry_get_text (self->greeting_entry);
    hello_world_widget_set_greeting (self, new_greeting);
    
    gtk_widget_set_visible (GTK_WIDGET (self->greeting_entry), FALSE);
    gtk_widget_set_visible (GTK_WIDGET (self->greeting_label), TRUE);
    gtk_button_set_label (button, "Change");
    self->editing = FALSE;
  }
}

static void
on_entry_activate (GtkEntry *entry, gpointer user_data)
{
  HelloWorldWidget *self = HELLO_WORLD_WIDGET (user_data);
  
  /* Simulate clicking the save button */
  g_signal_emit_by_name (self->change_button, "clicked");
}

/* Property implementation */

static void
hello_world_widget_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  HelloWorldWidget *self = HELLO_WORLD_WIDGET (object);
  
  switch (prop_id) {
    case PROP_GREETING:
      g_value_set_string (value, self->greeting);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
  }
}

static void
hello_world_widget_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  HelloWorldWidget *self = HELLO_WORLD_WIDGET (object);
  
  switch (prop_id) {
    case PROP_GREETING:
      hello_world_widget_set_greeting (self, g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
  }
}

/* Object lifecycle */

static void
hello_world_widget_dispose (GObject *object)
{
  HelloWorldWidget *self = HELLO_WORLD_WIDGET (object);
  
  /* Clear references to child widgets */
  self->greeting_label = NULL;
  self->change_button = NULL;
  self->greeting_entry = NULL;
  self->content_box = NULL;
  
  G_OBJECT_CLASS (hello_world_widget_parent_class)->dispose (object);
}

static void
hello_world_widget_finalize (GObject *object)
{
  HelloWorldWidget *self = HELLO_WORLD_WIDGET (object);
  
  g_clear_pointer (&self->greeting, g_free);
  
  G_OBJECT_CLASS (hello_world_widget_parent_class)->finalize (object);
}

/* Class implementation */

static void
hello_world_widget_class_init (HelloWorldWidgetClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  
  object_class->get_property = hello_world_widget_get_property;
  object_class->set_property = hello_world_widget_set_property;
  object_class->dispose = hello_world_widget_dispose;
  object_class->finalize = hello_world_widget_finalize;
  
  /* Properties */
  properties [PROP_GREETING] =
    g_param_spec_string ("greeting",
                         "Greeting",
                         "The greeting text to display",
                         "Hello, World!",
                         G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
  
  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
hello_world_widget_init (HelloWorldWidget *self)
{
  /* Initialize state */
  self->greeting = g_strdup ("Hello, World!");
  self->editing = FALSE;
  
  /* Set up the card */
  slate_dashboard_card_set_title (SLATE_DASHBOARD_CARD (self), "Hello World Widget");
  slate_dashboard_card_set_subtitle (SLATE_DASHBOARD_CARD (self), "Example plugin widget");
  
  /* Create content */
  self->content_box = GTK_BOX (gtk_box_new (GTK_ORIENTATION_VERTICAL, 12));
  gtk_widget_set_margin_top (GTK_WIDGET (self->content_box), 12);
  gtk_widget_set_margin_bottom (GTK_WIDGET (self->content_box), 12);
  gtk_widget_set_margin_start (GTK_WIDGET (self->content_box), 12);
  gtk_widget_set_margin_end (GTK_WIDGET (self->content_box), 12);
  
  /* Greeting label */
  self->greeting_label = GTK_LABEL (gtk_label_new (self->greeting));
  gtk_label_set_wrap (self->greeting_label, TRUE);
  gtk_label_set_justify (self->greeting_label, GTK_JUSTIFY_CENTER);
  gtk_widget_add_css_class (GTK_WIDGET (self->greeting_label), "title-2");
  gtk_box_append (self->content_box, GTK_WIDGET (self->greeting_label));
  
  /* Greeting entry (initially hidden) */
  self->greeting_entry = GTK_ENTRY (gtk_entry_new ());
  gtk_entry_set_placeholder_text (self->greeting_entry, "Enter your greeting...");
  gtk_widget_set_visible (GTK_WIDGET (self->greeting_entry), FALSE);
  g_signal_connect (self->greeting_entry, "activate",
                    G_CALLBACK (on_entry_activate), self);
  gtk_box_append (self->content_box, GTK_WIDGET (self->greeting_entry));
  
  /* Change button */
  self->change_button = GTK_BUTTON (gtk_button_new_with_label ("Change"));
  gtk_widget_add_css_class (GTK_WIDGET (self->change_button), "suggested-action");
  gtk_widget_set_halign (GTK_WIDGET (self->change_button), GTK_ALIGN_CENTER);
  g_signal_connect (self->change_button, "clicked",
                    G_CALLBACK (on_change_button_clicked), self);
  gtk_box_append (self->content_box, GTK_WIDGET (self->change_button));
  
  /* Add content to the card */
  slate_dashboard_card_set_content (SLATE_DASHBOARD_CARD (self), GTK_WIDGET (self->content_box));
  
  /* Add an action button to the card */
  GtkWidget *info_button = gtk_button_new_from_icon_name ("dialog-information-symbolic");
  gtk_widget_set_tooltip_text (info_button, "About this widget");
  slate_dashboard_card_add_action (SLATE_DASHBOARD_CARD (self), info_button);
}

/* Public API */

HelloWorldWidget *
hello_world_widget_new (void)
{
  return g_object_new (HELLO_WORLD_TYPE_WIDGET, NULL);
}

void
hello_world_widget_set_greeting (HelloWorldWidget *widget, const char *greeting)
{
  g_return_if_fail (HELLO_WORLD_IS_WIDGET (widget));
  
  if (g_strcmp0 (widget->greeting, greeting) == 0)
    return;
  
  g_free (widget->greeting);
  widget->greeting = g_strdup (greeting ? greeting : "Hello, World!");
  
  /* Update the label */
  gtk_label_set_text (widget->greeting_label, widget->greeting);
  
  g_object_notify_by_pspec (G_OBJECT (widget), properties [PROP_GREETING]);
}

const char *
hello_world_widget_get_greeting (HelloWorldWidget *widget)
{
  g_return_val_if_fail (HELLO_WORLD_IS_WIDGET (widget), NULL);
  return widget->greeting;
}

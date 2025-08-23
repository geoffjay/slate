/* slate-header-bar.c
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

#include "slate-header-bar.h"
#include <glib/gi18n.h>

struct _SlateHeaderBar
{
  GtkWidget parent_instance;

  /* UI Elements */
  AdwHeaderBar *header_bar;
  AdwWindowTitle *window_title;
  GtkBox *start_box;
  GtkBox *end_box;
  GtkButton *close_project_button;
  GtkBox *project_actions_box;

  /* Properties */
  char *project_title;
  char *project_subtitle;
  gboolean show_project_actions;
};

G_DEFINE_FINAL_TYPE (SlateHeaderBar, slate_header_bar, GTK_TYPE_WIDGET)

enum {
  PROP_0,
  PROP_PROJECT_TITLE,
  PROP_PROJECT_SUBTITLE,
  PROP_SHOW_PROJECT_ACTIONS,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

/**
 * slate_header_bar_new:
 *
 * Create a new #SlateHeaderBar.
 *
 * Returns: (transfer full): a newly created #SlateHeaderBar
 */
SlateHeaderBar *
slate_header_bar_new (void)
{
  return g_object_new (SLATE_TYPE_HEADER_BAR, NULL);
}

static void
slate_header_bar_dispose (GObject *object)
{
  SlateHeaderBar *self = (SlateHeaderBar *)object;

  if (self->header_bar)
    {
      gtk_widget_unparent (GTK_WIDGET (self->header_bar));
      self->header_bar = NULL;
    }

  G_OBJECT_CLASS (slate_header_bar_parent_class)->dispose (object);
}

static void
slate_header_bar_finalize (GObject *object)
{
  SlateHeaderBar *self = (SlateHeaderBar *)object;

  g_clear_pointer (&self->project_title, g_free);
  g_clear_pointer (&self->project_subtitle, g_free);

  G_OBJECT_CLASS (slate_header_bar_parent_class)->finalize (object);
}

static void
slate_header_bar_get_property (GObject    *object,
                                guint       prop_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
  SlateHeaderBar *self = SLATE_HEADER_BAR (object);

  switch (prop_id)
    {
    case PROP_PROJECT_TITLE:
      g_value_set_string (value, self->project_title);
      break;

    case PROP_PROJECT_SUBTITLE:
      g_value_set_string (value, self->project_subtitle);
      break;

    case PROP_SHOW_PROJECT_ACTIONS:
      g_value_set_boolean (value, self->show_project_actions);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
slate_header_bar_set_property (GObject      *object,
                                guint         prop_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
  SlateHeaderBar *self = SLATE_HEADER_BAR (object);

  switch (prop_id)
    {
    case PROP_PROJECT_TITLE:
      slate_header_bar_set_project_title (self, g_value_get_string (value));
      break;

    case PROP_PROJECT_SUBTITLE:
      slate_header_bar_set_project_subtitle (self, g_value_get_string (value));
      break;

    case PROP_SHOW_PROJECT_ACTIONS:
      slate_header_bar_set_show_project_actions (self, g_value_get_boolean (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
on_close_project_clicked (GtkButton      *button G_GNUC_UNUSED,
                           SlateHeaderBar *self)
{
  g_return_if_fail (SLATE_IS_HEADER_BAR (self));

  /* Emit a signal that can be connected to by the application */
  g_signal_emit_by_name (self, "close-project-requested");
}

static void
slate_header_bar_constructed (GObject *object)
{
  SlateHeaderBar *self = SLATE_HEADER_BAR (object);

  G_OBJECT_CLASS (slate_header_bar_parent_class)->constructed (object);

  /* Create the actual header bar */
  self->header_bar = ADW_HEADER_BAR (adw_header_bar_new ());
  gtk_widget_set_parent (GTK_WIDGET (self->header_bar), GTK_WIDGET (self));

  /* Create window title */
  self->window_title = ADW_WINDOW_TITLE (adw_window_title_new ("Slate", NULL));
  adw_header_bar_set_title_widget (self->header_bar, GTK_WIDGET (self->window_title));

  /* Create start box for plugin widgets */
  self->start_box = GTK_BOX (gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6));
  gtk_widget_set_halign (GTK_WIDGET (self->start_box), GTK_ALIGN_START);
  adw_header_bar_pack_start (self->header_bar, GTK_WIDGET (self->start_box));

  /* Create end box for plugin widgets and project actions */
  self->end_box = GTK_BOX (gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6));
  gtk_widget_set_halign (GTK_WIDGET (self->end_box), GTK_ALIGN_END);

  /* Create project actions box */
  self->project_actions_box = GTK_BOX (gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6));

  /* Create close project button */
  self->close_project_button = GTK_BUTTON (gtk_button_new_from_icon_name ("window-close-symbolic"));
  gtk_widget_set_tooltip_text (GTK_WIDGET (self->close_project_button), _("Close Project"));
  gtk_button_set_has_frame (self->close_project_button, FALSE);
  g_signal_connect (self->close_project_button, "clicked",
                    G_CALLBACK (on_close_project_clicked), self);

  /* Add close button to project actions */
  gtk_box_append (self->project_actions_box, GTK_WIDGET (self->close_project_button));

  /* Add project actions to end box */
  gtk_box_append (self->end_box, GTK_WIDGET (self->project_actions_box));

  /* Pack end box */
  adw_header_bar_pack_end (self->header_bar, GTK_WIDGET (self->end_box));

  /* Initially hide project actions */
  slate_header_bar_set_show_project_actions (self, FALSE);
}

static void
slate_header_bar_class_init (SlateHeaderBarClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = slate_header_bar_dispose;
  object_class->finalize = slate_header_bar_finalize;
  object_class->get_property = slate_header_bar_get_property;
  object_class->set_property = slate_header_bar_set_property;
  object_class->constructed = slate_header_bar_constructed;

  /**
   * SlateHeaderBar:project-title:
   *
   * The title of the current project.
   */
  properties [PROP_PROJECT_TITLE] =
    g_param_spec_string ("project-title", NULL, NULL,
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  /**
   * SlateHeaderBar:project-subtitle:
   *
   * The subtitle of the current project.
   */
  properties [PROP_PROJECT_SUBTITLE] =
    g_param_spec_string ("project-subtitle", NULL, NULL,
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  /**
   * SlateHeaderBar:show-project-actions:
   *
   * Whether project-specific actions should be visible.
   */
  properties [PROP_SHOW_PROJECT_ACTIONS] =
    g_param_spec_boolean ("show-project-actions", NULL, NULL,
                          FALSE,
                          (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);

  /**
   * SlateHeaderBar::close-project-requested:
   * @self: the #SlateHeaderBar
   *
   * Emitted when the user requests to close the current project.
   */
  g_signal_new ("close-project-requested",
                G_TYPE_FROM_CLASS (klass),
                G_SIGNAL_RUN_LAST,
                0,
                NULL, NULL,
                NULL,
                G_TYPE_NONE, 0);
}

static void
slate_header_bar_init (SlateHeaderBar *self)
{
  self->show_project_actions = FALSE;
}

/**
 * slate_header_bar_set_project_title:
 * @self: a #SlateHeaderBar
 * @title: (nullable): the project title
 *
 * Sets the project title displayed in the header bar.
 */
void
slate_header_bar_set_project_title (SlateHeaderBar *self,
                                     const char     *title)
{
  g_return_if_fail (SLATE_IS_HEADER_BAR (self));

  if (g_strcmp0 (self->project_title, title) == 0)
    return;

  g_clear_pointer (&self->project_title, g_free);
  self->project_title = g_strdup (title);

  if (self->window_title)
    adw_window_title_set_title (self->window_title, title ? title : "Slate");

  g_object_notify_by_pspec (G_OBJECT (self), properties [PROP_PROJECT_TITLE]);
}

/**
 * slate_header_bar_get_project_title:
 * @self: a #SlateHeaderBar
 *
 * Gets the current project title.
 *
 * Returns: (nullable): the project title
 */
const char *
slate_header_bar_get_project_title (SlateHeaderBar *self)
{
  g_return_val_if_fail (SLATE_IS_HEADER_BAR (self), NULL);

  return self->project_title;
}

/**
 * slate_header_bar_set_project_subtitle:
 * @self: a #SlateHeaderBar
 * @subtitle: (nullable): the project subtitle
 *
 * Sets the project subtitle displayed in the header bar.
 */
void
slate_header_bar_set_project_subtitle (SlateHeaderBar *self,
                                        const char     *subtitle)
{
  g_return_if_fail (SLATE_IS_HEADER_BAR (self));

  if (g_strcmp0 (self->project_subtitle, subtitle) == 0)
    return;

  g_clear_pointer (&self->project_subtitle, g_free);
  self->project_subtitle = g_strdup (subtitle);

  if (self->window_title)
    adw_window_title_set_subtitle (self->window_title, subtitle);

  g_object_notify_by_pspec (G_OBJECT (self), properties [PROP_PROJECT_SUBTITLE]);
}

/**
 * slate_header_bar_get_project_subtitle:
 * @self: a #SlateHeaderBar
 *
 * Gets the current project subtitle.
 *
 * Returns: (nullable): the project subtitle
 */
const char *
slate_header_bar_get_project_subtitle (SlateHeaderBar *self)
{
  g_return_val_if_fail (SLATE_IS_HEADER_BAR (self), NULL);

  return self->project_subtitle;
}

/**
 * slate_header_bar_add_start_widget:
 * @self: a #SlateHeaderBar
 * @widget: a #GtkWidget
 *
 * Adds a widget to the start of the header bar.
 * This is typically used by plugins to add custom buttons or controls.
 */
void
slate_header_bar_add_start_widget (SlateHeaderBar *self,
                                    GtkWidget      *widget)
{
  g_return_if_fail (SLATE_IS_HEADER_BAR (self));
  g_return_if_fail (GTK_IS_WIDGET (widget));

  gtk_box_append (self->start_box, widget);
}

/**
 * slate_header_bar_add_end_widget:
 * @self: a #SlateHeaderBar
 * @widget: a #GtkWidget
 *
 * Adds a widget to the end of the header bar.
 * This is typically used by plugins to add custom buttons or controls.
 */
void
slate_header_bar_add_end_widget (SlateHeaderBar *self,
                                  GtkWidget      *widget)
{
  g_return_if_fail (SLATE_IS_HEADER_BAR (self));
  g_return_if_fail (GTK_IS_WIDGET (widget));

  /* Insert before project actions box */
  gtk_box_insert_child_after (self->end_box, widget, NULL);
}

/**
 * slate_header_bar_remove_widget:
 * @self: a #SlateHeaderBar
 * @widget: a #GtkWidget
 *
 * Removes a widget from the header bar.
 */
void
slate_header_bar_remove_widget (SlateHeaderBar *self,
                                 GtkWidget      *widget)
{
  g_return_if_fail (SLATE_IS_HEADER_BAR (self));
  g_return_if_fail (GTK_IS_WIDGET (widget));

  GtkWidget *parent = gtk_widget_get_parent (widget);
  if (parent == GTK_WIDGET (self->start_box) || parent == GTK_WIDGET (self->end_box))
    gtk_box_remove (GTK_BOX (parent), widget);
}

/**
 * slate_header_bar_set_show_project_actions:
 * @self: a #SlateHeaderBar
 * @show: whether to show project actions
 *
 * Sets whether project-specific actions should be visible.
 */
void
slate_header_bar_set_show_project_actions (SlateHeaderBar *self,
                                            gboolean        show)
{
  g_return_if_fail (SLATE_IS_HEADER_BAR (self));

  if (self->show_project_actions == show)
    return;

  self->show_project_actions = show;

  if (self->project_actions_box)
    gtk_widget_set_visible (GTK_WIDGET (self->project_actions_box), show);

  g_object_notify_by_pspec (G_OBJECT (self), properties [PROP_SHOW_PROJECT_ACTIONS]);
}

/**
 * slate_header_bar_get_show_project_actions:
 * @self: a #SlateHeaderBar
 *
 * Gets whether project actions are currently visible.
 *
 * Returns: %TRUE if project actions are visible
 */
gboolean
slate_header_bar_get_show_project_actions (SlateHeaderBar *self)
{
  g_return_val_if_fail (SLATE_IS_HEADER_BAR (self), FALSE);

  return self->show_project_actions;
}

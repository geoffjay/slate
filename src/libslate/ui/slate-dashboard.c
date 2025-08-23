/* slate-dashboard.c
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

#include "slate-dashboard.h"
#include <glib/gi18n.h>

/**
 * SECTION:slate-dashboard
 * @short_description: Dashboard container widget
 * @title: SlateDashboard
 *
 * #SlateDashboard is a container widget that organizes and displays
 * dashboard components in a flexible layout.
 */

typedef enum {
  SLATE_DASHBOARD_LAYOUT_GRID,
  SLATE_DASHBOARD_LAYOUT_FLOW,
  SLATE_DASHBOARD_LAYOUT_STACK
} SlateDashboardLayout;

struct _SlateDashboard
{
  AdwBin parent_instance;

  /* Layout management */
  GtkWidget *main_container;
  GtkWidget *scrolled_window;
  GtkWidget *content_box;
  
  /* Widget tracking */
  GHashTable *widgets;
  
  /* Layout properties */
  SlateDashboardLayout layout_type;
  int columns;
  
  /* State */
  gboolean needs_refresh;
};

G_DEFINE_FINAL_TYPE (SlateDashboard, slate_dashboard, ADW_TYPE_BIN)

enum {
  PROP_0,
  PROP_LAYOUT,
  PROP_COLUMNS,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

enum {
  WIDGET_ADDED,
  WIDGET_REMOVED,
  LAYOUT_CHANGED,
  N_SIGNALS
};

static guint signals [N_SIGNALS];

static void
slate_dashboard_dispose (GObject *object)
{
  SlateDashboard *self = (SlateDashboard *)object;

  g_clear_pointer (&self->widgets, g_hash_table_unref);

  G_OBJECT_CLASS (slate_dashboard_parent_class)->dispose (object);
}

static void
slate_dashboard_get_property (GObject    *object,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  SlateDashboard *self = SLATE_DASHBOARD (object);

  switch (prop_id)
    {
    case PROP_LAYOUT:
      g_value_set_string (value, slate_dashboard_get_layout (self));
      break;
    case PROP_COLUMNS:
      g_value_set_int (value, slate_dashboard_get_columns (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
slate_dashboard_set_property (GObject      *object,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  SlateDashboard *self = SLATE_DASHBOARD (object);

  switch (prop_id)
    {
    case PROP_LAYOUT:
      slate_dashboard_set_layout (self, g_value_get_string (value));
      break;
    case PROP_COLUMNS:
      slate_dashboard_set_columns (self, g_value_get_int (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
slate_dashboard_class_init (SlateDashboardClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = slate_dashboard_dispose;
  object_class->get_property = slate_dashboard_get_property;
  object_class->set_property = slate_dashboard_set_property;

  /**
   * SlateDashboard:layout:
   *
   * The layout type for organizing dashboard widgets.
   */
  properties [PROP_LAYOUT] =
    g_param_spec_string ("layout", NULL, NULL,
                         "grid",
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));

  /**
   * SlateDashboard:columns:
   *
   * The number of columns for grid layout.
   */
  properties [PROP_COLUMNS] =
    g_param_spec_int ("columns", NULL, NULL,
                      1, 12, 3,
                      (G_PARAM_READWRITE |
                       G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);

  /**
   * SlateDashboard::widget-added:
   * @dashboard: the dashboard
   * @widget: the widget that was added
   * @widget_id: the ID of the added widget
   *
   * Emitted when a widget is added to the dashboard.
   */
  signals [WIDGET_ADDED] =
    g_signal_new ("widget-added",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL, NULL,
                  NULL,
                  G_TYPE_NONE, 2,
                  GTK_TYPE_WIDGET,
                  G_TYPE_STRING);

  /**
   * SlateDashboard::widget-removed:
   * @dashboard: the dashboard
   * @widget_id: the ID of the removed widget
   *
   * Emitted when a widget is removed from the dashboard.
   */
  signals [WIDGET_REMOVED] =
    g_signal_new ("widget-removed",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL, NULL,
                  NULL,
                  G_TYPE_NONE, 1,
                  G_TYPE_STRING);

  /**
   * SlateDashboard::layout-changed:
   * @dashboard: the dashboard
   * @layout: the new layout type
   *
   * Emitted when the dashboard layout changes.
   */
  signals [LAYOUT_CHANGED] =
    g_signal_new ("layout-changed",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL, NULL,
                  NULL,
                  G_TYPE_NONE, 1,
                  G_TYPE_STRING);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/Slate/ui/slate-dashboard.ui");
  gtk_widget_class_set_css_name (widget_class, "slate-dashboard");
}

static void
slate_dashboard_rebuild_layout (SlateDashboard *self)
{
  GtkWidget *new_container = NULL;
  
  /* Remove existing content container */
  if (self->content_box)
    {
      gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (self->scrolled_window), NULL);
      self->content_box = NULL;
    }

  /* Create new container based on layout type */
  switch (self->layout_type)
    {
    case SLATE_DASHBOARD_LAYOUT_GRID:
      new_container = gtk_grid_new ();
      gtk_grid_set_column_homogeneous (GTK_GRID (new_container), TRUE);
      gtk_grid_set_row_homogeneous (GTK_GRID (new_container), FALSE);
      gtk_grid_set_column_spacing (GTK_GRID (new_container), 12);
      gtk_grid_set_row_spacing (GTK_GRID (new_container), 12);
      break;

    case SLATE_DASHBOARD_LAYOUT_FLOW:
      new_container = gtk_flow_box_new ();
      gtk_flow_box_set_homogeneous (GTK_FLOW_BOX (new_container), FALSE);
      gtk_flow_box_set_column_spacing (GTK_FLOW_BOX (new_container), 12);
      gtk_flow_box_set_row_spacing (GTK_FLOW_BOX (new_container), 12);
      gtk_flow_box_set_max_children_per_line (GTK_FLOW_BOX (new_container), self->columns);
      break;

    case SLATE_DASHBOARD_LAYOUT_STACK:
    default:
      new_container = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
      break;
    }

  self->content_box = new_container;
  gtk_widget_set_margin_top (self->content_box, 12);
  gtk_widget_set_margin_bottom (self->content_box, 12);
  gtk_widget_set_margin_start (self->content_box, 12);
  gtk_widget_set_margin_end (self->content_box, 12);

  gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (self->scrolled_window), self->content_box);

  /* Re-add all widgets to the new container */
  slate_dashboard_refresh (self);
}

static void
slate_dashboard_init (SlateDashboard *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  /* Initialize properties */
  self->layout_type = SLATE_DASHBOARD_LAYOUT_GRID;
  self->columns = 3;
  self->needs_refresh = FALSE;

  /* Initialize widget tracking */
  self->widgets = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_object_unref);

  /* Create main container */
  self->scrolled_window = gtk_scrolled_window_new ();
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (self->scrolled_window),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_propagate_natural_width (GTK_SCROLLED_WINDOW (self->scrolled_window), TRUE);
  gtk_scrolled_window_set_propagate_natural_height (GTK_SCROLLED_WINDOW (self->scrolled_window), TRUE);

  adw_bin_set_child (ADW_BIN (self), self->scrolled_window);

  /* Build initial layout */
  slate_dashboard_rebuild_layout (self);
}

/**
 * slate_dashboard_new:
 *
 * Creates a new #SlateDashboard.
 *
 * Returns: (transfer full): a newly created #SlateDashboard
 */
SlateDashboard *
slate_dashboard_new (void)
{
  return g_object_new (SLATE_TYPE_DASHBOARD, NULL);
}

/**
 * slate_dashboard_add_widget:
 * @dashboard: a #SlateDashboard
 * @widget: the #GtkWidget to add
 * @widget_id: unique identifier for the widget
 *
 * Adds a widget to the dashboard with the specified ID.
 */
void
slate_dashboard_add_widget (SlateDashboard *dashboard,
                            GtkWidget      *widget,
                            const char     *widget_id)
{
  g_return_if_fail (SLATE_IS_DASHBOARD (dashboard));
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (widget_id != NULL);

  /* Remove existing widget with same ID */
  if (g_hash_table_contains (dashboard->widgets, widget_id))
    slate_dashboard_remove_widget (dashboard, widget_id);

  /* Store widget reference */
  g_hash_table_insert (dashboard->widgets, g_strdup (widget_id), g_object_ref (widget));

  /* Add to layout container */
  if (dashboard->layout_type == SLATE_DASHBOARD_LAYOUT_FLOW)
    {
      gtk_flow_box_append (GTK_FLOW_BOX (dashboard->content_box), widget);
    }
  else if (dashboard->layout_type == SLATE_DASHBOARD_LAYOUT_GRID)
    {
      int position = g_hash_table_size (dashboard->widgets) - 1;
      int row = position / dashboard->columns;
      int col = position % dashboard->columns;
      gtk_grid_attach (GTK_GRID (dashboard->content_box), widget, col, row, 1, 1);
    }
  else
    {
      gtk_box_append (GTK_BOX (dashboard->content_box), widget);
    }

  g_signal_emit (dashboard, signals[WIDGET_ADDED], 0, widget, widget_id);
}

/**
 * slate_dashboard_remove_widget:
 * @dashboard: a #SlateDashboard
 * @widget_id: the ID of the widget to remove
 *
 * Removes a widget from the dashboard.
 */
void
slate_dashboard_remove_widget (SlateDashboard *dashboard,
                               const char     *widget_id)
{
  GtkWidget *widget;

  g_return_if_fail (SLATE_IS_DASHBOARD (dashboard));
  g_return_if_fail (widget_id != NULL);

  widget = g_hash_table_lookup (dashboard->widgets, widget_id);
  if (widget != NULL)
    {
      gtk_widget_unparent (widget);
      g_hash_table_remove (dashboard->widgets, widget_id);
      g_signal_emit (dashboard, signals[WIDGET_REMOVED], 0, widget_id);
    }
}

/**
 * slate_dashboard_get_widget:
 * @dashboard: a #SlateDashboard
 * @widget_id: the ID of the widget to retrieve
 *
 * Gets a widget from the dashboard by ID.
 *
 * Returns: (transfer none) (nullable): the widget or %NULL if not found
 */
GtkWidget *
slate_dashboard_get_widget (SlateDashboard *dashboard,
                            const char     *widget_id)
{
  g_return_val_if_fail (SLATE_IS_DASHBOARD (dashboard), NULL);
  g_return_val_if_fail (widget_id != NULL, NULL);

  return g_hash_table_lookup (dashboard->widgets, widget_id);
}

/**
 * slate_dashboard_set_layout:
 * @dashboard: a #SlateDashboard
 * @layout_type: the layout type ("grid", "flow", or "stack")
 *
 * Sets the layout type for the dashboard.
 */
void
slate_dashboard_set_layout (SlateDashboard *dashboard,
                            const char     *layout_type)
{
  SlateDashboardLayout new_layout;

  g_return_if_fail (SLATE_IS_DASHBOARD (dashboard));
  g_return_if_fail (layout_type != NULL);

  if (g_strcmp0 (layout_type, "flow") == 0)
    new_layout = SLATE_DASHBOARD_LAYOUT_FLOW;
  else if (g_strcmp0 (layout_type, "stack") == 0)
    new_layout = SLATE_DASHBOARD_LAYOUT_STACK;
  else
    new_layout = SLATE_DASHBOARD_LAYOUT_GRID;

  if (dashboard->layout_type != new_layout)
    {
      dashboard->layout_type = new_layout;
      slate_dashboard_rebuild_layout (dashboard);
      g_object_notify_by_pspec (G_OBJECT (dashboard), properties[PROP_LAYOUT]);
      g_signal_emit (dashboard, signals[LAYOUT_CHANGED], 0, layout_type);
    }
}

/**
 * slate_dashboard_get_layout:
 * @dashboard: a #SlateDashboard
 *
 * Gets the current layout type.
 *
 * Returns: the layout type string
 */
const char *
slate_dashboard_get_layout (SlateDashboard *dashboard)
{
  g_return_val_if_fail (SLATE_IS_DASHBOARD (dashboard), "grid");

  switch (dashboard->layout_type)
    {
    case SLATE_DASHBOARD_LAYOUT_FLOW:
      return "flow";
    case SLATE_DASHBOARD_LAYOUT_STACK:
      return "stack";
    case SLATE_DASHBOARD_LAYOUT_GRID:
    default:
      return "grid";
    }
}

/**
 * slate_dashboard_set_columns:
 * @dashboard: a #SlateDashboard
 * @columns: number of columns (1-12)
 *
 * Sets the number of columns for grid and flow layouts.
 */
void
slate_dashboard_set_columns (SlateDashboard *dashboard,
                             int             columns)
{
  g_return_if_fail (SLATE_IS_DASHBOARD (dashboard));
  g_return_if_fail (columns >= 1 && columns <= 12);

  if (dashboard->columns != columns)
    {
      dashboard->columns = columns;
      
      if (dashboard->layout_type == SLATE_DASHBOARD_LAYOUT_FLOW && dashboard->content_box)
        {
          gtk_flow_box_set_max_children_per_line (GTK_FLOW_BOX (dashboard->content_box), columns);
        }
      else if (dashboard->layout_type == SLATE_DASHBOARD_LAYOUT_GRID)
        {
          slate_dashboard_rebuild_layout (dashboard);
        }
      
      g_object_notify_by_pspec (G_OBJECT (dashboard), properties[PROP_COLUMNS]);
    }
}

/**
 * slate_dashboard_get_columns:
 * @dashboard: a #SlateDashboard
 *
 * Gets the number of columns.
 *
 * Returns: the number of columns
 */
int
slate_dashboard_get_columns (SlateDashboard *dashboard)
{
  g_return_val_if_fail (SLATE_IS_DASHBOARD (dashboard), 3);

  return dashboard->columns;
}

/**
 * slate_dashboard_clear:
 * @dashboard: a #SlateDashboard
 *
 * Removes all widgets from the dashboard.
 */
void
slate_dashboard_clear (SlateDashboard *dashboard)
{
  g_return_if_fail (SLATE_IS_DASHBOARD (dashboard));

  g_hash_table_remove_all (dashboard->widgets);
  
  if (dashboard->content_box)
    {
      GtkWidget *child = gtk_widget_get_first_child (dashboard->content_box);
      while (child != NULL)
        {
          GtkWidget *next = gtk_widget_get_next_sibling (child);
          gtk_widget_unparent (child);
          child = next;
        }
    }
}

/**
 * slate_dashboard_refresh:
 * @dashboard: a #SlateDashboard
 *
 * Refreshes the dashboard layout and all widgets.
 */
void
slate_dashboard_refresh (SlateDashboard *dashboard)
{
  GHashTableIter iter;
  gpointer key, value;
  int position = 0;

  g_return_if_fail (SLATE_IS_DASHBOARD (dashboard));

  if (!dashboard->content_box)
    return;

  /* Clear current layout */
  GtkWidget *child = gtk_widget_get_first_child (dashboard->content_box);
  while (child != NULL)
    {
      GtkWidget *next = gtk_widget_get_next_sibling (child);
      gtk_widget_unparent (child);
      child = next;
    }

  /* Re-add all widgets */
  g_hash_table_iter_init (&iter, dashboard->widgets);
  while (g_hash_table_iter_next (&iter, &key, &value))
    {
      GtkWidget *widget = GTK_WIDGET (value);
      
      if (dashboard->layout_type == SLATE_DASHBOARD_LAYOUT_FLOW)
        {
          gtk_flow_box_append (GTK_FLOW_BOX (dashboard->content_box), widget);
        }
      else if (dashboard->layout_type == SLATE_DASHBOARD_LAYOUT_GRID)
        {
          int row = position / dashboard->columns;
          int col = position % dashboard->columns;
          gtk_grid_attach (GTK_GRID (dashboard->content_box), widget, col, row, 1, 1);
        }
      else
        {
          gtk_box_append (GTK_BOX (dashboard->content_box), widget);
        }
      
      position++;
    }

  dashboard->needs_refresh = FALSE;
}

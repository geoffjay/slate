/* slate-chart.c
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

#include "slate-chart.h"
#include <math.h>
#include <glib/gi18n.h>

/**
 * SECTION:slate-chart
 * @short_description: Chart widget for data visualization
 * @title: SlateChart
 *
 * #SlateChart is a widget for displaying various types of charts and graphs.
 */

struct _SlateChart
{
  GtkWidget parent_instance;

  /* Chart properties */
  char *title;
  SlateChartType chart_type;
  
  /* Data */
  GArray *data_points;
  
  /* Ranges */
  double x_min, x_max;
  double y_min, y_max;
  gboolean auto_range;
  
  /* Display options */
  gboolean show_grid;
  gboolean show_legend;
  
  /* Colors */
  GdkRGBA primary_color;
  GdkRGBA grid_color;
  GdkRGBA text_color;
};

G_DEFINE_FINAL_TYPE (SlateChart, slate_chart, GTK_TYPE_WIDGET)

enum {
  PROP_0,
  PROP_TITLE,
  PROP_CHART_TYPE,
  PROP_SHOW_GRID,
  PROP_SHOW_LEGEND,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

static void
slate_chart_data_point_clear (SlateChartDataPoint *point)
{
  g_free (point->label);
}

static void
slate_chart_dispose (GObject *object)
{
  SlateChart *self = (SlateChart *)object;

  g_clear_pointer (&self->title, g_free);
  g_clear_pointer (&self->data_points, g_array_unref);

  G_OBJECT_CLASS (slate_chart_parent_class)->dispose (object);
}

static void
slate_chart_get_property (GObject    *object,
                          guint       prop_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
  SlateChart *self = SLATE_CHART (object);

  switch (prop_id)
    {
    case PROP_TITLE:
      g_value_set_string (value, slate_chart_get_title (self));
      break;
    case PROP_CHART_TYPE:
      g_value_set_enum (value, slate_chart_get_chart_type (self));
      break;
    case PROP_SHOW_GRID:
      g_value_set_boolean (value, slate_chart_get_show_grid (self));
      break;
    case PROP_SHOW_LEGEND:
      g_value_set_boolean (value, slate_chart_get_show_legend (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
slate_chart_set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
  SlateChart *self = SLATE_CHART (object);

  switch (prop_id)
    {
    case PROP_TITLE:
      slate_chart_set_title (self, g_value_get_string (value));
      break;
    case PROP_CHART_TYPE:
      slate_chart_set_chart_type (self, g_value_get_enum (value));
      break;
    case PROP_SHOW_GRID:
      slate_chart_set_show_grid (self, g_value_get_boolean (value));
      break;
    case PROP_SHOW_LEGEND:
      slate_chart_set_show_legend (self, g_value_get_boolean (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
slate_chart_update_auto_range (SlateChart *self)
{
  if (!self->auto_range || self->data_points->len == 0)
    return;

  self->x_min = G_MAXDOUBLE;
  self->x_max = -G_MAXDOUBLE;
  self->y_min = G_MAXDOUBLE;
  self->y_max = -G_MAXDOUBLE;

  for (guint i = 0; i < self->data_points->len; i++)
    {
      SlateChartDataPoint *point = &g_array_index (self->data_points, SlateChartDataPoint, i);
      
      if (point->x < self->x_min) self->x_min = point->x;
      if (point->x > self->x_max) self->x_max = point->x;
      if (point->y < self->y_min) self->y_min = point->y;
      if (point->y > self->y_max) self->y_max = point->y;
    }

  /* Add some padding */
  double x_range = self->x_max - self->x_min;
  double y_range = self->y_max - self->y_min;
  
  if (x_range > 0)
    {
      self->x_min -= x_range * 0.1;
      self->x_max += x_range * 0.1;
    }
  
  if (y_range > 0)
    {
      self->y_min -= y_range * 0.1;
      self->y_max += y_range * 0.1;
    }
}

static void
slate_chart_draw_grid (SlateChart *self, cairo_t *cr, int width, int height)
{
  if (!self->show_grid)
    return;

  cairo_save (cr);
  
  gdk_cairo_set_source_rgba (cr, &self->grid_color);
  cairo_set_line_width (cr, 1.0);

  /* Draw vertical grid lines */
  for (int i = 1; i < 10; i++)
    {
      double x = (width * i) / 10.0;
      cairo_move_to (cr, x, 0);
      cairo_line_to (cr, x, height);
    }

  /* Draw horizontal grid lines */
  for (int i = 1; i < 10; i++)
    {
      double y = (height * i) / 10.0;
      cairo_move_to (cr, 0, y);
      cairo_line_to (cr, width, y);
    }

  cairo_stroke (cr);
  cairo_restore (cr);
}

static void
slate_chart_draw_line_chart (SlateChart *self, cairo_t *cr, int width, int height)
{
  if (self->data_points->len < 2)
    return;

  cairo_save (cr);
  
  gdk_cairo_set_source_rgba (cr, &self->primary_color);
  cairo_set_line_width (cr, 2.0);

  double x_range = self->x_max - self->x_min;
  double y_range = self->y_max - self->y_min;

  if (x_range <= 0 || y_range <= 0)
    {
      cairo_restore (cr);
      return;
    }

  gboolean first_point = TRUE;
  
  for (guint i = 0; i < self->data_points->len; i++)
    {
      SlateChartDataPoint *point = &g_array_index (self->data_points, SlateChartDataPoint, i);
      
      double x = ((point->x - self->x_min) / x_range) * width;
      double y = height - ((point->y - self->y_min) / y_range) * height;
      
      if (first_point)
        {
          cairo_move_to (cr, x, y);
          first_point = FALSE;
        }
      else
        {
          cairo_line_to (cr, x, y);
        }
    }

  cairo_stroke (cr);
  cairo_restore (cr);
}

static void
slate_chart_draw_bar_chart (SlateChart *self, cairo_t *cr, int width, int height)
{
  if (self->data_points->len == 0)
    return;

  cairo_save (cr);
  
  gdk_cairo_set_source_rgba (cr, &self->primary_color);

  double bar_width = (double)width / self->data_points->len * 0.8;
  double bar_spacing = (double)width / self->data_points->len * 0.2;
  double y_range = self->y_max - self->y_min;

  if (y_range <= 0)
    {
      cairo_restore (cr);
      return;
    }

  for (guint i = 0; i < self->data_points->len; i++)
    {
      SlateChartDataPoint *point = &g_array_index (self->data_points, SlateChartDataPoint, i);
      
      double x = i * (bar_width + bar_spacing) + bar_spacing / 2;
      double bar_height = ((point->y - self->y_min) / y_range) * height;
      double y = height - bar_height;
      
      cairo_rectangle (cr, x, y, bar_width, bar_height);
      cairo_fill (cr);
    }

  cairo_restore (cr);
}

static void
slate_chart_draw_title (SlateChart *self, cairo_t *cr, int width, int height)
{
  if (!self->title || strlen (self->title) == 0)
    return;

  cairo_save (cr);
  
  gdk_cairo_set_source_rgba (cr, &self->text_color);
  cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size (cr, 16.0);

  cairo_text_extents_t extents;
  cairo_text_extents (cr, self->title, &extents);
  
  double x = (width - extents.width) / 2;
  double y = 20;
  
  cairo_move_to (cr, x, y);
  cairo_show_text (cr, self->title);
  
  cairo_restore (cr);
}

static void
slate_chart_snapshot (GtkWidget   *widget,
                      GtkSnapshot *snapshot)
{
  SlateChart *self = SLATE_CHART (widget);
  int width = gtk_widget_get_width (widget);
  int height = gtk_widget_get_height (widget);
  cairo_t *cr;

  if (width <= 0 || height <= 0)
    return;

  cr = gtk_snapshot_append_cairo (snapshot, &GRAPHENE_RECT_INIT (0, 0, width, height));

  /* Update auto range if needed */
  slate_chart_update_auto_range (self);

  /* Draw background */
  cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
  cairo_paint (cr);

  /* Draw grid */
  slate_chart_draw_grid (self, cr, width, height);

  /* Draw chart content based on type */
  switch (self->chart_type)
    {
    case SLATE_CHART_TYPE_LINE:
      slate_chart_draw_line_chart (self, cr, width, height);
      break;
    case SLATE_CHART_TYPE_BAR:
      slate_chart_draw_bar_chart (self, cr, width, height);
      break;
    case SLATE_CHART_TYPE_PIE:
    case SLATE_CHART_TYPE_SCATTER:
      /* TODO: Implement pie and scatter charts */
      break;
    }

  /* Draw title */
  slate_chart_draw_title (self, cr, width, height);

  /* Additional drawing can be added here in the future */

  cairo_destroy (cr);
}

static void
slate_chart_class_init (SlateChartClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = slate_chart_dispose;
  object_class->get_property = slate_chart_get_property;
  object_class->set_property = slate_chart_set_property;

  widget_class->snapshot = slate_chart_snapshot;

  /**
   * SlateChart:title:
   *
   * The title of the chart.
   */
  properties [PROP_TITLE] =
    g_param_spec_string ("title", NULL, NULL,
                         NULL,
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));

  /**
   * SlateChart:chart-type:
   *
   * The type of chart to display.
   */
  properties [PROP_CHART_TYPE] =
    g_param_spec_int ("chart-type", NULL, NULL,
                      SLATE_CHART_TYPE_LINE,
                      SLATE_CHART_TYPE_SCATTER,
                      SLATE_CHART_TYPE_LINE,
                      (G_PARAM_READWRITE |
                       G_PARAM_STATIC_STRINGS));

  /**
   * SlateChart:show-grid:
   *
   * Whether to show the grid.
   */
  properties [PROP_SHOW_GRID] =
    g_param_spec_boolean ("show-grid", NULL, NULL,
                          TRUE,
                          (G_PARAM_READWRITE |
                           G_PARAM_STATIC_STRINGS));

  /**
   * SlateChart:show-legend:
   *
   * Whether to show the legend.
   */
  properties [PROP_SHOW_LEGEND] =
    g_param_spec_boolean ("show-legend", NULL, NULL,
                          FALSE,
                          (G_PARAM_READWRITE |
                           G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);

  gtk_widget_class_set_css_name (widget_class, "slate-chart");
}

static void
slate_chart_init (SlateChart *self)
{
  /* Initialize properties */
  self->chart_type = SLATE_CHART_TYPE_LINE;
  self->auto_range = TRUE;
  self->show_grid = TRUE;
  self->show_legend = FALSE;
  
  /* Initialize data */
  self->data_points = g_array_new (FALSE, FALSE, sizeof (SlateChartDataPoint));
  g_array_set_clear_func (self->data_points, (GDestroyNotify) slate_chart_data_point_clear);
  
  /* Initialize ranges */
  self->x_min = 0.0;
  self->x_max = 100.0;
  self->y_min = 0.0;
  self->y_max = 100.0;
  
  /* Initialize colors */
  gdk_rgba_parse (&self->primary_color, "#3584e4");
  gdk_rgba_parse (&self->grid_color, "#d5d5d5");
  gdk_rgba_parse (&self->text_color, "#2e3436");
  
  /* Set minimum size */
  gtk_widget_set_size_request (GTK_WIDGET (self), 200, 150);
}

/**
 * slate_chart_new:
 * @chart_type: the type of chart to create
 *
 * Creates a new #SlateChart.
 *
 * Returns: (transfer full): a newly created #SlateChart
 */
SlateChart *
slate_chart_new (SlateChartType chart_type)
{
  return g_object_new (SLATE_TYPE_CHART,
                       "chart-type", chart_type,
                       NULL);
}

/**
 * slate_chart_set_title:
 * @chart: a #SlateChart
 * @title: the chart title
 *
 * Sets the title of the chart.
 */
void
slate_chart_set_title (SlateChart *chart,
                       const char *title)
{
  g_return_if_fail (SLATE_IS_CHART (chart));

  if (g_strcmp0 (chart->title, title) != 0)
    {
      g_free (chart->title);
      chart->title = g_strdup (title);
      gtk_widget_queue_draw (GTK_WIDGET (chart));
      g_object_notify_by_pspec (G_OBJECT (chart), properties[PROP_TITLE]);
    }
}

/**
 * slate_chart_get_title:
 * @chart: a #SlateChart
 *
 * Gets the title of the chart.
 *
 * Returns: (nullable): the chart title
 */
const char *
slate_chart_get_title (SlateChart *chart)
{
  g_return_val_if_fail (SLATE_IS_CHART (chart), NULL);

  return chart->title;
}

/**
 * slate_chart_set_chart_type:
 * @chart: a #SlateChart
 * @chart_type: the chart type
 *
 * Sets the type of chart to display.
 */
void
slate_chart_set_chart_type (SlateChart     *chart,
                            SlateChartType  chart_type)
{
  g_return_if_fail (SLATE_IS_CHART (chart));

  if (chart->chart_type != chart_type)
    {
      chart->chart_type = chart_type;
      gtk_widget_queue_draw (GTK_WIDGET (chart));
      g_object_notify_by_pspec (G_OBJECT (chart), properties[PROP_CHART_TYPE]);
    }
}

/**
 * slate_chart_get_chart_type:
 * @chart: a #SlateChart
 *
 * Gets the type of chart.
 *
 * Returns: the chart type
 */
SlateChartType
slate_chart_get_chart_type (SlateChart *chart)
{
  g_return_val_if_fail (SLATE_IS_CHART (chart), SLATE_CHART_TYPE_LINE);

  return chart->chart_type;
}

/**
 * slate_chart_add_data_point:
 * @chart: a #SlateChart
 * @x: X coordinate
 * @y: Y coordinate
 * @label: (nullable): optional label
 *
 * Adds a data point to the chart.
 */
void
slate_chart_add_data_point (SlateChart *chart,
                            double      x,
                            double      y,
                            const char *label)
{
  SlateChartDataPoint point;

  g_return_if_fail (SLATE_IS_CHART (chart));

  point.x = x;
  point.y = y;
  point.label = g_strdup (label);

  g_array_append_val (chart->data_points, point);
  gtk_widget_queue_draw (GTK_WIDGET (chart));
}

/**
 * slate_chart_clear_data:
 * @chart: a #SlateChart
 *
 * Clears all data points from the chart.
 */
void
slate_chart_clear_data (SlateChart *chart)
{
  g_return_if_fail (SLATE_IS_CHART (chart));

  g_array_remove_range (chart->data_points, 0, chart->data_points->len);
  gtk_widget_queue_draw (GTK_WIDGET (chart));
}

/**
 * slate_chart_set_show_grid:
 * @chart: a #SlateChart
 * @show_grid: whether to show the grid
 *
 * Sets whether to show the grid.
 */
void
slate_chart_set_show_grid (SlateChart *chart,
                           gboolean    show_grid)
{
  g_return_if_fail (SLATE_IS_CHART (chart));

  if (chart->show_grid != show_grid)
    {
      chart->show_grid = show_grid;
      gtk_widget_queue_draw (GTK_WIDGET (chart));
      g_object_notify_by_pspec (G_OBJECT (chart), properties[PROP_SHOW_GRID]);
    }
}

/**
 * slate_chart_get_show_grid:
 * @chart: a #SlateChart
 *
 * Gets whether the grid is shown.
 *
 * Returns: %TRUE if the grid is shown
 */
gboolean
slate_chart_get_show_grid (SlateChart *chart)
{
  g_return_val_if_fail (SLATE_IS_CHART (chart), FALSE);

  return chart->show_grid;
}

/**
 * slate_chart_set_show_legend:
 * @chart: a #SlateChart
 * @show_legend: whether to show the legend
 *
 * Sets whether to show the legend.
 */
void
slate_chart_set_show_legend (SlateChart *chart,
                             gboolean    show_legend)
{
  g_return_if_fail (SLATE_IS_CHART (chart));

  if (chart->show_legend != show_legend)
    {
      chart->show_legend = show_legend;
      gtk_widget_queue_draw (GTK_WIDGET (chart));
      g_object_notify_by_pspec (G_OBJECT (chart), properties[PROP_SHOW_LEGEND]);
    }
}

/**
 * slate_chart_get_show_legend:
 * @chart: a #SlateChart
 *
 * Gets whether the legend is shown.
 *
 * Returns: %TRUE if the legend is shown
 */
gboolean
slate_chart_get_show_legend (SlateChart *chart)
{
  g_return_val_if_fail (SLATE_IS_CHART (chart), FALSE);

  return chart->show_legend;
}

/**
 * slate_chart_refresh:
 * @chart: a #SlateChart
 *
 * Refreshes the chart display.
 */
void
slate_chart_refresh (SlateChart *chart)
{
  g_return_if_fail (SLATE_IS_CHART (chart));

  /* Update data can be extended here in the future */

  gtk_widget_queue_draw (GTK_WIDGET (chart));
}

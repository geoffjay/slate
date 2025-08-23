/* slate-chart.h
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

#pragma once

#include <adwaita.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define SLATE_TYPE_CHART (slate_chart_get_type())

G_DECLARE_FINAL_TYPE (SlateChart, slate_chart, SLATE, CHART, GtkWidget)



/**
 * SlateChartType:
 * @SLATE_CHART_TYPE_LINE: Line chart
 * @SLATE_CHART_TYPE_BAR: Bar chart
 * @SLATE_CHART_TYPE_PIE: Pie chart
 * @SLATE_CHART_TYPE_SCATTER: Scatter plot
 *
 * Chart types supported by SlateChart.
 */
typedef enum {
  SLATE_CHART_TYPE_LINE,
  SLATE_CHART_TYPE_BAR,
  SLATE_CHART_TYPE_PIE,
  SLATE_CHART_TYPE_SCATTER
} SlateChartType;

/**
 * SlateChartDataPoint:
 * @x: X coordinate value
 * @y: Y coordinate value
 * @label: Optional label for the data point
 *
 * Represents a single data point in a chart.
 */
typedef struct {
  double x;
  double y;
  char *label;
} SlateChartDataPoint;

SlateChart * slate_chart_new (SlateChartType chart_type);

void slate_chart_set_title (SlateChart *chart, const char *title);
const char * slate_chart_get_title (SlateChart *chart);

void slate_chart_set_chart_type (SlateChart *chart, SlateChartType chart_type);
SlateChartType slate_chart_get_chart_type (SlateChart *chart);

void slate_chart_add_data_point (SlateChart *chart, double x, double y, const char *label);
void slate_chart_clear_data (SlateChart *chart);
void slate_chart_set_data (SlateChart *chart, SlateChartDataPoint *points, int n_points);

void slate_chart_set_x_range (SlateChart *chart, double min, double max);
void slate_chart_set_y_range (SlateChart *chart, double min, double max);
void slate_chart_get_x_range (SlateChart *chart, double *min, double *max);
void slate_chart_get_y_range (SlateChart *chart, double *min, double *max);

void slate_chart_set_show_grid (SlateChart *chart, gboolean show_grid);
gboolean slate_chart_get_show_grid (SlateChart *chart);

void slate_chart_set_show_legend (SlateChart *chart, gboolean show_legend);
gboolean slate_chart_get_show_legend (SlateChart *chart);

void slate_chart_refresh (SlateChart *chart);

G_END_DECLS

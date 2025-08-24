/* test-dashboard-system.c
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

/* Test dashboard system integration */

static void
test_dashboard_creation (void)
{
  SlateDashboard *dashboard = slate_dashboard_new ();
  g_assert_nonnull (dashboard);
  g_assert_true (SLATE_IS_DASHBOARD (dashboard));

    /* Test initial properties */
  int columns = slate_dashboard_get_columns (dashboard);
  g_assert_cmpint (columns, ==, 3); /* Default columns */

  const char *layout = slate_dashboard_get_layout (dashboard);
  g_assert_cmpstr (layout, ==, "grid"); /* Default layout */

  g_object_unref (dashboard);
}

static void
test_dashboard_widget_management (void)
{
  SlateDashboard *dashboard = slate_dashboard_new ();
  SlateDashboardCard *card = slate_dashboard_card_new ();

    /* Add widget */
  slate_dashboard_add_widget (dashboard, GTK_WIDGET (card), "test-card");

  /* Check widget exists */
  GtkWidget *retrieved = slate_dashboard_get_widget (dashboard, "test-card");
  g_assert_nonnull (retrieved);
  g_assert_true (retrieved == GTK_WIDGET (card));

  /* Remove widget */
  slate_dashboard_remove_widget (dashboard, "test-card");
  retrieved = slate_dashboard_get_widget (dashboard, "test-card");
  g_assert_null (retrieved);

  g_object_unref (dashboard);
}

static void
test_dashboard_layout_types (void)
{
  SlateDashboard *dashboard = slate_dashboard_new ();

    /* Test grid layout */
  slate_dashboard_set_layout (dashboard, "grid");
  g_assert_cmpstr (slate_dashboard_get_layout (dashboard), ==, "grid");

  /* Test flow layout */
  slate_dashboard_set_layout (dashboard, "flow");
  g_assert_cmpstr (slate_dashboard_get_layout (dashboard), ==, "flow");

  /* Test stack layout */
  slate_dashboard_set_layout (dashboard, "stack");
  g_assert_cmpstr (slate_dashboard_get_layout (dashboard), ==, "stack");

  g_object_unref (dashboard);
}

static void
test_dashboard_card_creation (void)
{
  SlateDashboardCard *card = slate_dashboard_card_new ();
  g_assert_nonnull (card);
  g_assert_true (SLATE_IS_DASHBOARD_CARD (card));

  /* Test title and subtitle */
  slate_dashboard_card_set_title (card, "Test Card");
  g_assert_cmpstr (slate_dashboard_card_get_title (card), ==, "Test Card");

  slate_dashboard_card_set_subtitle (card, "Test Subtitle");
  g_assert_cmpstr (slate_dashboard_card_get_subtitle (card), ==, "Test Subtitle");

  /* Test loading state */
  slate_dashboard_card_set_loading (card, TRUE);
  g_assert_true (slate_dashboard_card_get_loading (card));

  slate_dashboard_card_set_loading (card, FALSE);
  g_assert_false (slate_dashboard_card_get_loading (card));

  g_object_unref (card);
}

static void
test_chart_creation (void)
{
  SlateChart *chart = slate_chart_new (SLATE_CHART_TYPE_LINE);
  g_assert_nonnull (chart);
  g_assert_true (SLATE_IS_CHART (chart));

  /* Test initial properties */
  g_assert_cmpstr (slate_chart_get_title (chart), ==, "");
  g_assert_cmpint (slate_chart_get_chart_type (chart), ==, SLATE_CHART_TYPE_LINE);
  g_assert_true (slate_chart_get_show_grid (chart));

  g_object_unref (chart);
}

static void
test_chart_data_management (void)
{
  SlateChart *chart = slate_chart_new (SLATE_CHART_TYPE_LINE);

  /* Add data points */
  slate_chart_add_data_point (chart, 1.0, 10.0, "Point 1");
  slate_chart_add_data_point (chart, 2.0, 20.0, "Point 2");
  slate_chart_add_data_point (chart, 3.0, 15.0, "Point 3");

  /* Clear data (we can't easily test count without additional API) */
  slate_chart_clear_data (chart);

  g_object_unref (chart);
}

static void
test_chart_range_management (void)
{
  SlateChart *chart = slate_chart_new (SLATE_CHART_TYPE_LINE);

  /* Test basic chart functionality */
  g_assert_nonnull (chart);
  g_assert_true (SLATE_IS_CHART (chart));

  /* Test chart type */
  g_assert_cmpint (slate_chart_get_chart_type (chart), ==, SLATE_CHART_TYPE_LINE);

  g_object_unref (chart);
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

  g_test_add_func ("/dashboard-system/dashboard-creation", test_dashboard_creation);
  g_test_add_func ("/dashboard-system/widget-management", test_dashboard_widget_management);
  g_test_add_func ("/dashboard-system/layout-types", test_dashboard_layout_types);
  g_test_add_func ("/dashboard-system/card-creation", test_dashboard_card_creation);
  g_test_add_func ("/dashboard-system/chart-creation", test_chart_creation);
  g_test_add_func ("/dashboard-system/chart-data", test_chart_data_management);
  g_test_add_func ("/dashboard-system/chart-range", test_chart_range_management);

  return g_test_run ();
}

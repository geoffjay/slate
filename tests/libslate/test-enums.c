/* test-enums.c
 *
 * Copyright 2024 Slate Contributors
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
#include "../../src/libslate/ui/slate-enums.h"

static void
test_orientation_conversions (void)
{
  /* Test Slate to GTK conversion */
  g_assert_cmpint (slate_orientation_to_gtk (SLATE_ORIENTATION_HORIZONTAL), ==, GTK_ORIENTATION_HORIZONTAL);
  g_assert_cmpint (slate_orientation_to_gtk (SLATE_ORIENTATION_VERTICAL), ==, GTK_ORIENTATION_VERTICAL);

  /* Test string conversions */
  g_assert_cmpstr (slate_orientation_to_string (SLATE_ORIENTATION_HORIZONTAL), ==, "horizontal");
  g_assert_cmpstr (slate_orientation_to_string (SLATE_ORIENTATION_VERTICAL), ==, "vertical");

  /* Test parsing */
  g_assert_cmpint (slate_orientation_parse ("horizontal"), ==, SLATE_ORIENTATION_HORIZONTAL);
  g_assert_cmpint (slate_orientation_parse ("vertical"), ==, SLATE_ORIENTATION_VERTICAL);
  g_assert_cmpint (slate_orientation_parse ("HORIZONTAL"), ==, SLATE_ORIENTATION_HORIZONTAL);
  g_assert_cmpint (slate_orientation_parse ("invalid"), ==, SLATE_ORIENTATION_HORIZONTAL);
  g_assert_cmpint (slate_orientation_parse (NULL), ==, SLATE_ORIENTATION_HORIZONTAL);
}

static void
test_position_conversions (void)
{
  /* Test Slate to GTK conversion */
  g_assert_cmpint (slate_position_type_to_gtk (SLATE_POSITION_LEFT), ==, GTK_POS_LEFT);
  g_assert_cmpint (slate_position_type_to_gtk (SLATE_POSITION_RIGHT), ==, GTK_POS_RIGHT);
  g_assert_cmpint (slate_position_type_to_gtk (SLATE_POSITION_TOP), ==, GTK_POS_TOP);
  g_assert_cmpint (slate_position_type_to_gtk (SLATE_POSITION_BOTTOM), ==, GTK_POS_BOTTOM);

  /* Test string conversions */
  g_assert_cmpstr (slate_position_type_to_string (SLATE_POSITION_LEFT), ==, "left");
  g_assert_cmpstr (slate_position_type_to_string (SLATE_POSITION_RIGHT), ==, "right");
  g_assert_cmpstr (slate_position_type_to_string (SLATE_POSITION_TOP), ==, "top");
  g_assert_cmpstr (slate_position_type_to_string (SLATE_POSITION_BOTTOM), ==, "bottom");

  /* Test parsing */
  g_assert_cmpint (slate_position_type_parse ("left"), ==, SLATE_POSITION_LEFT);
  g_assert_cmpint (slate_position_type_parse ("right"), ==, SLATE_POSITION_RIGHT);
  g_assert_cmpint (slate_position_type_parse ("TOP"), ==, SLATE_POSITION_TOP);
  g_assert_cmpint (slate_position_type_parse ("invalid"), ==, SLATE_POSITION_LEFT);
  g_assert_cmpint (slate_position_type_parse (NULL), ==, SLATE_POSITION_LEFT);
}

int
main (int argc, char *argv[])
{
  gtk_test_init (&argc, &argv, NULL);

  g_test_add_func ("/enums/orientation", test_orientation_conversions);
  g_test_add_func ("/enums/position", test_position_conversions);

  return g_test_run ();
}

/* test-box.c
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
#include "../../src/libslate/ui/slate-box.h"
#include "../../src/libslate/ui/slate-widget.h"
#include "../../src/libslate/core/slate-buildable.h"

static void
test_box_creation (void)
{
  GtkWidget *box = slate_box_new ();
  g_object_ref_sink (box);

  g_assert_nonnull (box);
  g_assert_true (SLATE_IS_BOX (box));
  g_assert_true (GTK_IS_BOX (box));
  g_assert_true (SLATE_IS_WIDGET (box));
  g_assert_true (SLATE_IS_BUILDABLE (box));

  /* Test default values */
  g_assert_cmpstr (slate_box_get_id (SLATE_BOX (box)), ==, "box0");
  g_assert_true (slate_widget_get_fill (SLATE_WIDGET (box)));

  g_object_unref (box);
}

static void
test_box_properties (void)
{
  GtkWidget *box = slate_box_new ();
  g_object_ref_sink (box);
  SlateBox *slate_box = SLATE_BOX (box);

  /* Test ID property */
  slate_box_set_id (slate_box, "test-box");
  g_assert_cmpstr (slate_box_get_id (slate_box), ==, "test-box");

  /* Test orientation property */
  slate_box_set_slate_orientation (slate_box, SLATE_ORIENTATION_HORIZONTAL);
  g_assert_cmpint (slate_box_get_slate_orientation (slate_box), ==, SLATE_ORIENTATION_HORIZONTAL);

  slate_box_set_slate_orientation (slate_box, SLATE_ORIENTATION_VERTICAL);
  g_assert_cmpint (slate_box_get_slate_orientation (slate_box), ==, SLATE_ORIENTATION_VERTICAL);

  /* Test homogeneous property */
  slate_box_set_homogeneous (slate_box, TRUE);
  g_assert_true (slate_box_get_homogeneous (slate_box));

  slate_box_set_homogeneous (slate_box, FALSE);
  g_assert_false (slate_box_get_homogeneous (slate_box));

  /* Test fill property through SlateWidget interface */
  slate_widget_set_fill (SLATE_WIDGET (box), FALSE);
  g_assert_false (slate_widget_get_fill (SLATE_WIDGET (box)));

  g_object_unref (box);
}

static void
test_box_child_management (void)
{
  GtkWidget *box = slate_box_new ();
  g_object_ref_sink (box);
  GtkWidget *child = gtk_button_new_with_label ("Test Button");

  slate_box_add_child (SLATE_BOX (box), child);

  /* Verify child was added */
  GtkWidget *first_child = gtk_widget_get_first_child (box);
  g_assert_true (first_child == child);

  g_object_unref (box);
}

static void
test_box_buildable_interface (void)
{
  GtkWidget *box = slate_box_new ();
  g_object_ref_sink (box);
  SlateBuildable *buildable = SLATE_BUILDABLE (box);

  /* Test XML/XSD getters */
  const char *xml = slate_buildable_get_xml (buildable);
  const char *xsd = slate_buildable_get_xsd (buildable);

  g_assert_nonnull (xml);
  g_assert_nonnull (xsd);
  g_assert_true (g_str_has_prefix (xml, "<object"));
  g_assert_true (g_str_has_prefix (xsd, "<xs:element"));

  /* Test node getters/setters */
  g_assert_null (slate_buildable_get_node (buildable));

  g_object_unref (box);
}

int
main (int argc, char *argv[])
{
  gtk_test_init (&argc, &argv, NULL);

  g_test_add_func ("/box/creation", test_box_creation);
  g_test_add_func ("/box/properties", test_box_properties);
  g_test_add_func ("/box/child_management", test_box_child_management);
  g_test_add_func ("/box/buildable_interface", test_box_buildable_interface);

  return g_test_run ();
}

/* test-value.c - Tests for HclValue
 *
 * Copyright 2024 Geoff Johnson <geoff.jay@gmail.com>
 */

#include <glib.h>
#include <hcl.h>

static void
test_value_null (void)
{
  g_autoptr(HclValue) value = hcl_value_new_null ();

  g_assert_true (hcl_value_is_null (value));
  g_assert_false (hcl_value_is_bool (value));
  g_assert_false (hcl_value_is_number (value));
  g_assert_false (hcl_value_is_string (value));
  g_assert_false (hcl_value_is_list (value));
  g_assert_false (hcl_value_is_object (value));

  g_assert_cmpint (hcl_value_get_value_type (value), ==, HCL_VALUE_TYPE_NULL);
}

static void
test_value_bool (void)
{
  g_autoptr(HclValue) value_true = hcl_value_new_bool (TRUE);
  g_autoptr(HclValue) value_false = hcl_value_new_bool (FALSE);

  g_assert_true (hcl_value_is_bool (value_true));
  g_assert_true (hcl_value_is_bool (value_false));

  g_assert_true (hcl_value_get_bool (value_true));
  g_assert_false (hcl_value_get_bool (value_false));
}

static void
test_value_number (void)
{
  g_autoptr(HclValue) int_value = hcl_value_new_int (42);
  g_autoptr(HclValue) double_value = hcl_value_new_double (3.14);

  g_assert_true (hcl_value_is_number (int_value));
  g_assert_true (hcl_value_is_number (double_value));

  g_assert_cmpint (hcl_value_get_int (int_value), ==, 42);
  g_assert_cmpfloat (hcl_value_get_double (double_value), ==, 3.14);

  /* Test conversion */
  g_assert_cmpfloat (hcl_value_get_double (int_value), ==, 42.0);
  g_assert_cmpint (hcl_value_get_int (double_value), ==, 3);
}

static void
test_value_string (void)
{
  g_autoptr(HclValue) value = hcl_value_new_string ("hello world");

  g_assert_true (hcl_value_is_string (value));
  g_assert_cmpstr (hcl_value_get_string (value), ==, "hello world");
}

static void
test_value_list (void)
{
  g_autoptr(HclValue) list = hcl_value_new_list ();
  g_autoptr(HclValue) item1 = hcl_value_new_string ("item1");
  g_autoptr(HclValue) item2 = hcl_value_new_int (42);

  g_assert_true (hcl_value_is_list (list));
  g_assert_cmpuint (hcl_value_list_get_length (list), ==, 0);

  hcl_value_list_add_item (list, g_object_ref (item1));
  hcl_value_list_add_item (list, g_object_ref (item2));

  g_assert_cmpuint (hcl_value_list_get_length (list), ==, 2);

  HclValue *retrieved1 = hcl_value_list_get_item (list, 0);
  HclValue *retrieved2 = hcl_value_list_get_item (list, 1);

  g_assert_true (hcl_value_is_string (retrieved1));
  g_assert_true (hcl_value_is_number (retrieved2));

  g_assert_cmpstr (hcl_value_get_string (retrieved1), ==, "item1");
  g_assert_cmpint (hcl_value_get_int (retrieved2), ==, 42);
}

static void
test_value_object (void)
{
  g_autoptr(HclValue) object = hcl_value_new_object ();
  g_autoptr(HclValue) value1 = hcl_value_new_string ("value1");
  g_autoptr(HclValue) value2 = hcl_value_new_int (42);

  g_assert_true (hcl_value_is_object (object));
  g_assert_false (hcl_value_object_has_member (object, "key1"));

  hcl_value_object_set_member (object, "key1", g_object_ref (value1));
  hcl_value_object_set_member (object, "key2", g_object_ref (value2));

  g_assert_true (hcl_value_object_has_member (object, "key1"));
  g_assert_true (hcl_value_object_has_member (object, "key2"));
  g_assert_false (hcl_value_object_has_member (object, "key3"));

  HclValue *retrieved1 = hcl_value_object_get_member (object, "key1");
  HclValue *retrieved2 = hcl_value_object_get_member (object, "key2");

  g_assert_nonnull (retrieved1);
  g_assert_nonnull (retrieved2);

  g_assert_true (hcl_value_is_string (retrieved1));
  g_assert_true (hcl_value_is_number (retrieved2));

  g_assert_cmpstr (hcl_value_get_string (retrieved1), ==, "value1");
  g_assert_cmpint (hcl_value_get_int (retrieved2), ==, 42);

  GList *keys = hcl_value_object_get_keys (object);
  g_assert_cmpuint (g_list_length (keys), ==, 2);
  g_list_free (keys);
}

int
main (int argc, char **argv)
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/hcl/value/null", test_value_null);
  g_test_add_func ("/hcl/value/bool", test_value_bool);
  g_test_add_func ("/hcl/value/number", test_value_number);
  g_test_add_func ("/hcl/value/string", test_value_string);
  g_test_add_func ("/hcl/value/list", test_value_list);
  g_test_add_func ("/hcl/value/object", test_value_object);

  return g_test_run ();
}

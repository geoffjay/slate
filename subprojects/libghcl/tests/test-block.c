/* test-block.c - Tests for HclBlock
 *
 * Copyright 2024 The Slate Project
 */

#include <glib.h>
#include <hcl.h>

static void
test_block_basic (void)
{
  g_autoptr(HclBlock) block = hcl_block_new ("application", "myapp");

  g_assert_cmpstr (hcl_block_get_block_type (block), ==, "application");
  g_assert_cmpstr (hcl_block_get_label (block), ==, "myapp");

  hcl_block_set_label (block, "newapp");
  g_assert_cmpstr (hcl_block_get_label (block), ==, "newapp");
}

static void
test_block_attributes (void)
{
  g_autoptr(HclBlock) block = hcl_block_new ("test", NULL);
  g_autoptr(HclValue) value = hcl_value_new_string ("test_value");

  g_assert_false (hcl_block_has_attribute (block, "test_attr"));

  hcl_block_set_attribute (block, "test_attr", g_object_ref (value));

  g_assert_true (hcl_block_has_attribute (block, "test_attr"));

  HclValue *retrieved = hcl_block_get_attribute (block, "test_attr");
  g_assert_nonnull (retrieved);
  g_assert_true (hcl_value_is_string (retrieved));
  g_assert_cmpstr (hcl_value_get_string (retrieved), ==, "test_value");
}

static void
test_block_nested_blocks (void)
{
  g_autoptr(HclBlock) parent = hcl_block_new ("parent", NULL);
  g_autoptr(HclBlock) child1 = hcl_block_new ("child", "first");
  g_autoptr(HclBlock) child2 = hcl_block_new ("child", "second");
  g_autoptr(HclBlock) other = hcl_block_new ("other", NULL);

  hcl_block_add_block (parent, g_object_ref (child1));
  hcl_block_add_block (parent, g_object_ref (child2));
  hcl_block_add_block (parent, g_object_ref (other));

  GList *all_blocks = hcl_block_get_blocks (parent);
  g_assert_cmpuint (g_list_length (all_blocks), ==, 3);

  GList *child_blocks = hcl_block_get_blocks_by_type (parent, "child");
  g_assert_cmpuint (g_list_length (child_blocks), ==, 2);

  GList *other_blocks = hcl_block_get_blocks_by_type (parent, "other");
  g_assert_cmpuint (g_list_length (other_blocks), ==, 1);

  g_list_free (all_blocks);
  g_list_free (child_blocks);
  g_list_free (other_blocks);
}

int
main (int argc, char **argv)
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/hcl/block/basic", test_block_basic);
  g_test_add_func ("/hcl/block/attributes", test_block_attributes);
  g_test_add_func ("/hcl/block/nested_blocks", test_block_nested_blocks);

  return g_test_run ();
}

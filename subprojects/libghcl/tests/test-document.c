/* test-document.c - Tests for HclDocument
 *
 * Copyright 2024 The Slate Project
 */

#include <glib.h>
#include <hcl.h>

static void
test_document_basic (void)
{
  g_autoptr(HclDocument) document = hcl_document_new ();
  g_assert_nonnull (document);
}

static void
test_document_attributes (void)
{
  g_autoptr(HclDocument) document = hcl_document_new ();
  g_autoptr(HclValue) value = hcl_value_new_string ("test_value");

  g_assert_false (hcl_document_has_attribute (document, "test_attr"));

  hcl_document_set_attribute (document, "test_attr", g_object_ref (value));

  g_assert_true (hcl_document_has_attribute (document, "test_attr"));

  HclValue *retrieved = hcl_document_get_attribute (document, "test_attr");
  g_assert_nonnull (retrieved);
  g_assert_true (hcl_value_is_string (retrieved));
  g_assert_cmpstr (hcl_value_get_string (retrieved), ==, "test_value");
}

static void
test_document_blocks (void)
{
  g_autoptr(HclDocument) document = hcl_document_new ();
  g_autoptr(HclBlock) block1 = hcl_block_new ("application", "app1");
  g_autoptr(HclBlock) block2 = hcl_block_new ("application", "app2");
  g_autoptr(HclBlock) block3 = hcl_block_new ("database", "db1");

  hcl_document_add_block (document, g_object_ref (block1));
  hcl_document_add_block (document, g_object_ref (block2));
  hcl_document_add_block (document, g_object_ref (block3));

  GList *all_blocks = hcl_document_get_blocks (document);
  g_assert_cmpuint (g_list_length (all_blocks), ==, 3);

  GList *app_blocks = hcl_document_get_blocks_by_type (document, "application");
  g_assert_cmpuint (g_list_length (app_blocks), ==, 2);

  GList *db_blocks = hcl_document_get_blocks_by_type (document, "database");
  g_assert_cmpuint (g_list_length (db_blocks), ==, 1);

  g_list_free (all_blocks);
  g_list_free (app_blocks);
  g_list_free (db_blocks);
}

int
main (int argc, char **argv)
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/hcl/document/basic", test_document_basic);
  g_test_add_func ("/hcl/document/attributes", test_document_attributes);
  g_test_add_func ("/hcl/document/blocks", test_document_blocks);

  return g_test_run ();
}

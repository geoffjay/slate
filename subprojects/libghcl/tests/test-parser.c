/* test-parser.c - Tests for HclParser
 *
 * Copyright 2024 The Slate Project
 */

#include <glib.h>
#include <hcl.h>

static void
test_parse_simple_attribute (void)
{
  const gchar *input = "name = \"test\"";
  g_autoptr(GError) error = NULL;
  g_autoptr(HclDocument) document = hcl_parse_string (input, &error);

  g_assert_no_error (error);
  g_assert_nonnull (document);

  g_assert_true (hcl_document_has_attribute (document, "name"));

  HclValue *value = hcl_document_get_attribute (document, "name");
  g_assert_nonnull (value);
  g_assert_true (hcl_value_is_string (value));
  g_assert_cmpstr (hcl_value_get_string (value), ==, "test");
}

static void
test_parse_multiple_attributes (void)
{
  const gchar *input =
    "name = \"test\"\n"
    "count = 42\n"
    "enabled = true\n";

  g_autoptr(GError) error = NULL;
  g_autoptr(HclDocument) document = hcl_parse_string (input, &error);

  g_assert_no_error (error);
  g_assert_nonnull (document);

  /* Check name */
  g_assert_true (hcl_document_has_attribute (document, "name"));
  HclValue *name = hcl_document_get_attribute (document, "name");
  g_assert_true (hcl_value_is_string (name));
  g_assert_cmpstr (hcl_value_get_string (name), ==, "test");

  /* Check count */
  g_assert_true (hcl_document_has_attribute (document, "count"));
  HclValue *count = hcl_document_get_attribute (document, "count");
  g_assert_true (hcl_value_is_number (count));
  g_assert_cmpint (hcl_value_get_int (count), ==, 42);

  /* Check enabled */
  g_assert_true (hcl_document_has_attribute (document, "enabled"));
  HclValue *enabled = hcl_document_get_attribute (document, "enabled");
  g_assert_true (hcl_value_is_bool (enabled));
  g_assert_true (hcl_value_get_bool (enabled));
}

static void
test_parse_simple_block (void)
{
  const gchar *input =
    "application \"myapp\" {\n"
    "  name = \"test\"\n"
    "  count = 42\n"
    "}\n";

  g_autoptr(GError) error = NULL;
  g_autoptr(HclDocument) document = hcl_parse_string (input, &error);

  g_assert_no_error (error);
  g_assert_nonnull (document);

  GList *blocks = hcl_document_get_blocks_by_type (document, "application");
  g_assert_cmpuint (g_list_length (blocks), ==, 1);

  HclBlock *block = blocks->data;
  g_assert_nonnull (block);
  g_assert_cmpstr (hcl_block_get_block_type (block), ==, "application");
  g_assert_cmpstr (hcl_block_get_label (block), ==, "myapp");

  g_assert_true (hcl_block_has_attribute (block, "name"));
  g_assert_true (hcl_block_has_attribute (block, "count"));

  HclValue *name = hcl_block_get_attribute (block, "name");
  g_assert_true (hcl_value_is_string (name));
  g_assert_cmpstr (hcl_value_get_string (name), ==, "test");

  HclValue *count = hcl_block_get_attribute (block, "count");
  g_assert_true (hcl_value_is_number (count));
  g_assert_cmpint (hcl_value_get_int (count), ==, 42);

  g_list_free (blocks);
}

static void
test_parse_nested_blocks (void)
{
  const gchar *input =
    "application \"myapp\" {\n"
    "  dashboard {\n"
    "    title = \"My Dashboard\"\n"
    "    box \"container\" {\n"
    "      width = \"full\"\n"
    "      height = \"full\"\n"
    "    }\n"
    "  }\n"
    "}\n";

  g_autoptr(GError) error = NULL;
  g_autoptr(HclDocument) document = hcl_parse_string (input, &error);

  g_assert_no_error (error);
  g_assert_nonnull (document);

  GList *blocks = hcl_document_get_blocks_by_type (document, "application");
  g_assert_cmpuint (g_list_length (blocks), ==, 1);

  HclBlock *app_block = blocks->data;
  g_assert_nonnull (app_block);

  GList *dashboard_blocks = hcl_block_get_blocks_by_type (app_block, "dashboard");
  g_assert_cmpuint (g_list_length (dashboard_blocks), ==, 1);

  HclBlock *dashboard_block = dashboard_blocks->data;
  g_assert_nonnull (dashboard_block);

  g_assert_true (hcl_block_has_attribute (dashboard_block, "title"));
  HclValue *title = hcl_block_get_attribute (dashboard_block, "title");
  g_assert_cmpstr (hcl_value_get_string (title), ==, "My Dashboard");

  GList *box_blocks = hcl_block_get_blocks_by_type (dashboard_block, "box");
  g_assert_cmpuint (g_list_length (box_blocks), ==, 1);

  HclBlock *box_block = box_blocks->data;
  g_assert_nonnull (box_block);
  g_assert_cmpstr (hcl_block_get_label (box_block), ==, "container");

  g_assert_true (hcl_block_has_attribute (box_block, "width"));
  g_assert_true (hcl_block_has_attribute (box_block, "height"));

  g_list_free (blocks);
  g_list_free (dashboard_blocks);
  g_list_free (box_blocks);
}

static void
test_parse_list_values (void)
{
  const gchar *input =
    "items = [\"first\", \"second\", 42]\n"
    "numbers = [1, 2, 3]\n";

  g_autoptr(GError) error = NULL;
  g_autoptr(HclDocument) document = hcl_parse_string (input, &error);

  g_assert_no_error (error);
  g_assert_nonnull (document);

  HclValue *items = hcl_document_get_attribute (document, "items");
  g_assert_nonnull (items);
  g_assert_true (hcl_value_is_list (items));
  g_assert_cmpuint (hcl_value_list_get_length (items), ==, 3);

  HclValue *first = hcl_value_list_get_item (items, 0);
  g_assert_true (hcl_value_is_string (first));
  g_assert_cmpstr (hcl_value_get_string (first), ==, "first");

  HclValue *second = hcl_value_list_get_item (items, 1);
  g_assert_true (hcl_value_is_string (second));
  g_assert_cmpstr (hcl_value_get_string (second), ==, "second");

  HclValue *third = hcl_value_list_get_item (items, 2);
  g_assert_true (hcl_value_is_number (third));
  g_assert_cmpint (hcl_value_get_int (third), ==, 42);
}

static void
test_parse_object_values (void)
{
  const gchar *input =
    "config = {\n"
    "  host = \"localhost\"\n"
    "  port = 8080\n"
    "  ssl = true\n"
    "}\n";

  g_autoptr(GError) error = NULL;
  g_autoptr(HclDocument) document = hcl_parse_string (input, &error);

  g_assert_no_error (error);
  g_assert_nonnull (document);

  HclValue *config = hcl_document_get_attribute (document, "config");
  g_assert_nonnull (config);
  g_assert_true (hcl_value_is_object (config));

  g_assert_true (hcl_value_object_has_member (config, "host"));
  g_assert_true (hcl_value_object_has_member (config, "port"));
  g_assert_true (hcl_value_object_has_member (config, "ssl"));

  HclValue *host = hcl_value_object_get_member (config, "host");
  g_assert_true (hcl_value_is_string (host));
  g_assert_cmpstr (hcl_value_get_string (host), ==, "localhost");

  HclValue *port = hcl_value_object_get_member (config, "port");
  g_assert_true (hcl_value_is_number (port));
  g_assert_cmpint (hcl_value_get_int (port), ==, 8080);

  HclValue *ssl = hcl_value_object_get_member (config, "ssl");
  g_assert_true (hcl_value_is_bool (ssl));
  g_assert_true (hcl_value_get_bool (ssl));
}

static void
test_parse_with_comments (void)
{
  const gchar *input =
    "# This is a comment\n"
    "name = \"test\" # inline comment\n"
    "// Another comment style\n"
    "count = 42\n";

  g_autoptr(GError) error = NULL;
  g_autoptr(HclDocument) document = hcl_parse_string (input, &error);

  g_assert_no_error (error);
  g_assert_nonnull (document);

  g_assert_true (hcl_document_has_attribute (document, "name"));
  g_assert_true (hcl_document_has_attribute (document, "count"));

  HclValue *name = hcl_document_get_attribute (document, "name");
  g_assert_cmpstr (hcl_value_get_string (name), ==, "test");

  HclValue *count = hcl_document_get_attribute (document, "count");
  g_assert_cmpint (hcl_value_get_int (count), ==, 42);
}

static void
test_parse_error_handling (void)
{
  const gchar *invalid_input = "name = ";
  g_autoptr(GError) error = NULL;
  g_autoptr(HclDocument) document = hcl_parse_string (invalid_input, &error);

  g_assert_error (error, HCL_PARSER_ERROR, HCL_PARSER_ERROR_MISSING_VALUE);
  g_assert_null (document);
}

int
main (int argc, char **argv)
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/hcl/parser/simple_attribute", test_parse_simple_attribute);
  g_test_add_func ("/hcl/parser/multiple_attributes", test_parse_multiple_attributes);
  g_test_add_func ("/hcl/parser/simple_block", test_parse_simple_block);
  g_test_add_func ("/hcl/parser/nested_blocks", test_parse_nested_blocks);
  g_test_add_func ("/hcl/parser/list_values", test_parse_list_values);
  g_test_add_func ("/hcl/parser/object_values", test_parse_object_values);
  g_test_add_func ("/hcl/parser/with_comments", test_parse_with_comments);
  g_test_add_func ("/hcl/parser/error_handling", test_parse_error_handling);

  return g_test_run ();
}

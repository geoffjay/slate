/* test-lexer-enhanced.c - Enhanced tests for HclLexer
 *
 * Copyright 2025 Slate Contributors
 */

#include <glib.h>
#include <hcl.h>

static void
test_lexer_enhanced_numbers (void)
{
  const gchar *input = "42 0xFF 0b1010 3.14e-2 -123 0x1A2B";
  g_autoptr(HclLexer) lexer = hcl_lexer_new (input);
  g_autoptr(GError) error = NULL;

  /* Test decimal */
  g_autoptr(HclToken) token1 = hcl_lexer_next_token (lexer, &error);
  g_assert_no_error (error);
  g_assert_cmpint (hcl_token_get_token_type (token1), ==, HCL_TOKEN_TYPE_NUMBER);
  g_assert_cmpstr (hcl_token_get_value (token1), ==, "42");

  /* Test hexadecimal */
  g_autoptr(HclToken) token2 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token2), ==, HCL_TOKEN_TYPE_NUMBER);
  g_assert_cmpstr (hcl_token_get_value (token2), ==, "0xFF");

  /* Test binary */
  g_autoptr(HclToken) token3 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token3), ==, HCL_TOKEN_TYPE_NUMBER);
  g_assert_cmpstr (hcl_token_get_value (token3), ==, "0b1010");

  /* Test scientific notation */
  g_autoptr(HclToken) token4 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token4), ==, HCL_TOKEN_TYPE_NUMBER);
  g_assert_cmpstr (hcl_token_get_value (token4), ==, "3.14e-2");

  /* Test negative number */
  g_autoptr(HclToken) token5 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token5), ==, HCL_TOKEN_TYPE_NUMBER);
  g_assert_cmpstr (hcl_token_get_value (token5), ==, "-123");

  /* Test hex with mixed case */
  g_autoptr(HclToken) token6 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token6), ==, HCL_TOKEN_TYPE_NUMBER);
  g_assert_cmpstr (hcl_token_get_value (token6), ==, "0x1A2B");
}

static void
test_lexer_null_literal (void)
{
  const gchar *input = "null identifier null_value";
  g_autoptr(HclLexer) lexer = hcl_lexer_new (input);
  g_autoptr(GError) error = NULL;

  /* Test null literal */
  g_autoptr(HclToken) token1 = hcl_lexer_next_token (lexer, &error);
  g_assert_no_error (error);
  g_assert_cmpint (hcl_token_get_token_type (token1), ==, HCL_TOKEN_TYPE_NULL);
  g_assert_cmpstr (hcl_token_get_value (token1), ==, "null");

  /* Test regular identifier */
  g_autoptr(HclToken) token2 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token2), ==, HCL_TOKEN_TYPE_IDENTIFIER);
  g_assert_cmpstr (hcl_token_get_value (token2), ==, "identifier");

  /* Test identifier containing null */
  g_autoptr(HclToken) token3 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token3), ==, HCL_TOKEN_TYPE_IDENTIFIER);
  g_assert_cmpstr (hcl_token_get_value (token3), ==, "null_value");
}

static void
test_lexer_dot_token (void)
{
  const gchar *input = "object.property . another.path";
  g_autoptr(HclLexer) lexer = hcl_lexer_new (input);
  g_autoptr(GError) error = NULL;

  /* Test identifier */
  g_autoptr(HclToken) token1 = hcl_lexer_next_token (lexer, &error);
  g_assert_no_error (error);
  g_assert_cmpint (hcl_token_get_token_type (token1), ==, HCL_TOKEN_TYPE_IDENTIFIER);
  g_assert_cmpstr (hcl_token_get_value (token1), ==, "object");

  /* Test dot */
  g_autoptr(HclToken) token2 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token2), ==, HCL_TOKEN_TYPE_DOT);
  g_assert_cmpstr (hcl_token_get_value (token2), ==, ".");

  /* Test property */
  g_autoptr(HclToken) token3 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token3), ==, HCL_TOKEN_TYPE_IDENTIFIER);
  g_assert_cmpstr (hcl_token_get_value (token3), ==, "property");

  /* Test standalone dot */
  g_autoptr(HclToken) token4 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token4), ==, HCL_TOKEN_TYPE_DOT);
  g_assert_cmpstr (hcl_token_get_value (token4), ==, ".");
}

static void
test_lexer_enhanced_strings (void)
{
  const gchar *input = "\"hello\\nworld\" \"tab\\there\" \"quote\\\"test\"";
  g_autoptr(HclLexer) lexer = hcl_lexer_new (input);
  g_autoptr(GError) error = NULL;

  /* Test newline escape */
  g_autoptr(HclToken) token1 = hcl_lexer_next_token (lexer, &error);
  g_assert_no_error (error);
  g_assert_cmpint (hcl_token_get_token_type (token1), ==, HCL_TOKEN_TYPE_STRING);
  g_assert_cmpstr (hcl_token_get_value (token1), ==, "hello\nworld");

  /* Test tab escape */
  g_autoptr(HclToken) token2 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token2), ==, HCL_TOKEN_TYPE_STRING);
  g_assert_cmpstr (hcl_token_get_value (token2), ==, "tab\there");

  /* Test quote escape */
  g_autoptr(HclToken) token3 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token3), ==, HCL_TOKEN_TYPE_STRING);
  g_assert_cmpstr (hcl_token_get_value (token3), ==, "quote\"test");
}

static void
test_lexer_complex_config (void)
{
  const gchar *input =
    "app = \"Slate\"\n"
    "version = 1.0\n"
    "debug = true\n"
    "database = null\n"
    "port = 0x1F90\n"
    "binary_flags = 0b11010\n"
    "# This is a comment\n"
    "server {\n"
    "  host = \"localhost\"\n"
    "  timeout = 30.5e2\n"
    "}";

  g_autoptr(HclLexer) lexer = hcl_lexer_new (input);
  g_autoptr(GError) error = NULL;

  /* Test app identifier */
  g_autoptr(HclToken) token1 = hcl_lexer_next_token (lexer, &error);
  g_assert_no_error (error);
  g_assert_cmpint (hcl_token_get_token_type (token1), ==, HCL_TOKEN_TYPE_IDENTIFIER);
  g_assert_cmpstr (hcl_token_get_value (token1), ==, "app");

  /* Test assignment */
  g_autoptr(HclToken) token2 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token2), ==, HCL_TOKEN_TYPE_ASSIGN);

  /* Test string value */
  g_autoptr(HclToken) token3 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token3), ==, HCL_TOKEN_TYPE_STRING);
  g_assert_cmpstr (hcl_token_get_value (token3), ==, "Slate");

  /* Skip to test hex number */
  for (int i = 0; i < 10; i++) {
    g_autoptr(HclToken) skip_token = hcl_lexer_next_token (lexer, &error);
    if (hcl_token_get_token_type (skip_token) == HCL_TOKEN_TYPE_NUMBER &&
        g_str_has_prefix (hcl_token_get_value (skip_token), "0x")) {
      g_assert_cmpstr (hcl_token_get_value (skip_token), ==, "0x1F90");
      break;
    }
  }
}

int
main (int argc, char **argv)
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/hcl/lexer/enhanced_numbers", test_lexer_enhanced_numbers);
  g_test_add_func ("/hcl/lexer/null_literal", test_lexer_null_literal);
  g_test_add_func ("/hcl/lexer/dot_token", test_lexer_dot_token);
  g_test_add_func ("/hcl/lexer/enhanced_strings", test_lexer_enhanced_strings);
  g_test_add_func ("/hcl/lexer/complex_config", test_lexer_complex_config);

  return g_test_run ();
}

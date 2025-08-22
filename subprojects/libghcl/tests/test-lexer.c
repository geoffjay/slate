/* test-lexer.c - Tests for HclLexer
 *
 * Copyright 2024 Geoff Johnson <geoff.jay@gmail.com>
 */

#include <glib.h>
#include <hcl.h>

static void
test_lexer_basic_tokens (void)
{
  const gchar *input = "= { } [ ] ( ) ,";
  g_autoptr(HclLexer) lexer = hcl_lexer_new (input);
  g_autoptr(GError) error = NULL;

  g_autoptr(HclToken) token1 = hcl_lexer_next_token (lexer, &error);
  g_assert_no_error (error);
  g_assert_cmpint (hcl_token_get_token_type (token1), ==, HCL_TOKEN_TYPE_ASSIGN);

  g_autoptr(HclToken) token2 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token2), ==, HCL_TOKEN_TYPE_LBRACE);

  g_autoptr(HclToken) token3 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token3), ==, HCL_TOKEN_TYPE_RBRACE);

  g_autoptr(HclToken) token4 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token4), ==, HCL_TOKEN_TYPE_LBRACKET);

  g_autoptr(HclToken) token5 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token5), ==, HCL_TOKEN_TYPE_RBRACKET);

  g_autoptr(HclToken) token6 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token6), ==, HCL_TOKEN_TYPE_LPAREN);

  g_autoptr(HclToken) token7 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token7), ==, HCL_TOKEN_TYPE_RPAREN);

  g_autoptr(HclToken) token8 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token8), ==, HCL_TOKEN_TYPE_COMMA);
}

static void
test_lexer_strings (void)
{
  const gchar *input = "\"hello world\" 'single quoted'";
  g_autoptr(HclLexer) lexer = hcl_lexer_new (input);
  g_autoptr(GError) error = NULL;

  g_autoptr(HclToken) token1 = hcl_lexer_next_token (lexer, &error);
  g_assert_no_error (error);
  g_assert_cmpint (hcl_token_get_token_type (token1), ==, HCL_TOKEN_TYPE_STRING);
  g_assert_cmpstr (hcl_token_get_value (token1), ==, "hello world");

  g_autoptr(HclToken) token2 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token2), ==, HCL_TOKEN_TYPE_STRING);
  g_assert_cmpstr (hcl_token_get_value (token2), ==, "single quoted");
}

static void
test_lexer_numbers (void)
{
  const gchar *input = "42 3.14 -10 1.5e10";
  g_autoptr(HclLexer) lexer = hcl_lexer_new (input);
  g_autoptr(GError) error = NULL;

  g_autoptr(HclToken) token1 = hcl_lexer_next_token (lexer, &error);
  g_assert_no_error (error);
  g_assert_cmpint (hcl_token_get_token_type (token1), ==, HCL_TOKEN_TYPE_NUMBER);
  g_assert_cmpstr (hcl_token_get_value (token1), ==, "42");

  g_autoptr(HclToken) token2 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token2), ==, HCL_TOKEN_TYPE_NUMBER);
  g_assert_cmpstr (hcl_token_get_value (token2), ==, "3.14");

  g_autoptr(HclToken) token3 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token3), ==, HCL_TOKEN_TYPE_NUMBER);
  g_assert_cmpstr (hcl_token_get_value (token3), ==, "-10");

  g_autoptr(HclToken) token4 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token4), ==, HCL_TOKEN_TYPE_NUMBER);
  g_assert_cmpstr (hcl_token_get_value (token4), ==, "1.5e10");
}

static void
test_lexer_identifiers_and_bools (void)
{
  const gchar *input = "identifier true false another_identifier";
  g_autoptr(HclLexer) lexer = hcl_lexer_new (input);
  g_autoptr(GError) error = NULL;

  g_autoptr(HclToken) token1 = hcl_lexer_next_token (lexer, &error);
  g_assert_no_error (error);
  g_assert_cmpint (hcl_token_get_token_type (token1), ==, HCL_TOKEN_TYPE_IDENTIFIER);
  g_assert_cmpstr (hcl_token_get_value (token1), ==, "identifier");

  g_autoptr(HclToken) token2 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token2), ==, HCL_TOKEN_TYPE_BOOL);
  g_assert_cmpstr (hcl_token_get_value (token2), ==, "true");

  g_autoptr(HclToken) token3 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token3), ==, HCL_TOKEN_TYPE_BOOL);
  g_assert_cmpstr (hcl_token_get_value (token3), ==, "false");

  g_autoptr(HclToken) token4 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token4), ==, HCL_TOKEN_TYPE_IDENTIFIER);
  g_assert_cmpstr (hcl_token_get_value (token4), ==, "another_identifier");
}

static void
test_lexer_comments (void)
{
  const gchar *input = "# Hash comment\n// Slash comment\nidentifier";
  g_autoptr(HclLexer) lexer = hcl_lexer_new (input);
  g_autoptr(GError) error = NULL;

  g_autoptr(HclToken) token1 = hcl_lexer_next_token (lexer, &error);
  g_assert_no_error (error);
  g_assert_cmpint (hcl_token_get_token_type (token1), ==, HCL_TOKEN_TYPE_COMMENT);
  g_assert_cmpstr (hcl_token_get_value (token1), ==, " Hash comment");

  g_autoptr(HclToken) token2 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token2), ==, HCL_TOKEN_TYPE_NEWLINE);

  g_autoptr(HclToken) token3 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token3), ==, HCL_TOKEN_TYPE_COMMENT);
  g_assert_cmpstr (hcl_token_get_value (token3), ==, " Slash comment");

  g_autoptr(HclToken) token4 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token4), ==, HCL_TOKEN_TYPE_NEWLINE);

  g_autoptr(HclToken) token5 = hcl_lexer_next_token (lexer, &error);
  g_assert_cmpint (hcl_token_get_token_type (token5), ==, HCL_TOKEN_TYPE_IDENTIFIER);
  g_assert_cmpstr (hcl_token_get_value (token5), ==, "identifier");
}

int
main (int argc, char **argv)
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/hcl/lexer/basic_tokens", test_lexer_basic_tokens);
  g_test_add_func ("/hcl/lexer/strings", test_lexer_strings);
  g_test_add_func ("/hcl/lexer/numbers", test_lexer_numbers);
  g_test_add_func ("/hcl/lexer/identifiers_and_bools", test_lexer_identifiers_and_bools);
  g_test_add_func ("/hcl/lexer/comments", test_lexer_comments);

  return g_test_run ();
}

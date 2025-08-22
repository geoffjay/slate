/* hcl-parser.c - HCL parser implementation
 *
 * Copyright 2024 Geoff Johnson <geoff.jay@gmail.com>
 */

#include "hcl-parser.h"
#include "hcl-lexer.h"
#include <errno.h>

/**
 * SECTION:hcl-parser
 * @short_description: HCL parser
 * @title: HclParser
 *
 * #HclParser parses HCL configuration text into a document tree.
 */

struct _HclParser
{
  GObject parent_instance;

  HclLexer *lexer;
  HclToken *current_token;
};

G_DEFINE_FINAL_TYPE (HclParser, hcl_parser, G_TYPE_OBJECT)

static void
hcl_parser_finalize (GObject *object)
{
  HclParser *self = HCL_PARSER (object);

  if (self->lexer)
    g_object_unref (self->lexer);

  if (self->current_token)
    g_object_unref (self->current_token);

  G_OBJECT_CLASS (hcl_parser_parent_class)->finalize (object);
}

static void
hcl_parser_class_init (HclParserClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = hcl_parser_finalize;
}

static void
hcl_parser_init (HclParser *self)
{
  (void)self; /* Suppress unused parameter warning */
}

/**
 * hcl_parser_new:
 *
 * Creates a new HCL parser.
 *
 * Returns: (transfer full): a new #HclParser
 */
HclParser *
hcl_parser_new (void)
{
  return g_object_new (HCL_TYPE_PARSER, NULL);
}

static gboolean
hcl_parser_advance (HclParser *parser, GError **error)
{
  if (parser->current_token)
    g_object_unref (parser->current_token);

  parser->current_token = hcl_lexer_next_token (parser->lexer, error);
  return parser->current_token != NULL;
}

static gboolean
hcl_parser_match (HclParser *parser, HclTokenType type)
{
  if (!parser->current_token)
    return FALSE;

  return hcl_token_get_token_type (parser->current_token) == type;
}

static gboolean
hcl_parser_consume (HclParser *parser, HclTokenType type, GError **error)
{
  if (!hcl_parser_match (parser, type)) {
    g_set_error (error, HCL_PARSER_ERROR, HCL_PARSER_ERROR_UNEXPECTED_TOKEN,
                 "Expected token type %d but got %d at line %zu, column %zu",
                 type,
                 parser->current_token ? (int)hcl_token_get_token_type (parser->current_token) : -1,
                 parser->current_token ? hcl_token_get_line (parser->current_token) : 0,
                 parser->current_token ? hcl_token_get_column (parser->current_token) : 0);
    return FALSE;
  }

  return hcl_parser_advance (parser, error);
}

static void
hcl_parser_skip_newlines (HclParser *parser, GError **error)
{
  while (parser->current_token &&
         hcl_parser_match (parser, HCL_TOKEN_TYPE_NEWLINE)) {
    hcl_parser_advance (parser, error);
  }
}

static HclValue *hcl_parser_parse_value (HclParser *parser, GError **error);

static HclValue *
hcl_parser_parse_list (HclParser *parser, GError **error)
{
  HclValue *list = hcl_value_new_list ();

  if (!hcl_parser_consume (parser, HCL_TOKEN_TYPE_LBRACKET, error)) {
    g_object_unref (list);
    return NULL;
  }

  hcl_parser_skip_newlines (parser, error);

  while (parser->current_token &&
         !hcl_parser_match (parser, HCL_TOKEN_TYPE_RBRACKET)) {
    HclValue *item = hcl_parser_parse_value (parser, error);
    if (!item) {
      g_object_unref (list);
      return NULL;
    }

    hcl_value_list_add_item (list, item);

    hcl_parser_skip_newlines (parser, error);

    if (hcl_parser_match (parser, HCL_TOKEN_TYPE_COMMA)) {
      hcl_parser_advance (parser, error);
      hcl_parser_skip_newlines (parser, error);
    }
  }

  if (!hcl_parser_consume (parser, HCL_TOKEN_TYPE_RBRACKET, error)) {
    g_object_unref (list);
    return NULL;
  }

  return list;
}

static HclValue *
hcl_parser_parse_object (HclParser *parser, GError **error)
{
  HclValue *object = hcl_value_new_object ();

  if (!hcl_parser_consume (parser, HCL_TOKEN_TYPE_LBRACE, error)) {
    g_object_unref (object);
    return NULL;
  }

  hcl_parser_skip_newlines (parser, error);

  while (parser->current_token &&
         !hcl_parser_match (parser, HCL_TOKEN_TYPE_RBRACE)) {
    /* Parse key */
    if (!hcl_parser_match (parser, HCL_TOKEN_TYPE_IDENTIFIER) &&
        !hcl_parser_match (parser, HCL_TOKEN_TYPE_STRING)) {
      g_set_error (error, HCL_PARSER_ERROR, HCL_PARSER_ERROR_SYNTAX,
                   "Expected identifier or string for object key");
      g_object_unref (object);
      return NULL;
    }

    const gchar *key = hcl_token_get_value (parser->current_token);
    gchar *key_copy = g_strdup (key);

    hcl_parser_advance (parser, error);

    if (!hcl_parser_consume (parser, HCL_TOKEN_TYPE_ASSIGN, error)) {
      g_free (key_copy);
      g_object_unref (object);
      return NULL;
    }

    HclValue *value = hcl_parser_parse_value (parser, error);
    if (!value) {
      g_free (key_copy);
      g_object_unref (object);
      return NULL;
    }

    hcl_value_object_set_member (object, key_copy, value);
    g_free (key_copy);

    hcl_parser_skip_newlines (parser, error);

    if (hcl_parser_match (parser, HCL_TOKEN_TYPE_COMMA)) {
      hcl_parser_advance (parser, error);
      hcl_parser_skip_newlines (parser, error);
    }
  }

  if (!hcl_parser_consume (parser, HCL_TOKEN_TYPE_RBRACE, error)) {
    g_object_unref (object);
    return NULL;
  }

  return object;
}

static HclValue *
hcl_parser_parse_value (HclParser *parser, GError **error)
{
  if (!parser->current_token) {
    g_set_error (error, HCL_PARSER_ERROR, HCL_PARSER_ERROR_MISSING_VALUE,
                 "Expected value but reached end of input");
    return NULL;
  }

  HclTokenType type = hcl_token_get_token_type (parser->current_token);
  const gchar *value = hcl_token_get_value (parser->current_token);

  switch (type) {
    case HCL_TOKEN_TYPE_STRING: {
      HclValue *val = hcl_value_new_string (value);
      hcl_parser_advance (parser, error);
      return val;
    }

    case HCL_TOKEN_TYPE_NUMBER: {
      HclValue *val;
      if (strchr (value, '.') || strchr (value, 'e') || strchr (value, 'E')) {
        gdouble num = g_ascii_strtod (value, NULL);
        val = hcl_value_new_double (num);
      } else {
        gint64 num = g_ascii_strtoll (value, NULL, 10);
        val = hcl_value_new_int (num);
      }
      hcl_parser_advance (parser, error);
      return val;
    }

    case HCL_TOKEN_TYPE_BOOL: {
      gboolean bool_val = g_strcmp0 (value, "true") == 0;
      HclValue *val = hcl_value_new_bool (bool_val);
      hcl_parser_advance (parser, error);
      return val;
    }

    case HCL_TOKEN_TYPE_IDENTIFIER: {
      HclValue *val = hcl_value_new_string (value);
      hcl_parser_advance (parser, error);
      return val;
    }

    case HCL_TOKEN_TYPE_LBRACKET:
      return hcl_parser_parse_list (parser, error);

    case HCL_TOKEN_TYPE_LBRACE:
      return hcl_parser_parse_object (parser, error);

    default:
      g_set_error (error, HCL_PARSER_ERROR, HCL_PARSER_ERROR_SYNTAX,
                   "Unexpected token for value at line %zu, column %zu",
                   hcl_token_get_line (parser->current_token),
                   hcl_token_get_column (parser->current_token));
      return NULL;
  }
}

static gboolean
hcl_parser_parse_attribute (HclParser *parser, HclDocument *document, GError **error)
{
  if (!hcl_parser_match (parser, HCL_TOKEN_TYPE_IDENTIFIER)) {
    return FALSE;
  }

  const gchar *name = hcl_token_get_value (parser->current_token);
  gchar *name_copy = g_strdup (name);

  hcl_parser_advance (parser, error);

  if (!hcl_parser_consume (parser, HCL_TOKEN_TYPE_ASSIGN, error)) {
    g_free (name_copy);
    return FALSE;
  }

  HclValue *value = hcl_parser_parse_value (parser, error);
  if (!value) {
    g_free (name_copy);
    return FALSE;
  }

  hcl_document_set_attribute (document, name_copy, value);
  g_free (name_copy);

  return TRUE;
}

static gboolean
hcl_parser_parse_block (HclParser *parser, HclDocument *document, GError **error)
{
  if (!hcl_parser_match (parser, HCL_TOKEN_TYPE_IDENTIFIER)) {
    return FALSE;
  }

  const gchar *type = hcl_token_get_value (parser->current_token);
  gchar *type_copy = g_strdup (type);

  hcl_parser_advance (parser, error);

  gchar *label = NULL;
  if (hcl_parser_match (parser, HCL_TOKEN_TYPE_STRING) ||
      hcl_parser_match (parser, HCL_TOKEN_TYPE_IDENTIFIER)) {
    label = g_strdup (hcl_token_get_value (parser->current_token));
    hcl_parser_advance (parser, error);
  }

  HclBlock *block = hcl_block_new (type_copy, label);

  if (!hcl_parser_consume (parser, HCL_TOKEN_TYPE_LBRACE, error)) {
    g_free (type_copy);
    g_free (label);
    g_object_unref (block);
    return FALSE;
  }

  hcl_parser_skip_newlines (parser, error);

  while (parser->current_token &&
         !hcl_parser_match (parser, HCL_TOKEN_TYPE_RBRACE)) {

    /* Check for nested block */
    if (hcl_parser_match (parser, HCL_TOKEN_TYPE_IDENTIFIER)) {
      HclToken *peek = hcl_lexer_peek_token (parser->lexer, error);
      gboolean is_block = FALSE;

      if (peek) {
        HclTokenType peek_type = hcl_token_get_token_type (peek);
        is_block = (peek_type == HCL_TOKEN_TYPE_STRING ||
                   peek_type == HCL_TOKEN_TYPE_IDENTIFIER ||
                   peek_type == HCL_TOKEN_TYPE_LBRACE);

        /* If next token is assignment, it's an attribute */
        if (peek_type == HCL_TOKEN_TYPE_ASSIGN) {
          is_block = FALSE;
        }
      }

      if (is_block) {
        /* Parse nested block - create a temporary document to hold it */
        HclDocument *temp_doc = hcl_document_new ();
        if (!hcl_parser_parse_block (parser, temp_doc, error)) {
          g_object_unref (temp_doc);
          g_free (type_copy);
          g_free (label);
          g_object_unref (block);
          return FALSE;
        }

        /* Move the block from temp document to our block */
        GList *blocks = hcl_document_get_blocks (temp_doc);
        if (blocks) {
          hcl_block_add_block (block, g_object_ref (blocks->data));
        }

        g_list_free (blocks);
        g_object_unref (temp_doc);
      } else {
        /* Parse attribute */
        const gchar *attr_name = hcl_token_get_value (parser->current_token);
        gchar *attr_name_copy = g_strdup (attr_name);

        hcl_parser_advance (parser, error);

        if (!hcl_parser_consume (parser, HCL_TOKEN_TYPE_ASSIGN, error)) {
          g_free (attr_name_copy);
          g_free (type_copy);
          g_free (label);
          g_object_unref (block);
          return FALSE;
        }

        HclValue *value = hcl_parser_parse_value (parser, error);
        if (!value) {
          g_free (attr_name_copy);
          g_free (type_copy);
          g_free (label);
          g_object_unref (block);
          return FALSE;
        }

        hcl_block_set_attribute (block, attr_name_copy, value);
        g_free (attr_name_copy);
      }
    } else {
      g_set_error (error, HCL_PARSER_ERROR, HCL_PARSER_ERROR_SYNTAX,
                   "Expected identifier in block body");
      g_free (type_copy);
      g_free (label);
      g_object_unref (block);
      return FALSE;
    }

    hcl_parser_skip_newlines (parser, error);
  }

  if (!hcl_parser_consume (parser, HCL_TOKEN_TYPE_RBRACE, error)) {
    g_free (type_copy);
    g_free (label);
    g_object_unref (block);
    return FALSE;
  }

  hcl_document_add_block (document, block);

  g_free (type_copy);
  g_free (label);
  return TRUE;
}

/**
 * hcl_parser_parse_string:
 * @parser: an #HclParser
 * @input: HCL input string
 * @error: return location for error
 *
 * Parses an HCL string into a document.
 *
 * Returns: (transfer full) (nullable): parsed document or %NULL on error
 */
HclDocument *
hcl_parser_parse_string (HclParser *parser, const gchar *input, GError **error)
{
  g_return_val_if_fail (HCL_IS_PARSER (parser), NULL);
  g_return_val_if_fail (input != NULL, NULL);

  /* Clean up previous state */
  if (parser->lexer)
    g_object_unref (parser->lexer);
  if (parser->current_token)
    g_object_unref (parser->current_token);

  parser->lexer = hcl_lexer_new (input);
  parser->current_token = NULL;

  /* Initialize first token */
  if (!hcl_parser_advance (parser, error)) {
    return NULL;
  }

  HclDocument *document = hcl_document_new ();

  hcl_parser_skip_newlines (parser, error);

  while (parser->current_token &&
         !hcl_parser_match (parser, HCL_TOKEN_TYPE_EOF)) {

    /* Skip comments */
    if (hcl_parser_match (parser, HCL_TOKEN_TYPE_COMMENT)) {
      hcl_parser_advance (parser, error);
      hcl_parser_skip_newlines (parser, error);
      continue;
    }

    /* Try to parse as block first */
    if (hcl_parser_match (parser, HCL_TOKEN_TYPE_IDENTIFIER)) {
      HclToken *peek = hcl_lexer_peek_token (parser->lexer, error);
      gboolean is_block = FALSE;

      if (peek) {
        HclTokenType peek_type = hcl_token_get_token_type (peek);
        is_block = (peek_type == HCL_TOKEN_TYPE_STRING ||
                   peek_type == HCL_TOKEN_TYPE_IDENTIFIER ||
                   peek_type == HCL_TOKEN_TYPE_LBRACE);

        /* If next token is assignment, it's an attribute */
        if (peek_type == HCL_TOKEN_TYPE_ASSIGN) {
          is_block = FALSE;
        }
      }

      if (is_block) {
        if (!hcl_parser_parse_block (parser, document, error)) {
          g_object_unref (document);
          return NULL;
        }
      } else {
        if (!hcl_parser_parse_attribute (parser, document, error)) {
          g_object_unref (document);
          return NULL;
        }
      }
    } else {
      g_set_error (error, HCL_PARSER_ERROR, HCL_PARSER_ERROR_SYNTAX,
                   "Expected identifier at top level");
      g_object_unref (document);
      return NULL;
    }

    hcl_parser_skip_newlines (parser, error);
  }

  return document;
}

/**
 * hcl_parser_parse_file:
 * @parser: an #HclParser
 * @filename: path to HCL file
 * @error: return location for error
 *
 * Parses an HCL file into a document.
 *
 * Returns: (transfer full) (nullable): parsed document or %NULL on error
 */
HclDocument *
hcl_parser_parse_file (HclParser *parser, const gchar *filename, GError **error)
{
  g_return_val_if_fail (HCL_IS_PARSER (parser), NULL);
  g_return_val_if_fail (filename != NULL, NULL);

  gchar *contents;
  if (!g_file_get_contents (filename, &contents, NULL, error)) {
    return NULL;
  }

  HclDocument *document = hcl_parser_parse_string (parser, contents, error);
  g_free (contents);

  return document;
}

/**
 * hcl_parse_string:
 * @input: HCL input string
 * @error: return location for error
 *
 * Convenience function to parse an HCL string.
 *
 * Returns: (transfer full) (nullable): parsed document or %NULL on error
 */
HclDocument *
hcl_parse_string (const gchar *input, GError **error)
{
  g_autoptr(HclParser) parser = hcl_parser_new ();
  return hcl_parser_parse_string (parser, input, error);
}

/**
 * hcl_parse_file:
 * @filename: path to HCL file
 * @error: return location for error
 *
 * Convenience function to parse an HCL file.
 *
 * Returns: (transfer full) (nullable): parsed document or %NULL on error
 */
HclDocument *
hcl_parse_file (const gchar *filename, GError **error)
{
  g_autoptr(HclParser) parser = hcl_parser_new ();
  return hcl_parser_parse_file (parser, filename, error);
}

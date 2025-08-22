/* hcl-lexer.c - HCL lexer implementation
 *
 * Copyright 2024 Geoff Johnson <geoff.jay@gmail.com>
 */

#include "hcl-lexer.h"
#include <string.h>
#include <ctype.h>

/**
 * SECTION:hcl-lexer
 * @short_description: HCL lexical analyzer
 * @title: HclLexer
 *
 * #HclLexer tokenizes HCL input text.
 */

/* Token implementation */
struct _HclToken
{
  GObject parent_instance;

  HclTokenType type;
  gchar *value;
  gsize line;
  gsize column;
};

G_DEFINE_FINAL_TYPE (HclToken, hcl_token, G_TYPE_OBJECT)

static void
hcl_token_finalize (GObject *object)
{
  HclToken *self = HCL_TOKEN (object);

  g_free (self->value);

  G_OBJECT_CLASS (hcl_token_parent_class)->finalize (object);
}

static void
hcl_token_class_init (HclTokenClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = hcl_token_finalize;
}

static void
hcl_token_init (HclToken *self)
{
  (void)self; /* Suppress unused parameter warning */
}

/**
 * hcl_token_new:
 * @type: token type
 * @value: token value
 * @line: line number
 * @column: column number
 *
 * Creates a new HCL token.
 *
 * Returns: (transfer full): a new #HclToken
 */
HclToken *
hcl_token_new (HclTokenType type, const gchar *value, gsize line, gsize column)
{
  HclToken *self = g_object_new (HCL_TYPE_TOKEN, NULL);

  self->type = type;
  self->value = g_strdup (value);
  self->line = line;
  self->column = column;

  return self;
}

/**
 * hcl_token_get_token_type:
 * @token: an #HclToken
 *
 * Gets the token type.
 *
 * Returns: the token type
 */
HclTokenType
hcl_token_get_token_type (HclToken *token)
{
  g_return_val_if_fail (HCL_IS_TOKEN (token), HCL_TOKEN_TYPE_INVALID);

  return token->type;
}

/**
 * hcl_token_get_value:
 * @token: an #HclToken
 *
 * Gets the token value.
 *
 * Returns: the token value
 */
const gchar *
hcl_token_get_value (HclToken *token)
{
  g_return_val_if_fail (HCL_IS_TOKEN (token), NULL);

  return token->value;
}

/**
 * hcl_token_get_line:
 * @token: an #HclToken
 *
 * Gets the token line number.
 *
 * Returns: the line number
 */
gsize
hcl_token_get_line (HclToken *token)
{
  g_return_val_if_fail (HCL_IS_TOKEN (token), 0);

  return token->line;
}

/**
 * hcl_token_get_column:
 * @token: an #HclToken
 *
 * Gets the token column number.
 *
 * Returns: the column number
 */
gsize
hcl_token_get_column (HclToken *token)
{
  g_return_val_if_fail (HCL_IS_TOKEN (token), 0);

  return token->column;
}

/* Lexer implementation */
struct _HclLexer
{
  GObject parent_instance;

  const gchar *input;
  gsize input_length;
  gsize position;
  gsize line;
  gsize column;

  HclToken *peeked_token;
};

G_DEFINE_FINAL_TYPE (HclLexer, hcl_lexer, G_TYPE_OBJECT)

static void
hcl_lexer_finalize (GObject *object)
{
  HclLexer *self = HCL_LEXER (object);

  if (self->peeked_token)
    g_object_unref (self->peeked_token);

  G_OBJECT_CLASS (hcl_lexer_parent_class)->finalize (object);
}

static void
hcl_lexer_class_init (HclLexerClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = hcl_lexer_finalize;
}

static void
hcl_lexer_init (HclLexer *self)
{
  self->line = 1;
  self->column = 1;
}

/**
 * hcl_lexer_new:
 * @input: input text to tokenize
 *
 * Creates a new HCL lexer.
 *
 * Returns: (transfer full): a new #HclLexer
 */
HclLexer *
hcl_lexer_new (const gchar *input)
{
  HclLexer *self = g_object_new (HCL_TYPE_LEXER, NULL);

  self->input = input;
  self->input_length = strlen (input);

  return self;
}

static gchar
hcl_lexer_current_char (HclLexer *lexer)
{
  if (lexer->position >= lexer->input_length)
    return '\0';
  return lexer->input[lexer->position];
}

static gchar
hcl_lexer_peek_char (HclLexer *lexer, gsize offset)
{
  gsize pos = lexer->position + offset;
  if (pos >= lexer->input_length)
    return '\0';
  return lexer->input[pos];
}

static void
hcl_lexer_advance (HclLexer *lexer)
{
  if (lexer->position < lexer->input_length) {
    if (lexer->input[lexer->position] == '\n') {
      lexer->line++;
      lexer->column = 1;
    } else {
      lexer->column++;
    }
    lexer->position++;
  }
}

static void
hcl_lexer_skip_whitespace (HclLexer *lexer)
{
  while (lexer->position < lexer->input_length) {
    gchar c = hcl_lexer_current_char (lexer);
    if (c == ' ' || c == '\t' || c == '\r') {
      hcl_lexer_advance (lexer);
    } else {
      break;
    }
  }
}

static gboolean
hcl_lexer_is_identifier_char (gchar c)
{
  return isalnum (c) || c == '_' || c == '-';
}

static HclToken *
hcl_lexer_read_string (HclLexer *lexer, GError **error)
{
  gsize start_line = lexer->line;
  gsize start_column = lexer->column;
  GString *value = g_string_new (NULL);
  gchar quote_char = hcl_lexer_current_char (lexer);

  hcl_lexer_advance (lexer); /* Skip opening quote */

  while (lexer->position < lexer->input_length) {
    gchar c = hcl_lexer_current_char (lexer);

    if (c == quote_char) {
      hcl_lexer_advance (lexer); /* Skip closing quote */
      HclToken *token = hcl_token_new (HCL_TOKEN_TYPE_STRING, value->str,
                                       start_line, start_column);
      g_string_free (value, TRUE);
      return token;
    }

    if (c == '\\') {
      hcl_lexer_advance (lexer);
      if (lexer->position >= lexer->input_length) {
        g_set_error (error, HCL_PARSER_ERROR, HCL_PARSER_ERROR_UNTERMINATED_STRING,
                     "Unterminated string at line %zu, column %zu",
                     start_line, start_column);
        g_string_free (value, TRUE);
        return NULL;
      }

      gchar escaped = hcl_lexer_current_char (lexer);
      switch (escaped) {
        case 'n':
          g_string_append_c (value, '\n');
          break;
        case 't':
          g_string_append_c (value, '\t');
          break;
        case 'r':
          g_string_append_c (value, '\r');
          break;
        case '\\':
          g_string_append_c (value, '\\');
          break;
        case '"':
          g_string_append_c (value, '"');
          break;
        case '\'':
          g_string_append_c (value, '\'');
          break;
        default:
          g_set_error (error, HCL_PARSER_ERROR, HCL_PARSER_ERROR_INVALID_ESCAPE,
                       "Invalid escape sequence '\\%c' at line %zu, column %zu",
                       escaped, lexer->line, lexer->column);
          g_string_free (value, TRUE);
          return NULL;
      }
      hcl_lexer_advance (lexer);
    } else {
      g_string_append_c (value, c);
      hcl_lexer_advance (lexer);
    }
  }

  g_set_error (error, HCL_PARSER_ERROR, HCL_PARSER_ERROR_UNTERMINATED_STRING,
               "Unterminated string at line %zu, column %zu",
               start_line, start_column);
  g_string_free (value, TRUE);
  return NULL;
}

static HclToken *
hcl_lexer_read_number (HclLexer *lexer)
{
  gsize start_line = lexer->line;
  gsize start_column = lexer->column;
  GString *value = g_string_new (NULL);

  while (lexer->position < lexer->input_length) {
    gchar c = hcl_lexer_current_char (lexer);
    if (isdigit (c) || c == '.' || c == '-' || c == '+' ||
        c == 'e' || c == 'E') {
      g_string_append_c (value, c);
      hcl_lexer_advance (lexer);
    } else {
      break;
    }
  }

  HclToken *token = hcl_token_new (HCL_TOKEN_TYPE_NUMBER, value->str,
                                   start_line, start_column);
  g_string_free (value, TRUE);
  return token;
}

static HclToken *
hcl_lexer_read_identifier (HclLexer *lexer)
{
  gsize start_line = lexer->line;
  gsize start_column = lexer->column;
  GString *value = g_string_new (NULL);

  while (lexer->position < lexer->input_length) {
    gchar c = hcl_lexer_current_char (lexer);
    if (hcl_lexer_is_identifier_char (c)) {
      g_string_append_c (value, c);
      hcl_lexer_advance (lexer);
    } else {
      break;
    }
  }

  HclTokenType type = HCL_TOKEN_TYPE_IDENTIFIER;

  /* Check for boolean literals */
  if (g_strcmp0 (value->str, "true") == 0 || g_strcmp0 (value->str, "false") == 0) {
    type = HCL_TOKEN_TYPE_BOOL;
  }

  HclToken *token = hcl_token_new (type, value->str, start_line, start_column);
  g_string_free (value, TRUE);
  return token;
}

static HclToken *
hcl_lexer_read_comment (HclLexer *lexer)
{
  gsize start_line = lexer->line;
  gsize start_column = lexer->column;
  GString *value = g_string_new (NULL);

  /* Skip initial # or // */
  if (hcl_lexer_current_char (lexer) == '#') {
    hcl_lexer_advance (lexer);
  } else if (hcl_lexer_current_char (lexer) == '/' &&
             hcl_lexer_peek_char (lexer, 1) == '/') {
    hcl_lexer_advance (lexer);
    hcl_lexer_advance (lexer);
  }

  while (lexer->position < lexer->input_length) {
    gchar c = hcl_lexer_current_char (lexer);
    if (c == '\n') {
      break;
    }
    g_string_append_c (value, c);
    hcl_lexer_advance (lexer);
  }

  HclToken *token = hcl_token_new (HCL_TOKEN_TYPE_COMMENT, value->str,
                                   start_line, start_column);
  g_string_free (value, TRUE);
  return token;
}

/**
 * hcl_lexer_next_token:
 * @lexer: an #HclLexer
 * @error: return location for error
 *
 * Gets the next token from the lexer.
 *
 * Returns: (transfer full) (nullable): the next token or %NULL on error
 */
HclToken *
hcl_lexer_next_token (HclLexer *lexer, GError **error)
{
  g_return_val_if_fail (HCL_IS_LEXER (lexer), NULL);

  /* Return peeked token if available */
  if (lexer->peeked_token) {
    HclToken *token = lexer->peeked_token;
    lexer->peeked_token = NULL;
    return token;
  }

  hcl_lexer_skip_whitespace (lexer);

  if (lexer->position >= lexer->input_length) {
    return hcl_token_new (HCL_TOKEN_TYPE_EOF, "", lexer->line, lexer->column);
  }

  gchar c = hcl_lexer_current_char (lexer);
  gsize line = lexer->line;
  gsize column = lexer->column;

  switch (c) {
    case '\n':
      hcl_lexer_advance (lexer);
      return hcl_token_new (HCL_TOKEN_TYPE_NEWLINE, "\n", line, column);

    case '=':
      hcl_lexer_advance (lexer);
      return hcl_token_new (HCL_TOKEN_TYPE_ASSIGN, "=", line, column);

    case '{':
      hcl_lexer_advance (lexer);
      return hcl_token_new (HCL_TOKEN_TYPE_LBRACE, "{", line, column);

    case '}':
      hcl_lexer_advance (lexer);
      return hcl_token_new (HCL_TOKEN_TYPE_RBRACE, "}", line, column);

    case '[':
      hcl_lexer_advance (lexer);
      return hcl_token_new (HCL_TOKEN_TYPE_LBRACKET, "[", line, column);

    case ']':
      hcl_lexer_advance (lexer);
      return hcl_token_new (HCL_TOKEN_TYPE_RBRACKET, "]", line, column);

    case '(':
      hcl_lexer_advance (lexer);
      return hcl_token_new (HCL_TOKEN_TYPE_LPAREN, "(", line, column);

    case ')':
      hcl_lexer_advance (lexer);
      return hcl_token_new (HCL_TOKEN_TYPE_RPAREN, ")", line, column);

    case ',':
      hcl_lexer_advance (lexer);
      return hcl_token_new (HCL_TOKEN_TYPE_COMMA, ",", line, column);

    case '"':
    case '\'':
      return hcl_lexer_read_string (lexer, error);

    case '#':
      return hcl_lexer_read_comment (lexer);

    case '/':
      if (hcl_lexer_peek_char (lexer, 1) == '/') {
        return hcl_lexer_read_comment (lexer);
      }
      /* Fall through for division or other uses */
      break;

    default:
      if (isdigit (c) || (c == '-' && isdigit (hcl_lexer_peek_char (lexer, 1)))) {
        return hcl_lexer_read_number (lexer);
      } else if (isalpha (c) || c == '_') {
        return hcl_lexer_read_identifier (lexer);
      }
      break;
  }

  g_set_error (error, HCL_PARSER_ERROR, HCL_PARSER_ERROR_SYNTAX,
               "Unexpected character '%c' at line %zu, column %zu",
               c, line, column);
  return NULL;
}

/**
 * hcl_lexer_peek_token:
 * @lexer: an #HclLexer
 * @error: return location for error
 *
 * Peeks at the next token without consuming it.
 *
 * Returns: (transfer none) (nullable): the next token or %NULL on error
 */
HclToken *
hcl_lexer_peek_token (HclLexer *lexer, GError **error)
{
  g_return_val_if_fail (HCL_IS_LEXER (lexer), NULL);

  if (!lexer->peeked_token) {
    lexer->peeked_token = hcl_lexer_next_token (lexer, error);
  }

  return lexer->peeked_token;
}

/**
 * hcl_lexer_at_end:
 * @lexer: an #HclLexer
 *
 * Checks if the lexer is at the end of input.
 *
 * Returns: %TRUE if at end of input
 */
gboolean
hcl_lexer_at_end (HclLexer *lexer)
{
  g_return_val_if_fail (HCL_IS_LEXER (lexer), TRUE);

  return lexer->position >= lexer->input_length;
}

/**
 * hcl_lexer_get_line:
 * @lexer: an #HclLexer
 *
 * Gets the current line number.
 *
 * Returns: the line number
 */
gsize
hcl_lexer_get_line (HclLexer *lexer)
{
  g_return_val_if_fail (HCL_IS_LEXER (lexer), 0);

  return lexer->line;
}

/**
 * hcl_lexer_get_column:
 * @lexer: an #HclLexer
 *
 * Gets the current column number.
 *
 * Returns: the column number
 */
gsize
hcl_lexer_get_column (HclLexer *lexer)
{
  g_return_val_if_fail (HCL_IS_LEXER (lexer), 0);

  return lexer->column;
}

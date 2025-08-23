/* hcl-enums.h - Enumerations for libghcl
 *
 * Copyright 2024 Geoff Johnson <geoff.jay@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */

#ifndef __HCL_ENUMS_H__
#define __HCL_ENUMS_H__

#include <glib-object.h>

G_BEGIN_DECLS

/**
 * HclValueType:
 * @HCL_VALUE_TYPE_NULL: Null value
 * @HCL_VALUE_TYPE_BOOL: Boolean value
 * @HCL_VALUE_TYPE_NUMBER: Numeric value (integer or float)
 * @HCL_VALUE_TYPE_STRING: String value
 * @HCL_VALUE_TYPE_LIST: List/array value
 * @HCL_VALUE_TYPE_OBJECT: Object/map value
 *
 * The type of an HCL value.
 */
typedef enum {
  HCL_VALUE_TYPE_NULL,
  HCL_VALUE_TYPE_BOOL,
  HCL_VALUE_TYPE_NUMBER,
  HCL_VALUE_TYPE_STRING,
  HCL_VALUE_TYPE_LIST,
  HCL_VALUE_TYPE_OBJECT
} HclValueType;

/**
 * HclNumberType:
 * @HCL_NUMBER_TYPE_INTEGER: Integer number
 * @HCL_NUMBER_TYPE_FLOAT: Floating point number
 *
 * The type of numeric value.
 */
typedef enum {
  HCL_NUMBER_TYPE_INTEGER,
  HCL_NUMBER_TYPE_FLOAT
} HclNumberType;

/**
 * HclTokenType:
 * @HCL_TOKEN_TYPE_INVALID: Invalid token
 * @HCL_TOKEN_TYPE_EOF: End of file
 * @HCL_TOKEN_TYPE_IDENTIFIER: Identifier
 * @HCL_TOKEN_TYPE_STRING: String literal
 * @HCL_TOKEN_TYPE_NUMBER: Number literal
 * @HCL_TOKEN_TYPE_BOOL: Boolean literal
 * @HCL_TOKEN_TYPE_NULL: Null literal
 * @HCL_TOKEN_TYPE_ASSIGN: Assignment operator (=)
 * @HCL_TOKEN_TYPE_LBRACE: Left brace ({)
 * @HCL_TOKEN_TYPE_RBRACE: Right brace (})
 * @HCL_TOKEN_TYPE_LBRACKET: Left bracket ([)
 * @HCL_TOKEN_TYPE_RBRACKET: Right bracket (])
 * @HCL_TOKEN_TYPE_LPAREN: Left parenthesis (()
 * @HCL_TOKEN_TYPE_RPAREN: Right parenthesis ())
 * @HCL_TOKEN_TYPE_COMMA: Comma (,)
 * @HCL_TOKEN_TYPE_DOT: Dot (.)
 * @HCL_TOKEN_TYPE_NEWLINE: Newline
 * @HCL_TOKEN_TYPE_COMMENT: Comment
 *
 * Token types for the HCL lexer.
 */
typedef enum {
  HCL_TOKEN_TYPE_INVALID,
  HCL_TOKEN_TYPE_EOF,
  HCL_TOKEN_TYPE_IDENTIFIER,
  HCL_TOKEN_TYPE_STRING,
  HCL_TOKEN_TYPE_NUMBER,
  HCL_TOKEN_TYPE_BOOL,
  HCL_TOKEN_TYPE_NULL,
  HCL_TOKEN_TYPE_ASSIGN,
  HCL_TOKEN_TYPE_LBRACE,
  HCL_TOKEN_TYPE_RBRACE,
  HCL_TOKEN_TYPE_LBRACKET,
  HCL_TOKEN_TYPE_RBRACKET,
  HCL_TOKEN_TYPE_LPAREN,
  HCL_TOKEN_TYPE_RPAREN,
  HCL_TOKEN_TYPE_COMMA,
  HCL_TOKEN_TYPE_DOT,
  HCL_TOKEN_TYPE_NEWLINE,
  HCL_TOKEN_TYPE_COMMENT
} HclTokenType;

/**
 * HclParserError:
 * @HCL_PARSER_ERROR_SYNTAX: Syntax error
 * @HCL_PARSER_ERROR_UNEXPECTED_TOKEN: Unexpected token
 * @HCL_PARSER_ERROR_MISSING_VALUE: Missing value
 * @HCL_PARSER_ERROR_INVALID_ESCAPE: Invalid escape sequence
 * @HCL_PARSER_ERROR_UNTERMINATED_STRING: Unterminated string
 * @HCL_PARSER_ERROR_INVALID_NUMBER: Invalid number format
 * @HCL_PARSER_ERROR_INVALID_CHARACTER: Invalid character
 * @HCL_PARSER_ERROR_BUFFER_OVERFLOW: Buffer overflow
 *
 * Parser error codes.
 */
typedef enum {
  HCL_PARSER_ERROR_SYNTAX,
  HCL_PARSER_ERROR_UNEXPECTED_TOKEN,
  HCL_PARSER_ERROR_MISSING_VALUE,
  HCL_PARSER_ERROR_INVALID_ESCAPE,
  HCL_PARSER_ERROR_UNTERMINATED_STRING,
  HCL_PARSER_ERROR_INVALID_NUMBER,
  HCL_PARSER_ERROR_INVALID_CHARACTER,
  HCL_PARSER_ERROR_BUFFER_OVERFLOW
} HclParserError;

#define HCL_PARSER_ERROR hcl_parser_error_quark()
GQuark hcl_parser_error_quark (void);

G_END_DECLS

#endif /* __HCL_ENUMS_H__ */

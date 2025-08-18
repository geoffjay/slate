/* hcl-lexer.h - HCL lexer
 *
 * Copyright 2024 The Slate Project
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */

#ifndef __HCL_LEXER_H__
#define __HCL_LEXER_H__

#include <glib-object.h>
#include "hcl-enums.h"

G_BEGIN_DECLS

#define HCL_TYPE_TOKEN (hcl_token_get_type())
G_DECLARE_FINAL_TYPE (HclToken, hcl_token, HCL, TOKEN, GObject)

#define HCL_TYPE_LEXER (hcl_lexer_get_type())
G_DECLARE_FINAL_TYPE (HclLexer, hcl_lexer, HCL, LEXER, GObject)

/* Token API */
HclToken       *hcl_token_new              (HclTokenType type,
                                            const gchar *value,
                                            gsize line,
                                            gsize column);

HclTokenType    hcl_token_get_token_type   (HclToken *token);
const gchar    *hcl_token_get_value        (HclToken *token);
gsize           hcl_token_get_line         (HclToken *token);
gsize           hcl_token_get_column       (HclToken *token);

/* Lexer API */
HclLexer       *hcl_lexer_new              (const gchar *input);
HclToken       *hcl_lexer_next_token       (HclLexer *lexer,
                                            GError **error);
HclToken       *hcl_lexer_peek_token       (HclLexer *lexer,
                                            GError **error);
gboolean        hcl_lexer_at_end           (HclLexer *lexer);
gsize           hcl_lexer_get_line         (HclLexer *lexer);
gsize           hcl_lexer_get_column       (HclLexer *lexer);

G_END_DECLS

#endif /* __HCL_LEXER_H__ */

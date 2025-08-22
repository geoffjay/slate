/* hcl-parser.h - HCL parser
 *
 * Copyright 2024 Geoff Johnson <geoff.jay@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */

#ifndef __HCL_PARSER_H__
#define __HCL_PARSER_H__

#include <glib-object.h>
#include "hcl-document.h"
#include "hcl-enums.h"

G_BEGIN_DECLS

#define HCL_TYPE_PARSER (hcl_parser_get_type())
G_DECLARE_FINAL_TYPE (HclParser, hcl_parser, HCL, PARSER, GObject)

/* Constructor */
HclParser      *hcl_parser_new                  (void);

/* Parsing */
HclDocument    *hcl_parser_parse_string         (HclParser *parser,
                                                 const gchar *input,
                                                 GError **error);

HclDocument    *hcl_parser_parse_file           (HclParser *parser,
                                                 const gchar *filename,
                                                 GError **error);

/* Convenience functions */
HclDocument    *hcl_parse_string                (const gchar *input,
                                                 GError **error);

HclDocument    *hcl_parse_file                  (const gchar *filename,
                                                 GError **error);

G_END_DECLS

#endif /* __HCL_PARSER_H__ */

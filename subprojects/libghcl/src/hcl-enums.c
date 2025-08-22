/* hcl-enums.c - Enumerations implementation for libghcl
 *
 * Copyright 2024 Geoff Johnson <geoff.jay@gmail.com>
 */

#include "hcl-enums.h"

/**
 * hcl_parser_error_quark:
 *
 * Returns: the error quark for HCL parser errors
 */
GQuark
hcl_parser_error_quark (void)
{
  return g_quark_from_static_string ("hcl-parser-error-quark");
}

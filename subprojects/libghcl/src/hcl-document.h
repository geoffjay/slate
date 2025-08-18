/* hcl-document.h - HCL document structure
 *
 * Copyright 2024 The Slate Project
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */

#ifndef __HCL_DOCUMENT_H__
#define __HCL_DOCUMENT_H__

#include <glib-object.h>
#include "hcl-block.h"
#include "hcl-value.h"

G_BEGIN_DECLS

#define HCL_TYPE_DOCUMENT (hcl_document_get_type())
G_DECLARE_FINAL_TYPE (HclDocument, hcl_document, HCL, DOCUMENT, GObject)

/* Constructor */
HclDocument    *hcl_document_new                (void);

/* Attributes */
GList          *hcl_document_get_attribute_names(HclDocument *document);
HclValue       *hcl_document_get_attribute      (HclDocument *document,
                                                 const gchar *name);
void            hcl_document_set_attribute      (HclDocument *document,
                                                 const gchar *name,
                                                 HclValue *value);
gboolean        hcl_document_has_attribute      (HclDocument *document,
                                                 const gchar *name);

/* Blocks */
GList          *hcl_document_get_blocks         (HclDocument *document);
void            hcl_document_add_block          (HclDocument *document,
                                                 HclBlock *block);
GList          *hcl_document_get_blocks_by_type (HclDocument *document,
                                                 const gchar *type);

/* Utility */
gchar          *hcl_document_to_string          (HclDocument *document);

G_END_DECLS

#endif /* __HCL_DOCUMENT_H__ */

/* hcl-block.h - HCL block structure
 *
 * Copyright 2024 The Slate Project
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */

#ifndef __HCL_BLOCK_H__
#define __HCL_BLOCK_H__

#include <glib-object.h>
#include "hcl-value.h"

G_BEGIN_DECLS

#define HCL_TYPE_BLOCK (hcl_block_get_type())
G_DECLARE_FINAL_TYPE (HclBlock, hcl_block, HCL, BLOCK, GObject)

/* Constructor */
HclBlock       *hcl_block_new                   (const gchar *type,
                                                 const gchar *label);

/* Block properties */
const gchar    *hcl_block_get_block_type        (HclBlock *block);
const gchar    *hcl_block_get_label             (HclBlock *block);
void            hcl_block_set_label             (HclBlock *block,
                                                 const gchar *label);

/* Attributes */
GList          *hcl_block_get_attribute_names   (HclBlock *block);
HclValue       *hcl_block_get_attribute         (HclBlock *block,
                                                 const gchar *name);
void            hcl_block_set_attribute         (HclBlock *block,
                                                 const gchar *name,
                                                 HclValue *value);
gboolean        hcl_block_has_attribute         (HclBlock *block,
                                                 const gchar *name);

/* Nested blocks */
GList          *hcl_block_get_blocks            (HclBlock *block);
void            hcl_block_add_block             (HclBlock *block,
                                                 HclBlock *child);
GList          *hcl_block_get_blocks_by_type    (HclBlock *block,
                                                 const gchar *type);

/* Utility */
gchar          *hcl_block_to_string             (HclBlock *block);

G_END_DECLS

#endif /* __HCL_BLOCK_H__ */

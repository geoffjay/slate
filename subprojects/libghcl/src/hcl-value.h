/* hcl-value.h - HCL value types
 *
 * Copyright 2024 The Slate Project
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */

#ifndef __HCL_VALUE_H__
#define __HCL_VALUE_H__

#include <glib-object.h>
#include "hcl-enums.h"

G_BEGIN_DECLS

#define HCL_TYPE_VALUE (hcl_value_get_type())
G_DECLARE_FINAL_TYPE (HclValue, hcl_value, HCL, VALUE, GObject)

/* Constructors */
HclValue *hcl_value_new_null          (void);
HclValue *hcl_value_new_bool          (gboolean value);
HclValue *hcl_value_new_int           (gint64 value);
HclValue *hcl_value_new_double        (gdouble value);
HclValue *hcl_value_new_string        (const gchar *value);
HclValue *hcl_value_new_list          (void);
HclValue *hcl_value_new_object        (void);

/* Type checking */
HclValueType    hcl_value_get_value_type    (HclValue *value);
gboolean        hcl_value_is_null           (HclValue *value);
gboolean        hcl_value_is_bool           (HclValue *value);
gboolean        hcl_value_is_number         (HclValue *value);
gboolean        hcl_value_is_string         (HclValue *value);
gboolean        hcl_value_is_list           (HclValue *value);
gboolean        hcl_value_is_object         (HclValue *value);

/* Value getters */
gboolean        hcl_value_get_bool          (HclValue *value);
gint64          hcl_value_get_int           (HclValue *value);
gdouble         hcl_value_get_double        (HclValue *value);
const gchar    *hcl_value_get_string        (HclValue *value);

/* List operations */
guint           hcl_value_list_get_length   (HclValue *value);
HclValue       *hcl_value_list_get_item     (HclValue *value, guint index);
void            hcl_value_list_add_item     (HclValue *value, HclValue *item);

/* Object operations */
GList          *hcl_value_object_get_keys   (HclValue *value);
HclValue       *hcl_value_object_get_member (HclValue *value, const gchar *key);
void            hcl_value_object_set_member (HclValue *value, const gchar *key, HclValue *member);
gboolean        hcl_value_object_has_member (HclValue *value, const gchar *key);

/* Utility */
gchar          *hcl_value_to_string         (HclValue *value);
HclValue       *hcl_value_copy              (HclValue *value);

G_END_DECLS

#endif /* __HCL_VALUE_H__ */

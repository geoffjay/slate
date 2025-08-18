/* hcl-value.c - HCL value implementation
 *
 * Copyright 2024 The Slate Project
 */

#include "hcl-value.h"
#include <math.h>

/**
 * SECTION:hcl-value
 * @short_description: HCL value types
 * @title: HclValue
 *
 * #HclValue represents a value in HCL configuration.
 */

struct _HclValue
{
  GObject parent_instance;

  HclValueType type;

  union {
    gboolean bool_value;
    struct {
      HclNumberType number_type;
      union {
        gint64 int_value;
        gdouble double_value;
      };
    } number;
    gchar *string_value;
    GPtrArray *list_value;    /* Array of HclValue* */
    GHashTable *object_value; /* String -> HclValue* */
  } data;
};

G_DEFINE_FINAL_TYPE (HclValue, hcl_value, G_TYPE_OBJECT)

static void
hcl_value_finalize (GObject *object)
{
  HclValue *self = HCL_VALUE (object);

  switch (self->type) {
    case HCL_VALUE_TYPE_STRING:
      g_free (self->data.string_value);
      break;

    case HCL_VALUE_TYPE_LIST:
      if (self->data.list_value)
        g_ptr_array_unref (self->data.list_value);
      break;

    case HCL_VALUE_TYPE_OBJECT:
      if (self->data.object_value)
        g_hash_table_unref (self->data.object_value);
      break;

    default:
      break;
  }

  G_OBJECT_CLASS (hcl_value_parent_class)->finalize (object);
}

static void
hcl_value_class_init (HclValueClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = hcl_value_finalize;
}

static void
hcl_value_init (HclValue *self)
{
  self->type = HCL_VALUE_TYPE_NULL;
}

/**
 * hcl_value_new_null:
 *
 * Creates a new null HCL value.
 *
 * Returns: (transfer full): a new #HclValue
 */
HclValue *
hcl_value_new_null (void)
{
  return g_object_new (HCL_TYPE_VALUE, NULL);
}

/**
 * hcl_value_new_bool:
 * @value: boolean value
 *
 * Creates a new boolean HCL value.
 *
 * Returns: (transfer full): a new #HclValue
 */
HclValue *
hcl_value_new_bool (gboolean value)
{
  HclValue *self = g_object_new (HCL_TYPE_VALUE, NULL);
  self->type = HCL_VALUE_TYPE_BOOL;
  self->data.bool_value = value;
  return self;
}

/**
 * hcl_value_new_int:
 * @value: integer value
 *
 * Creates a new integer HCL value.
 *
 * Returns: (transfer full): a new #HclValue
 */
HclValue *
hcl_value_new_int (gint64 value)
{
  HclValue *self = g_object_new (HCL_TYPE_VALUE, NULL);
  self->type = HCL_VALUE_TYPE_NUMBER;
  self->data.number.number_type = HCL_NUMBER_TYPE_INTEGER;
  self->data.number.int_value = value;
  return self;
}

/**
 * hcl_value_new_double:
 * @value: double value
 *
 * Creates a new double HCL value.
 *
 * Returns: (transfer full): a new #HclValue
 */
HclValue *
hcl_value_new_double (gdouble value)
{
  HclValue *self = g_object_new (HCL_TYPE_VALUE, NULL);
  self->type = HCL_VALUE_TYPE_NUMBER;
  self->data.number.number_type = HCL_NUMBER_TYPE_FLOAT;
  self->data.number.double_value = value;
  return self;
}

/**
 * hcl_value_new_string:
 * @value: string value
 *
 * Creates a new string HCL value.
 *
 * Returns: (transfer full): a new #HclValue
 */
HclValue *
hcl_value_new_string (const gchar *value)
{
  HclValue *self = g_object_new (HCL_TYPE_VALUE, NULL);
  self->type = HCL_VALUE_TYPE_STRING;
  self->data.string_value = g_strdup (value);
  return self;
}

/**
 * hcl_value_new_list:
 *
 * Creates a new list HCL value.
 *
 * Returns: (transfer full): a new #HclValue
 */
HclValue *
hcl_value_new_list (void)
{
  HclValue *self = g_object_new (HCL_TYPE_VALUE, NULL);
  self->type = HCL_VALUE_TYPE_LIST;
  self->data.list_value = g_ptr_array_new_with_free_func (g_object_unref);
  return self;
}

/**
 * hcl_value_new_object:
 *
 * Creates a new object HCL value.
 *
 * Returns: (transfer full): a new #HclValue
 */
HclValue *
hcl_value_new_object (void)
{
  HclValue *self = g_object_new (HCL_TYPE_VALUE, NULL);
  self->type = HCL_VALUE_TYPE_OBJECT;
  self->data.object_value = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                   g_free, g_object_unref);
  return self;
}

/**
 * hcl_value_get_value_type:
 * @value: an #HclValue
 *
 * Gets the type of the HCL value.
 *
 * Returns: the #HclValueType
 */
HclValueType
hcl_value_get_value_type (HclValue *value)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), HCL_VALUE_TYPE_NULL);

  return value->type;
}

/**
 * hcl_value_is_null:
 * @value: an #HclValue
 *
 * Checks if the value is null.
 *
 * Returns: %TRUE if the value is null
 */
gboolean
hcl_value_is_null (HclValue *value)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), FALSE);

  return value->type == HCL_VALUE_TYPE_NULL;
}

/**
 * hcl_value_is_bool:
 * @value: an #HclValue
 *
 * Checks if the value is a boolean.
 *
 * Returns: %TRUE if the value is a boolean
 */
gboolean
hcl_value_is_bool (HclValue *value)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), FALSE);

  return value->type == HCL_VALUE_TYPE_BOOL;
}

/**
 * hcl_value_is_number:
 * @value: an #HclValue
 *
 * Checks if the value is a number.
 *
 * Returns: %TRUE if the value is a number
 */
gboolean
hcl_value_is_number (HclValue *value)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), FALSE);

  return value->type == HCL_VALUE_TYPE_NUMBER;
}

/**
 * hcl_value_is_string:
 * @value: an #HclValue
 *
 * Checks if the value is a string.
 *
 * Returns: %TRUE if the value is a string
 */
gboolean
hcl_value_is_string (HclValue *value)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), FALSE);

  return value->type == HCL_VALUE_TYPE_STRING;
}

/**
 * hcl_value_is_list:
 * @value: an #HclValue
 *
 * Checks if the value is a list.
 *
 * Returns: %TRUE if the value is a list
 */
gboolean
hcl_value_is_list (HclValue *value)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), FALSE);

  return value->type == HCL_VALUE_TYPE_LIST;
}

/**
 * hcl_value_is_object:
 * @value: an #HclValue
 *
 * Checks if the value is an object.
 *
 * Returns: %TRUE if the value is an object
 */
gboolean
hcl_value_is_object (HclValue *value)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), FALSE);

  return value->type == HCL_VALUE_TYPE_OBJECT;
}

/**
 * hcl_value_get_bool:
 * @value: an #HclValue
 *
 * Gets the boolean value.
 *
 * Returns: the boolean value
 */
gboolean
hcl_value_get_bool (HclValue *value)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), FALSE);
  g_return_val_if_fail (value->type == HCL_VALUE_TYPE_BOOL, FALSE);

  return value->data.bool_value;
}

/**
 * hcl_value_get_int:
 * @value: an #HclValue
 *
 * Gets the integer value.
 *
 * Returns: the integer value
 */
gint64
hcl_value_get_int (HclValue *value)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), 0);
  g_return_val_if_fail (value->type == HCL_VALUE_TYPE_NUMBER, 0);

  if (value->data.number.number_type == HCL_NUMBER_TYPE_INTEGER)
    return value->data.number.int_value;
  else
    return (gint64) value->data.number.double_value;
}

/**
 * hcl_value_get_double:
 * @value: an #HclValue
 *
 * Gets the double value.
 *
 * Returns: the double value
 */
gdouble
hcl_value_get_double (HclValue *value)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), 0.0);
  g_return_val_if_fail (value->type == HCL_VALUE_TYPE_NUMBER, 0.0);

  if (value->data.number.number_type == HCL_NUMBER_TYPE_FLOAT)
    return value->data.number.double_value;
  else
    return (gdouble) value->data.number.int_value;
}

/**
 * hcl_value_get_string:
 * @value: an #HclValue
 *
 * Gets the string value.
 *
 * Returns: the string value
 */
const gchar *
hcl_value_get_string (HclValue *value)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), NULL);
  g_return_val_if_fail (value->type == HCL_VALUE_TYPE_STRING, NULL);

  return value->data.string_value;
}

/**
 * hcl_value_list_get_length:
 * @value: an #HclValue
 *
 * Gets the length of a list value.
 *
 * Returns: the list length
 */
guint
hcl_value_list_get_length (HclValue *value)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), 0);
  g_return_val_if_fail (value->type == HCL_VALUE_TYPE_LIST, 0);

  return value->data.list_value->len;
}

/**
 * hcl_value_list_get_item:
 * @value: an #HclValue
 * @index: item index
 *
 * Gets an item from a list value.
 *
 * Returns: (transfer none): the list item or %NULL
 */
HclValue *
hcl_value_list_get_item (HclValue *value, guint index)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), NULL);
  g_return_val_if_fail (value->type == HCL_VALUE_TYPE_LIST, NULL);

  if (index >= value->data.list_value->len)
    return NULL;

  return g_ptr_array_index (value->data.list_value, index);
}

/**
 * hcl_value_list_add_item:
 * @value: an #HclValue
 * @item: (transfer full): item to add
 *
 * Adds an item to a list value.
 */
void
hcl_value_list_add_item (HclValue *value, HclValue *item)
{
  g_return_if_fail (HCL_IS_VALUE (value));
  g_return_if_fail (value->type == HCL_VALUE_TYPE_LIST);
  g_return_if_fail (HCL_IS_VALUE (item));

  g_ptr_array_add (value->data.list_value, item);
}

/**
 * hcl_value_object_get_keys:
 * @value: an #HclValue
 *
 * Gets all keys from an object value.
 *
 * Returns: (transfer container) (element-type utf8): list of keys
 */
GList *
hcl_value_object_get_keys (HclValue *value)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), NULL);
  g_return_val_if_fail (value->type == HCL_VALUE_TYPE_OBJECT, NULL);

  return g_hash_table_get_keys (value->data.object_value);
}

/**
 * hcl_value_object_get_member:
 * @value: an #HclValue
 * @key: member key
 *
 * Gets a member from an object value.
 *
 * Returns: (transfer none): the member value or %NULL
 */
HclValue *
hcl_value_object_get_member (HclValue *value, const gchar *key)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), NULL);
  g_return_val_if_fail (value->type == HCL_VALUE_TYPE_OBJECT, NULL);
  g_return_val_if_fail (key != NULL, NULL);

  return g_hash_table_lookup (value->data.object_value, key);
}

/**
 * hcl_value_object_set_member:
 * @value: an #HclValue
 * @key: member key
 * @member: (transfer full): member value
 *
 * Sets a member in an object value.
 */
void
hcl_value_object_set_member (HclValue *value, const gchar *key, HclValue *member)
{
  g_return_if_fail (HCL_IS_VALUE (value));
  g_return_if_fail (value->type == HCL_VALUE_TYPE_OBJECT);
  g_return_if_fail (key != NULL);
  g_return_if_fail (HCL_IS_VALUE (member));

  g_hash_table_insert (value->data.object_value, g_strdup (key), member);
}

/**
 * hcl_value_object_has_member:
 * @value: an #HclValue
 * @key: member key
 *
 * Checks if an object has a member.
 *
 * Returns: %TRUE if the object has the member
 */
gboolean
hcl_value_object_has_member (HclValue *value, const gchar *key)
{
  g_return_val_if_fail (HCL_IS_VALUE (value), FALSE);
  g_return_val_if_fail (value->type == HCL_VALUE_TYPE_OBJECT, FALSE);
  g_return_val_if_fail (key != NULL, FALSE);

  return g_hash_table_contains (value->data.object_value, key);
}

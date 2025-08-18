/* hcl-block.c - HCL block implementation
 *
 * Copyright 2024 The Slate Project
 */

#include "hcl-block.h"

/**
 * SECTION:hcl-block
 * @short_description: HCL configuration block
 * @title: HclBlock
 *
 * #HclBlock represents a configuration block in HCL.
 */

struct _HclBlock
{
  GObject parent_instance;

  gchar *type;
  gchar *label;
  GHashTable *attributes;  /* String -> HclValue* */
  GPtrArray *blocks;       /* Array of HclBlock* */
};

G_DEFINE_FINAL_TYPE (HclBlock, hcl_block, G_TYPE_OBJECT)

static void
hcl_block_finalize (GObject *object)
{
  HclBlock *self = HCL_BLOCK (object);

  g_free (self->type);
  g_free (self->label);

  if (self->attributes)
    g_hash_table_unref (self->attributes);

  if (self->blocks)
    g_ptr_array_unref (self->blocks);

  G_OBJECT_CLASS (hcl_block_parent_class)->finalize (object);
}

static void
hcl_block_class_init (HclBlockClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = hcl_block_finalize;
}

static void
hcl_block_init (HclBlock *self)
{
  self->attributes = g_hash_table_new_full (g_str_hash, g_str_equal,
                                            g_free, g_object_unref);
  self->blocks = g_ptr_array_new_with_free_func (g_object_unref);
}

/**
 * hcl_block_new:
 * @type: block type
 * @label: (nullable): block label
 *
 * Creates a new HCL block.
 *
 * Returns: (transfer full): a new #HclBlock
 */
HclBlock *
hcl_block_new (const gchar *type, const gchar *label)
{
  HclBlock *self = g_object_new (HCL_TYPE_BLOCK, NULL);

  self->type = g_strdup (type);
  self->label = g_strdup (label);

  return self;
}

/**
 * hcl_block_get_block_type:
 * @block: an #HclBlock
 *
 * Gets the block type.
 *
 * Returns: the block type
 */
const gchar *
hcl_block_get_block_type (HclBlock *block)
{
  g_return_val_if_fail (HCL_IS_BLOCK (block), NULL);

  return block->type;
}

/**
 * hcl_block_get_label:
 * @block: an #HclBlock
 *
 * Gets the block label.
 *
 * Returns: (nullable): the block label
 */
const gchar *
hcl_block_get_label (HclBlock *block)
{
  g_return_val_if_fail (HCL_IS_BLOCK (block), NULL);

  return block->label;
}

/**
 * hcl_block_set_label:
 * @block: an #HclBlock
 * @label: (nullable): new label
 *
 * Sets the block label.
 */
void
hcl_block_set_label (HclBlock *block, const gchar *label)
{
  g_return_if_fail (HCL_IS_BLOCK (block));

  g_free (block->label);
  block->label = g_strdup (label);
}

/**
 * hcl_block_get_attribute_names:
 * @block: an #HclBlock
 *
 * Gets all attribute names from the block.
 *
 * Returns: (transfer container) (element-type utf8): list of attribute names
 */
GList *
hcl_block_get_attribute_names (HclBlock *block)
{
  g_return_val_if_fail (HCL_IS_BLOCK (block), NULL);

  return g_hash_table_get_keys (block->attributes);
}

/**
 * hcl_block_get_attribute:
 * @block: an #HclBlock
 * @name: attribute name
 *
 * Gets an attribute value from the block.
 *
 * Returns: (transfer none) (nullable): the attribute value
 */
HclValue *
hcl_block_get_attribute (HclBlock *block, const gchar *name)
{
  g_return_val_if_fail (HCL_IS_BLOCK (block), NULL);
  g_return_val_if_fail (name != NULL, NULL);

  return g_hash_table_lookup (block->attributes, name);
}

/**
 * hcl_block_set_attribute:
 * @block: an #HclBlock
 * @name: attribute name
 * @value: (transfer full): attribute value
 *
 * Sets an attribute in the block.
 */
void
hcl_block_set_attribute (HclBlock *block, const gchar *name, HclValue *value)
{
  g_return_if_fail (HCL_IS_BLOCK (block));
  g_return_if_fail (name != NULL);
  g_return_if_fail (HCL_IS_VALUE (value));

  g_hash_table_insert (block->attributes, g_strdup (name), value);
}

/**
 * hcl_block_has_attribute:
 * @block: an #HclBlock
 * @name: attribute name
 *
 * Checks if the block has an attribute.
 *
 * Returns: %TRUE if the block has the attribute
 */
gboolean
hcl_block_has_attribute (HclBlock *block, const gchar *name)
{
  g_return_val_if_fail (HCL_IS_BLOCK (block), FALSE);
  g_return_val_if_fail (name != NULL, FALSE);

  return g_hash_table_contains (block->attributes, name);
}

/**
 * hcl_block_get_blocks:
 * @block: an #HclBlock
 *
 * Gets all nested blocks.
 *
 * Returns: (transfer container) (element-type HclBlock): list of blocks
 */
GList *
hcl_block_get_blocks (HclBlock *block)
{
  GList *result = NULL;
  guint i;

  g_return_val_if_fail (HCL_IS_BLOCK (block), NULL);

  for (i = 0; i < block->blocks->len; i++) {
    result = g_list_prepend (result, g_ptr_array_index (block->blocks, i));
  }

  return g_list_reverse (result);
}

/**
 * hcl_block_add_block:
 * @block: an #HclBlock
 * @child: (transfer full): child block to add
 *
 * Adds a nested block.
 */
void
hcl_block_add_block (HclBlock *block, HclBlock *child)
{
  g_return_if_fail (HCL_IS_BLOCK (block));
  g_return_if_fail (HCL_IS_BLOCK (child));

  g_ptr_array_add (block->blocks, child);
}

/**
 * hcl_block_get_blocks_by_type:
 * @block: an #HclBlock
 * @type: block type to filter by
 *
 * Gets all nested blocks of a specific type.
 *
 * Returns: (transfer container) (element-type HclBlock): list of matching blocks
 */
GList *
hcl_block_get_blocks_by_type (HclBlock *block, const gchar *type)
{
  GList *result = NULL;
  guint i;

  g_return_val_if_fail (HCL_IS_BLOCK (block), NULL);
  g_return_val_if_fail (type != NULL, NULL);

  for (i = 0; i < block->blocks->len; i++) {
    HclBlock *child = g_ptr_array_index (block->blocks, i);
    if (g_strcmp0 (hcl_block_get_block_type (child), type) == 0) {
      result = g_list_prepend (result, child);
    }
  }

  return g_list_reverse (result);
}

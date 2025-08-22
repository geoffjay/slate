/* hcl-document.c - HCL document implementation
 *
 * Copyright 2024 Geoff Johnson <geoff.jay@gmail.com>
 */

#include "hcl-document.h"

/**
 * SECTION:hcl-document
 * @short_description: HCL document container
 * @title: HclDocument
 *
 * #HclDocument represents a complete HCL configuration document.
 */

struct _HclDocument
{
  GObject parent_instance;

  GHashTable *attributes;  /* String -> HclValue* */
  GPtrArray *blocks;       /* Array of HclBlock* */
};

G_DEFINE_FINAL_TYPE (HclDocument, hcl_document, G_TYPE_OBJECT)

static void
hcl_document_finalize (GObject *object)
{
  HclDocument *self = HCL_DOCUMENT (object);

  if (self->attributes)
    g_hash_table_unref (self->attributes);

  if (self->blocks)
    g_ptr_array_unref (self->blocks);

  G_OBJECT_CLASS (hcl_document_parent_class)->finalize (object);
}

static void
hcl_document_class_init (HclDocumentClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = hcl_document_finalize;
}

static void
hcl_document_init (HclDocument *self)
{
  self->attributes = g_hash_table_new_full (g_str_hash, g_str_equal,
                                            g_free, g_object_unref);
  self->blocks = g_ptr_array_new_with_free_func (g_object_unref);
}

/**
 * hcl_document_new:
 *
 * Creates a new HCL document.
 *
 * Returns: (transfer full): a new #HclDocument
 */
HclDocument *
hcl_document_new (void)
{
  return g_object_new (HCL_TYPE_DOCUMENT, NULL);
}

/**
 * hcl_document_get_attribute_names:
 * @document: an #HclDocument
 *
 * Gets all attribute names from the document.
 *
 * Returns: (transfer container) (element-type utf8): list of attribute names
 */
GList *
hcl_document_get_attribute_names (HclDocument *document)
{
  g_return_val_if_fail (HCL_IS_DOCUMENT (document), NULL);

  return g_hash_table_get_keys (document->attributes);
}

/**
 * hcl_document_get_attribute:
 * @document: an #HclDocument
 * @name: attribute name
 *
 * Gets an attribute value from the document.
 *
 * Returns: (transfer none) (nullable): the attribute value
 */
HclValue *
hcl_document_get_attribute (HclDocument *document, const gchar *name)
{
  g_return_val_if_fail (HCL_IS_DOCUMENT (document), NULL);
  g_return_val_if_fail (name != NULL, NULL);

  return g_hash_table_lookup (document->attributes, name);
}

/**
 * hcl_document_set_attribute:
 * @document: an #HclDocument
 * @name: attribute name
 * @value: (transfer full): attribute value
 *
 * Sets an attribute in the document.
 */
void
hcl_document_set_attribute (HclDocument *document, const gchar *name, HclValue *value)
{
  g_return_if_fail (HCL_IS_DOCUMENT (document));
  g_return_if_fail (name != NULL);
  g_return_if_fail (HCL_IS_VALUE (value));

  g_hash_table_insert (document->attributes, g_strdup (name), value);
}

/**
 * hcl_document_has_attribute:
 * @document: an #HclDocument
 * @name: attribute name
 *
 * Checks if the document has an attribute.
 *
 * Returns: %TRUE if the document has the attribute
 */
gboolean
hcl_document_has_attribute (HclDocument *document, const gchar *name)
{
  g_return_val_if_fail (HCL_IS_DOCUMENT (document), FALSE);
  g_return_val_if_fail (name != NULL, FALSE);

  return g_hash_table_contains (document->attributes, name);
}

/**
 * hcl_document_get_blocks:
 * @document: an #HclDocument
 *
 * Gets all top-level blocks.
 *
 * Returns: (transfer container) (element-type HclBlock): list of blocks
 */
GList *
hcl_document_get_blocks (HclDocument *document)
{
  GList *result = NULL;
  guint i;

  g_return_val_if_fail (HCL_IS_DOCUMENT (document), NULL);

  for (i = 0; i < document->blocks->len; i++) {
    result = g_list_prepend (result, g_ptr_array_index (document->blocks, i));
  }

  return g_list_reverse (result);
}

/**
 * hcl_document_add_block:
 * @document: an #HclDocument
 * @block: (transfer full): block to add
 *
 * Adds a top-level block to the document.
 */
void
hcl_document_add_block (HclDocument *document, HclBlock *block)
{
  g_return_if_fail (HCL_IS_DOCUMENT (document));
  g_return_if_fail (HCL_IS_BLOCK (block));

  g_ptr_array_add (document->blocks, block);
}

/**
 * hcl_document_get_blocks_by_type:
 * @document: an #HclDocument
 * @type: block type to filter by
 *
 * Gets all top-level blocks of a specific type.
 *
 * Returns: (transfer container) (element-type HclBlock): list of matching blocks
 */
GList *
hcl_document_get_blocks_by_type (HclDocument *document, const gchar *type)
{
  GList *result = NULL;
  guint i;

  g_return_val_if_fail (HCL_IS_DOCUMENT (document), NULL);
  g_return_val_if_fail (type != NULL, NULL);

  for (i = 0; i < document->blocks->len; i++) {
    HclBlock *block = g_ptr_array_index (document->blocks, i);
    if (g_strcmp0 (hcl_block_get_block_type (block), type) == 0) {
      result = g_list_prepend (result, block);
    }
  }

  return g_list_reverse (result);
}

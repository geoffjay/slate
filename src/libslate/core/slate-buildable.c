/* slate-buildable.c
 *
 * Copyright 2024 Geoff Johnson <geoff.jay@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "slate-buildable.h"
#include <hcl.h>

/**
 * SlateBuildable:
 *
 * A common interface for buildable objects that can be constructed from HCL.
 *
 * This interface provides the basic functionality needed for objects that
 * can be built from HCL configuration. It includes methods for getting and
 * setting HCL blocks, as well as building the object from an HCL block.
 */

G_DEFINE_INTERFACE (SlateBuildable, slate_buildable, G_TYPE_OBJECT)

static void
slate_buildable_default_init (SlateBuildableInterface *iface)
{
  /* Default implementations are provided but can be overridden */
  (void)iface;
}

/**
 * slate_buildable_get_hcl:
 * @self: a #SlateBuildable
 *
 * Gets the default HCL representation for this buildable object.
 *
 * Returns: (transfer none): the HCL string representation
 */
const char *
slate_buildable_get_hcl (SlateBuildable *self)
{
  g_return_val_if_fail (SLATE_IS_BUILDABLE (self), NULL);

  SlateBuildableInterface *iface = SLATE_BUILDABLE_GET_IFACE (self);

  if (iface->get_hcl != NULL)
    return iface->get_hcl (self);

  return slate_buildable_get_hcl_default ();
}

/**
 * slate_buildable_get_block:
 * @self: a #SlateBuildable
 *
 * Gets the HCL block associated with this buildable object.
 *
 * Returns: (transfer none) (nullable): the HCL block
 */
HclBlock *
slate_buildable_get_block (SlateBuildable *self)
{
  g_return_val_if_fail (SLATE_IS_BUILDABLE (self), NULL);

  SlateBuildableInterface *iface = SLATE_BUILDABLE_GET_IFACE (self);

  if (iface->get_block != NULL)
    return iface->get_block (self);

  return NULL;
}

/**
 * slate_buildable_set_block:
 * @self: a #SlateBuildable
 * @block: (nullable): the HCL block to set
 *
 * Sets the HCL block for this buildable object.
 */
void
slate_buildable_set_block (SlateBuildable *self,
                            HclBlock       *block)
{
  g_return_if_fail (SLATE_IS_BUILDABLE (self));

  SlateBuildableInterface *iface = SLATE_BUILDABLE_GET_IFACE (self);

  if (iface->set_block != NULL)
    iface->set_block (self, block);
}

/**
 * slate_buildable_build_from_hcl_block:
 * @self: a #SlateBuildable
 * @block: the HCL block to build from
 *
 * Builds the object using the provided HCL block.
 */
void
slate_buildable_build_from_hcl_block (SlateBuildable *self,
                                       HclBlock       *block)
{
  g_return_if_fail (SLATE_IS_BUILDABLE (self));
  g_return_if_fail (HCL_IS_BLOCK (block));

  SlateBuildableInterface *iface = SLATE_BUILDABLE_GET_IFACE (self);

  if (iface->build_from_hcl_block != NULL)
    iface->build_from_hcl_block (self, block);
}

/**
 * slate_buildable_get_hcl_default:
 *
 * Gets the default HCL representation for buildable objects.
 *
 * Returns: (transfer none): the default HCL string
 */
const char *
slate_buildable_get_hcl_default (void)
{
  return "object \"buildable\" {\n"
         "  id = \"buildable0\"\n"
         "}";
}



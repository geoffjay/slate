/* slate-buildable.h
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

#pragma once

#include <glib-object.h>
#include <hcl.h>

G_BEGIN_DECLS

#define SLATE_TYPE_BUILDABLE (slate_buildable_get_type())

G_DECLARE_INTERFACE (SlateBuildable, slate_buildable, SLATE, BUILDABLE, GObject)

/**
 * SlateBuildableInterface:
 * @get_hcl: Virtual function to get the default HCL representation
 * @get_block: Virtual function to get the HCL block
 * @set_block: Virtual function to set the HCL block
 * @build_from_hcl_block: Virtual function to build the object from an HCL block
 *
 * Interface for buildable objects that can be constructed from HCL configuration.
 */
struct _SlateBuildableInterface
{
  GTypeInterface parent_iface;

  /* Virtual methods */
  const char * (*get_hcl) (SlateBuildable *self);
  HclBlock *   (*get_block) (SlateBuildable *self);
  void         (*set_block) (SlateBuildable *self, HclBlock *block);
  void         (*build_from_hcl_block) (SlateBuildable *self, HclBlock *block);
};

/* Interface methods */
const char *slate_buildable_get_hcl (SlateBuildable *self);
HclBlock   *slate_buildable_get_block (SlateBuildable *self);
void        slate_buildable_set_block (SlateBuildable *self, HclBlock *block);
void        slate_buildable_build_from_hcl_block (SlateBuildable *self, HclBlock *block);

/* Default implementations */
const char *slate_buildable_get_hcl_default (void);

G_END_DECLS

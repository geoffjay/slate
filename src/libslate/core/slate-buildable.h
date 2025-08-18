/* slate-buildable.h
 *
 * Copyright 2024 Slate Contributors
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
#include <libxml/tree.h>

G_BEGIN_DECLS

#define SLATE_TYPE_BUILDABLE (slate_buildable_get_type())

G_DECLARE_INTERFACE (SlateBuildable, slate_buildable, SLATE, BUILDABLE, GObject)

/**
 * SlateBuildableInterface:
 * @get_xml: Virtual function to get the default XML representation
 * @get_xsd: Virtual function to get the XSD schema
 * @get_node: Virtual function to get the XML node
 * @set_node: Virtual function to set the XML node
 * @build_from_xml_node: Virtual function to build the object from an XML node
 *
 * Interface for buildable objects that can be constructed from XML.
 */
struct _SlateBuildableInterface
{
  GTypeInterface parent_iface;

  /* Virtual methods */
  const char * (*get_xml) (SlateBuildable *self);
  const char * (*get_xsd) (SlateBuildable *self);
  xmlNode *    (*get_node) (SlateBuildable *self);
  void         (*set_node) (SlateBuildable *self, xmlNode *node);
  void         (*build_from_xml_node) (SlateBuildable *self, xmlNode *node);
};

/* Interface methods */
const char *slate_buildable_get_xml (SlateBuildable *self);
const char *slate_buildable_get_xsd (SlateBuildable *self);
xmlNode    *slate_buildable_get_node (SlateBuildable *self);
void        slate_buildable_set_node (SlateBuildable *self, xmlNode *node);
void        slate_buildable_build_from_xml_node (SlateBuildable *self, xmlNode *node);

/* Default implementations */
const char *slate_buildable_get_xml_default (void);
const char *slate_buildable_get_xsd_default (void);

G_END_DECLS

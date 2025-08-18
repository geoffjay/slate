/* slate-buildable.c
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

#include "slate-buildable.h"

/**
 * SlateBuildable:
 *
 * A common interface for buildable objects that can be constructed from XML.
 *
 * This interface provides the basic functionality needed for objects that
 * can be built from XML configuration. It includes methods for getting and
 * setting XML nodes, as well as building the object from an XML node.
 */

G_DEFINE_INTERFACE (SlateBuildable, slate_buildable, G_TYPE_OBJECT)

static void
slate_buildable_default_init (SlateBuildableInterface *iface)
{
  /* Default implementations are provided but can be overridden */
  (void)iface;
}

/**
 * slate_buildable_get_xml:
 * @self: a #SlateBuildable
 *
 * Gets the default XML representation for this buildable object.
 *
 * Returns: (transfer none): the XML string representation
 */
const char *
slate_buildable_get_xml (SlateBuildable *self)
{
  g_return_val_if_fail (SLATE_IS_BUILDABLE (self), NULL);

  SlateBuildableInterface *iface = SLATE_BUILDABLE_GET_IFACE (self);

  if (iface->get_xml != NULL)
    return iface->get_xml (self);

  return slate_buildable_get_xml_default ();
}

/**
 * slate_buildable_get_xsd:
 * @self: a #SlateBuildable
 *
 * Gets the XSD schema for this buildable object.
 *
 * Returns: (transfer none): the XSD schema string
 */
const char *
slate_buildable_get_xsd (SlateBuildable *self)
{
  g_return_val_if_fail (SLATE_IS_BUILDABLE (self), NULL);

  SlateBuildableInterface *iface = SLATE_BUILDABLE_GET_IFACE (self);

  if (iface->get_xsd != NULL)
    return iface->get_xsd (self);

  return slate_buildable_get_xsd_default ();
}

/**
 * slate_buildable_get_node:
 * @self: a #SlateBuildable
 *
 * Gets the XML node associated with this buildable object.
 *
 * Returns: (transfer none) (nullable): the XML node
 */
xmlNode *
slate_buildable_get_node (SlateBuildable *self)
{
  g_return_val_if_fail (SLATE_IS_BUILDABLE (self), NULL);

  SlateBuildableInterface *iface = SLATE_BUILDABLE_GET_IFACE (self);

  if (iface->get_node != NULL)
    return iface->get_node (self);

  return NULL;
}

/**
 * slate_buildable_set_node:
 * @self: a #SlateBuildable
 * @node: (nullable): the XML node to set
 *
 * Sets the XML node for this buildable object.
 */
void
slate_buildable_set_node (SlateBuildable *self,
                           xmlNode        *node)
{
  g_return_if_fail (SLATE_IS_BUILDABLE (self));

  SlateBuildableInterface *iface = SLATE_BUILDABLE_GET_IFACE (self);

  if (iface->set_node != NULL)
    iface->set_node (self, node);
}

/**
 * slate_buildable_build_from_xml_node:
 * @self: a #SlateBuildable
 * @node: the XML node to build from
 *
 * Builds the object using the provided XML node.
 */
void
slate_buildable_build_from_xml_node (SlateBuildable *self,
                                      xmlNode        *node)
{
  g_return_if_fail (SLATE_IS_BUILDABLE (self));
  g_return_if_fail (node != NULL);

  SlateBuildableInterface *iface = SLATE_BUILDABLE_GET_IFACE (self);

  if (iface->build_from_xml_node != NULL)
    iface->build_from_xml_node (self, node);
}

/**
 * slate_buildable_get_xml_default:
 *
 * Gets the default XML representation for buildable objects.
 *
 * Returns: (transfer none): the default XML string
 */
const char *
slate_buildable_get_xml_default (void)
{
  return "<object type=\"buildable\"/>";
}

/**
 * slate_buildable_get_xsd_default:
 *
 * Gets the default XSD schema for buildable objects.
 *
 * Returns: (transfer none): the default XSD schema string
 */
const char *
slate_buildable_get_xsd_default (void)
{
  return "<xs:element name=\"object\">\n"
         "  <xs:attribute name=\"id\" type=\"xs:string\" use=\"required\"/>\n"
         "  <xs:attribute name=\"type\" type=\"xs:string\" use=\"required\"/>\n"
         "</xs:element>";
}

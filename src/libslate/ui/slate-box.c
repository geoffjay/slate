/* slate-box.c
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

#include "slate-box.h"

struct _SlateBox
{
  GtkBox parent_instance;

  char *id;
  gboolean fill;
  xmlNode *node;
};

enum {
  PROP_0,
  PROP_ID,
  PROP_FILL,
  PROP_SLATE_ORIENTATION,
  N_PROPS
};

static GParamSpec *properties[N_PROPS];

/* Interface implementations */
static void slate_buildable_iface_init (SlateBuildableInterface *iface);
static void slate_widget_iface_init (SlateWidgetInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE (SlateBox, slate_box, GTK_TYPE_BOX,
                               G_IMPLEMENT_INTERFACE (SLATE_TYPE_BUILDABLE, slate_buildable_iface_init)
                               G_IMPLEMENT_INTERFACE (SLATE_TYPE_WIDGET, slate_widget_iface_init))

/* SlateBuildable interface implementation */
static const char *
slate_box_buildable_get_xml (SlateBuildable *buildable)
{
  (void)buildable;
  return "<object id=\"box0\" type=\"box\"/>";
}

static const char *
slate_box_buildable_get_xsd (SlateBuildable *buildable)
{
  (void)buildable;
  return "<xs:element name=\"object\">\n"
         "  <xs:attribute name=\"id\" type=\"xs:string\" use=\"required\"/>\n"
         "  <xs:attribute name=\"type\" type=\"xs:string\" use=\"required\"/>\n"
         "</xs:element>";
}

static xmlNode *
slate_box_buildable_get_node (SlateBuildable *buildable)
{
  SlateBox *self = SLATE_BOX (buildable);
  return self->node;
}

static void
slate_box_buildable_set_node (SlateBuildable *buildable,
                               xmlNode        *node)
{
  SlateBox *self = SLATE_BOX (buildable);
  self->node = node;
}

static void
slate_box_buildable_build_from_xml_node (SlateBuildable *buildable,
                                          xmlNode        *node)
{
  SlateBox *self = SLATE_BOX (buildable);

  /* For now, just store the node - XML building will be implemented later */
  slate_box_buildable_set_node (buildable, node);
}

static void
slate_buildable_iface_init (SlateBuildableInterface *iface)
{
  iface->get_xml = slate_box_buildable_get_xml;
  iface->get_xsd = slate_box_buildable_get_xsd;
  iface->get_node = slate_box_buildable_get_node;
  iface->set_node = slate_box_buildable_set_node;
  iface->build_from_xml_node = slate_box_buildable_build_from_xml_node;
}

/* SlateWidget interface implementation */
static gboolean
slate_box_widget_get_fill (SlateWidget *widget)
{
  SlateBox *self = SLATE_BOX (widget);
  return self->fill;
}

static void
slate_box_widget_set_fill (SlateWidget *widget,
                            gboolean     fill)
{
  SlateBox *self = SLATE_BOX (widget);

  if (self->fill != fill)
    {
      self->fill = fill;
      g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_FILL]);
    }
}

static void
slate_widget_iface_init (SlateWidgetInterface *iface)
{
  iface->get_fill = slate_box_widget_get_fill;
  iface->set_fill = slate_box_widget_set_fill;
}

/* GObject implementation */
static void
slate_box_get_property (GObject    *object,
                         guint       prop_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
  SlateBox *self = SLATE_BOX (object);

  switch (prop_id)
    {
    case PROP_ID:
      g_value_set_string (value, self->id);
      break;
    case PROP_FILL:
      g_value_set_boolean (value, self->fill);
      break;
    case PROP_SLATE_ORIENTATION:
      g_value_set_enum (value,
                        gtk_orientable_get_orientation (GTK_ORIENTABLE (self)) == GTK_ORIENTATION_HORIZONTAL ?
                        SLATE_ORIENTATION_HORIZONTAL : SLATE_ORIENTATION_VERTICAL);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
slate_box_set_property (GObject      *object,
                         guint         prop_id,
                         const GValue *value,
                         GParamSpec   *pspec)
{
  SlateBox *self = SLATE_BOX (object);

  switch (prop_id)
    {
    case PROP_ID:
      slate_box_set_id (self, g_value_get_string (value));
      break;
    case PROP_FILL:
      self->fill = g_value_get_boolean (value);
      break;
    case PROP_SLATE_ORIENTATION:
      slate_box_set_slate_orientation (self, g_value_get_enum (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
slate_box_finalize (GObject *object)
{
  SlateBox *self = SLATE_BOX (object);

  g_clear_pointer (&self->id, g_free);

  G_OBJECT_CLASS (slate_box_parent_class)->finalize (object);
}

static void
slate_box_class_init (SlateBoxClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->get_property = slate_box_get_property;
  object_class->set_property = slate_box_set_property;
  object_class->finalize = slate_box_finalize;

  /**
   * SlateBox:id:
   *
   * The unique identifier for this box.
   */
  properties[PROP_ID] =
    g_param_spec_string ("id",
                         "ID",
                         "The unique identifier for this box",
                         "box0",
                         G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  /**
   * SlateBox:fill:
   *
   * Whether the box should fill available space.
   */
  properties[PROP_FILL] =
    g_param_spec_boolean ("fill",
                          "Fill",
                          "Whether the box should fill available space",
                          TRUE,
                          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  /**
   * SlateBox:slate-orientation:
   *
   * The orientation of the box (Slate enum version).
   */
  properties[PROP_SLATE_ORIENTATION] =
    g_param_spec_enum ("slate-orientation",
                       "Slate Orientation",
                       "The orientation of the box (Slate enum)",
                       SLATE_TYPE_ORIENTATION,
                       SLATE_ORIENTATION_VERTICAL,
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, N_PROPS, properties);

  gtk_widget_class_set_template_from_resource (widget_class,
                                               "/org/gnome/libslate/ui/box.ui");
}

static void
slate_box_init (SlateBox *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->id = g_strdup ("box0");
  self->fill = TRUE;
  self->node = NULL;
}

/**
 * slate_box_new:
 *
 * Creates a new #SlateBox.
 *
 * Returns: (transfer full): a new #SlateBox
 */
GtkWidget *
slate_box_new (void)
{
  return g_object_new (SLATE_TYPE_BOX, NULL);
}

/**
 * slate_box_get_id:
 * @self: a #SlateBox
 *
 * Gets the ID of the box.
 *
 * Returns: (transfer none): the ID of the box
 */
const char *
slate_box_get_id (SlateBox *self)
{
  g_return_val_if_fail (SLATE_IS_BOX (self), NULL);
  return self->id;
}

/**
 * slate_box_set_id:
 * @self: a #SlateBox
 * @id: the new ID for the box
 *
 * Sets the ID of the box.
 */
void
slate_box_set_id (SlateBox   *self,
                   const char *id)
{
  g_return_if_fail (SLATE_IS_BOX (self));

  if (g_strcmp0 (self->id, id) != 0)
    {
      g_free (self->id);
      self->id = g_strdup (id);
      g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_ID]);
    }
}

/**
 * slate_box_get_slate_orientation:
 * @self: a #SlateBox
 *
 * Gets the orientation of the box.
 *
 * Returns: the orientation of the box
 */
SlateOrientation
slate_box_get_slate_orientation (SlateBox *self)
{
  g_return_val_if_fail (SLATE_IS_BOX (self), SLATE_ORIENTATION_VERTICAL);

  return gtk_orientable_get_orientation (GTK_ORIENTABLE (self)) == GTK_ORIENTATION_HORIZONTAL ?
         SLATE_ORIENTATION_HORIZONTAL : SLATE_ORIENTATION_VERTICAL;
}

/**
 * slate_box_set_slate_orientation:
 * @self: a #SlateBox
 * @orientation: the new orientation
 *
 * Sets the orientation of the box.
 */
void
slate_box_set_slate_orientation (SlateBox         *self,
                                  SlateOrientation  orientation)
{
  g_return_if_fail (SLATE_IS_BOX (self));

  GtkOrientation gtk_orientation = slate_orientation_to_gtk (orientation);

  if (gtk_orientable_get_orientation (GTK_ORIENTABLE (self)) != gtk_orientation)
    {
      gtk_orientable_set_orientation (GTK_ORIENTABLE (self), gtk_orientation);
      g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_SLATE_ORIENTATION]);
    }
}

/**
 * slate_box_get_homogeneous:
 * @self: a #SlateBox
 *
 * Gets whether the box is homogeneous.
 *
 * Returns: %TRUE if the box is homogeneous
 */
gboolean
slate_box_get_homogeneous (SlateBox *self)
{
  g_return_val_if_fail (SLATE_IS_BOX (self), FALSE);
  return gtk_box_get_homogeneous (GTK_BOX (self));
}

/**
 * slate_box_set_homogeneous:
 * @self: a #SlateBox
 * @homogeneous: whether the box should be homogeneous
 *
 * Sets whether the box is homogeneous.
 */
void
slate_box_set_homogeneous (SlateBox *self,
                            gboolean  homogeneous)
{
  g_return_if_fail (SLATE_IS_BOX (self));
  gtk_box_set_homogeneous (GTK_BOX (self), homogeneous);
}

/**
 * slate_box_add_child:
 * @self: a #SlateBox
 * @child: the child widget to add
 *
 * Adds a child widget to the box.
 */
void
slate_box_add_child (SlateBox  *self,
                      GtkWidget *child)
{
  g_return_if_fail (SLATE_IS_BOX (self));
  g_return_if_fail (GTK_IS_WIDGET (child));

  gtk_box_append (GTK_BOX (self), child);
}

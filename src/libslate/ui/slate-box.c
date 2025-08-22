/* slate-box.c
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

#include "slate-box.h"
#include <hcl.h>

struct _SlateBox
{
  GtkBox parent_instance;

  char *id;
  gboolean fill;
  HclBlock *block;
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
slate_box_buildable_get_hcl (SlateBuildable *buildable)
{
  (void)buildable;
  return "object \"box\" {\n"
         "  id = \"box0\"\n"
         "}";
}

static HclBlock *
slate_box_buildable_get_block (SlateBuildable *buildable)
{
  SlateBox *self = SLATE_BOX (buildable);
  return self->block;
}

static void
slate_box_buildable_set_block (SlateBuildable *buildable,
                                HclBlock       *block)
{
  SlateBox *self = SLATE_BOX (buildable);

  if (self->block != NULL)
    g_object_unref (self->block);

  self->block = block != NULL ? g_object_ref (block) : NULL;
}

static void
slate_box_buildable_build_from_hcl_block (SlateBuildable *buildable,
                                           HclBlock       *block)
{
  SlateBox *self = SLATE_BOX (buildable);
  HclValue *value;

  g_return_if_fail (HCL_IS_BLOCK (block));

  /* Store the block */
  slate_box_buildable_set_block (buildable, block);

  /* Extract properties from the HCL block */
  if (hcl_block_has_attribute (block, "id"))
    {
      value = hcl_block_get_attribute (block, "id");
      if (hcl_value_is_string (value))
        slate_box_set_id (self, hcl_value_get_string (value));
    }

  if (hcl_block_has_attribute (block, "orientation"))
    {
      value = hcl_block_get_attribute (block, "orientation");
      if (hcl_value_is_string (value))
        {
          const char *orientation_str = hcl_value_get_string (value);
          if (g_strcmp0 (orientation_str, "horizontal") == 0)
            slate_box_set_slate_orientation (self, SLATE_ORIENTATION_HORIZONTAL);
          else if (g_strcmp0 (orientation_str, "vertical") == 0)
            slate_box_set_slate_orientation (self, SLATE_ORIENTATION_VERTICAL);
        }
    }

  if (hcl_block_has_attribute (block, "homogeneous"))
    {
      value = hcl_block_get_attribute (block, "homogeneous");
      if (hcl_value_is_bool (value))
        slate_box_set_homogeneous (self, hcl_value_get_bool (value));
    }

  if (hcl_block_has_attribute (block, "spacing"))
    {
      value = hcl_block_get_attribute (block, "spacing");
      if (hcl_value_is_number (value))
        gtk_box_set_spacing (GTK_BOX (self), (int)hcl_value_get_int (value));
    }

  /* TODO: Handle nested blocks for child objects */
}

static void
slate_buildable_iface_init (SlateBuildableInterface *iface)
{
  iface->get_hcl = slate_box_buildable_get_hcl;
  iface->get_block = slate_box_buildable_get_block;
  iface->set_block = slate_box_buildable_set_block;
  iface->build_from_hcl_block = slate_box_buildable_build_from_hcl_block;
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
  g_clear_object (&self->block);

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
  self->block = NULL;
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

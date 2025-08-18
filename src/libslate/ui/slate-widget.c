/* slate-widget.c
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

#include "slate-widget.h"

/**
 * SlateWidget:
 *
 * Interface for all Slate widgets.
 *
 * This interface provides common functionality that all Slate widgets
 * should implement, such as the fill property.
 */

G_DEFINE_INTERFACE (SlateWidget, slate_widget, G_TYPE_OBJECT)

static void
slate_widget_default_init (SlateWidgetInterface *iface)
{
  /* Default implementations can be overridden */
  (void)iface;

  /**
   * SlateWidget:fill:
   *
   * Whether the widget should fill available space.
   */
  g_object_interface_install_property (iface,
                                       g_param_spec_boolean ("fill",
                                                             "Fill",
                                                             "Whether the widget should fill available space",
                                                             TRUE,
                                                             G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}

/**
 * slate_widget_get_fill:
 * @self: a #SlateWidget
 *
 * Gets whether the widget should fill available space.
 *
 * Returns: %TRUE if the widget should fill available space
 */
gboolean
slate_widget_get_fill (SlateWidget *self)
{
  g_return_val_if_fail (SLATE_IS_WIDGET (self), TRUE);

  SlateWidgetInterface *iface = SLATE_WIDGET_GET_IFACE (self);

  if (iface->get_fill != NULL)
    return iface->get_fill (self);

  /* Default implementation using property */
  gboolean fill;
  g_object_get (self, "fill", &fill, NULL);
  return fill;
}

/**
 * slate_widget_set_fill:
 * @self: a #SlateWidget
 * @fill: whether the widget should fill available space
 *
 * Sets whether the widget should fill available space.
 */
void
slate_widget_set_fill (SlateWidget *self,
                        gboolean     fill)
{
  g_return_if_fail (SLATE_IS_WIDGET (self));

  SlateWidgetInterface *iface = SLATE_WIDGET_GET_IFACE (self);

  if (iface->set_fill != NULL)
    {
      iface->set_fill (self, fill);
      return;
    }

  /* Default implementation using property */
  g_object_set (self, "fill", fill, NULL);
}

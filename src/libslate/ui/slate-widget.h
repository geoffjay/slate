/* slate-widget.h
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
#include "../core/slate-buildable.h"

G_BEGIN_DECLS

#define SLATE_TYPE_WIDGET (slate_widget_get_type())

G_DECLARE_INTERFACE (SlateWidget, slate_widget, SLATE, WIDGET, GObject)

/**
 * SlateWidgetInterface:
 * @get_fill: Virtual function to get the fill property
 * @set_fill: Virtual function to set the fill property
 *
 * Interface for all Slate widgets.
 */
struct _SlateWidgetInterface
{
  GTypeInterface parent_iface;

  /* Virtual methods */
  gboolean (*get_fill) (SlateWidget *self);
  void     (*set_fill) (SlateWidget *self, gboolean fill);
};

/* Interface methods */
gboolean slate_widget_get_fill (SlateWidget *self);
void     slate_widget_set_fill (SlateWidget *self, gboolean fill);

G_END_DECLS

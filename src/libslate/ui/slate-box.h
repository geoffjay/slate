/* slate-box.h
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

#include <gtk/gtk.h>
#include <glib-object.h>
#include "../core/slate-buildable.h"
#include "slate-widget.h"
#include "slate-enums.h"

G_BEGIN_DECLS

#define SLATE_TYPE_BOX (slate_box_get_type())

G_DECLARE_FINAL_TYPE (SlateBox, slate_box, SLATE, BOX, GtkBox)

/**
 * SlateBox:
 *
 * A container widget for laying out other widgets.
 *
 * SlateBox is a layout container that can arrange its children either
 * horizontally or vertically. It implements the SlateBuildable and
 * SlateWidget interfaces.
 */

/* Constructors */
GtkWidget *slate_box_new (void);

/* Properties */
const char       *slate_box_get_id (SlateBox *self);
void              slate_box_set_id (SlateBox *self, const char *id);
SlateOrientation  slate_box_get_slate_orientation (SlateBox *self);
void              slate_box_set_slate_orientation (SlateBox *self, SlateOrientation orientation);
gboolean          slate_box_get_homogeneous (SlateBox *self);
void              slate_box_set_homogeneous (SlateBox *self, gboolean homogeneous);

/* Widget management */
void slate_box_add_child (SlateBox *self, GtkWidget *child);

G_END_DECLS

/* slate-enums.h
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
#include <gtk/gtk.h>

G_BEGIN_DECLS

/**
 * SlateOrientation:
 * @SLATE_ORIENTATION_HORIZONTAL: Horizontal orientation
 * @SLATE_ORIENTATION_VERTICAL: Vertical orientation
 *
 * Orientation options for UI elements.
 */
typedef enum {
  SLATE_ORIENTATION_HORIZONTAL,
  SLATE_ORIENTATION_VERTICAL
} SlateOrientation;

#define SLATE_TYPE_ORIENTATION (slate_orientation_get_type())
GType slate_orientation_get_type (void) G_GNUC_CONST;

/**
 * SlatePositionType:
 * @SLATE_POSITION_LEFT: Left position
 * @SLATE_POSITION_RIGHT: Right position
 * @SLATE_POSITION_TOP: Top position
 * @SLATE_POSITION_BOTTOM: Bottom position
 *
 * Position type options for UI elements.
 */
typedef enum {
  SLATE_POSITION_LEFT,
  SLATE_POSITION_RIGHT,
  SLATE_POSITION_TOP,
  SLATE_POSITION_BOTTOM
} SlatePositionType;

#define SLATE_TYPE_POSITION_TYPE (slate_position_type_get_type())
GType slate_position_type_get_type (void) G_GNUC_CONST;

/* Utility functions */
GtkOrientation slate_orientation_to_gtk (SlateOrientation orientation);
GtkPositionType slate_position_type_to_gtk (SlatePositionType position);
const char *slate_orientation_to_string (SlateOrientation orientation);
const char *slate_position_type_to_string (SlatePositionType position);
SlateOrientation slate_orientation_parse (const char *value);
SlatePositionType slate_position_type_parse (const char *value);

G_END_DECLS

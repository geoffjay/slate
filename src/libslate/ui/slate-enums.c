/* slate-enums.c
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

#include "slate-enums.h"
#include <string.h>

/**
 * slate_orientation_get_type:
 *
 * Gets the GType for SlateOrientation.
 *
 * Returns: the GType for SlateOrientation
 */
GType
slate_orientation_get_type (void)
{
  static gsize orientation_type_id = 0;

  if (g_once_init_enter (&orientation_type_id))
    {
      static const GEnumValue values[] = {
        { SLATE_ORIENTATION_HORIZONTAL, "SLATE_ORIENTATION_HORIZONTAL", "horizontal" },
        { SLATE_ORIENTATION_VERTICAL, "SLATE_ORIENTATION_VERTICAL", "vertical" },
        { 0, NULL, NULL }
      };

      GType type_id = g_enum_register_static ("SlateOrientation", values);
      g_once_init_leave (&orientation_type_id, type_id);
    }

  return orientation_type_id;
}

/**
 * slate_position_type_get_type:
 *
 * Gets the GType for SlatePositionType.
 *
 * Returns: the GType for SlatePositionType
 */
GType
slate_position_type_get_type (void)
{
  static gsize position_type_id = 0;

  if (g_once_init_enter (&position_type_id))
    {
      static const GEnumValue values[] = {
        { SLATE_POSITION_LEFT, "SLATE_POSITION_LEFT", "left" },
        { SLATE_POSITION_RIGHT, "SLATE_POSITION_RIGHT", "right" },
        { SLATE_POSITION_TOP, "SLATE_POSITION_TOP", "top" },
        { SLATE_POSITION_BOTTOM, "SLATE_POSITION_BOTTOM", "bottom" },
        { 0, NULL, NULL }
      };

      GType type_id = g_enum_register_static ("SlatePositionType", values);
      g_once_init_leave (&position_type_id, type_id);
    }

  return position_type_id;
}

/**
 * slate_orientation_to_gtk:
 * @orientation: a #SlateOrientation
 *
 * Converts a SlateOrientation to a GtkOrientation.
 *
 * Returns: the corresponding GtkOrientation
 */
GtkOrientation
slate_orientation_to_gtk (SlateOrientation orientation)
{
  switch (orientation)
    {
    case SLATE_ORIENTATION_HORIZONTAL:
      return GTK_ORIENTATION_HORIZONTAL;
    case SLATE_ORIENTATION_VERTICAL:
      return GTK_ORIENTATION_VERTICAL;
    default:
      g_return_val_if_reached (GTK_ORIENTATION_HORIZONTAL);
    }
}

/**
 * slate_position_type_to_gtk:
 * @position: a #SlatePositionType
 *
 * Converts a SlatePositionType to a GtkPositionType.
 *
 * Returns: the corresponding GtkPositionType
 */
GtkPositionType
slate_position_type_to_gtk (SlatePositionType position)
{
  switch (position)
    {
    case SLATE_POSITION_LEFT:
      return GTK_POS_LEFT;
    case SLATE_POSITION_RIGHT:
      return GTK_POS_RIGHT;
    case SLATE_POSITION_TOP:
      return GTK_POS_TOP;
    case SLATE_POSITION_BOTTOM:
      return GTK_POS_BOTTOM;
    default:
      g_return_val_if_reached (GTK_POS_LEFT);
    }
}

/**
 * slate_orientation_to_string:
 * @orientation: a #SlateOrientation
 *
 * Converts a SlateOrientation to a string representation.
 *
 * Returns: (transfer none): the string representation
 */
const char *
slate_orientation_to_string (SlateOrientation orientation)
{
  switch (orientation)
    {
    case SLATE_ORIENTATION_HORIZONTAL:
      return "horizontal";
    case SLATE_ORIENTATION_VERTICAL:
      return "vertical";
    default:
      g_return_val_if_reached ("horizontal");
    }
}

/**
 * slate_position_type_to_string:
 * @position: a #SlatePositionType
 *
 * Converts a SlatePositionType to a string representation.
 *
 * Returns: (transfer none): the string representation
 */
const char *
slate_position_type_to_string (SlatePositionType position)
{
  switch (position)
    {
    case SLATE_POSITION_LEFT:
      return "left";
    case SLATE_POSITION_RIGHT:
      return "right";
    case SLATE_POSITION_TOP:
      return "top";
    case SLATE_POSITION_BOTTOM:
      return "bottom";
    default:
      g_return_val_if_reached ("left");
    }
}

/**
 * slate_orientation_parse:
 * @value: the string to parse
 *
 * Parses a string into a SlateOrientation.
 *
 * Returns: the parsed SlateOrientation
 */
SlateOrientation
slate_orientation_parse (const char *value)
{
  if (value == NULL)
    return SLATE_ORIENTATION_HORIZONTAL;

  if (g_ascii_strcasecmp (value, "horizontal") == 0)
    return SLATE_ORIENTATION_HORIZONTAL;
  else if (g_ascii_strcasecmp (value, "vertical") == 0)
    return SLATE_ORIENTATION_VERTICAL;

  return SLATE_ORIENTATION_HORIZONTAL;
}

/**
 * slate_position_type_parse:
 * @value: the string to parse
 *
 * Parses a string into a SlatePositionType.
 *
 * Returns: the parsed SlatePositionType
 */
SlatePositionType
slate_position_type_parse (const char *value)
{
  if (value == NULL)
    return SLATE_POSITION_LEFT;

  if (g_ascii_strcasecmp (value, "left") == 0)
    return SLATE_POSITION_LEFT;
  else if (g_ascii_strcasecmp (value, "right") == 0)
    return SLATE_POSITION_RIGHT;
  else if (g_ascii_strcasecmp (value, "top") == 0)
    return SLATE_POSITION_TOP;
  else if (g_ascii_strcasecmp (value, "bottom") == 0)
    return SLATE_POSITION_BOTTOM;

  return SLATE_POSITION_LEFT;
}

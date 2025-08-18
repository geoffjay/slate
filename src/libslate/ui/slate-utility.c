/* slate-utility.c
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

#include "slate-utility.h"
#include <math.h>
#include <string.h>

/**
 * slate_utility_signum:
 * @x: any double precision number
 *
 * A sign or signum function.
 *
 * Returns: 1 if x > 0, 0 if x = 0, -1 if x < 0
 */
int
slate_utility_signum (double x)
{
  if (x == 0.0)
    return 0;
  else
    return (int)(fabs (x) / x);
}

/**
 * slate_utility_degrees_to_radians:
 * @degrees: angle in degrees
 *
 * Convert angle from degrees to radians.
 *
 * Returns: angle in radians
 */
double
slate_utility_degrees_to_radians (double degrees)
{
  return degrees * M_PI / 180.0;
}

/**
 * slate_utility_degrees_to_positive:
 * @degrees: angle in degrees
 *
 * Convert angle in degrees to an equivalent positive value < 360.
 *
 * Returns: positive angle less than 360
 */
double
slate_utility_degrees_to_positive (double degrees)
{
  double value = fabs (fmod (degrees, 360.0));
  if (degrees < 0.0)
    value = 360.0 - value;

  return value;
}

/**
 * slate_utility_hsv_lerp:
 * @value: the value to interpolate for
 * @color1: color represented by value1
 * @value1: the smallest value
 * @color2: color represented by value2
 * @value2: the largest value
 *
 * Linear interpolation in HSV color space.
 *
 * Returns: the interpolated color
 */
GdkRGBA
slate_utility_hsv_lerp (double  value,
                         GdkRGBA color1, double value1,
                         GdkRGBA color2, double value2)
{
  g_return_val_if_fail (value2 > value1, color1);
  g_return_val_if_fail (value >= value1, color1);
  g_return_val_if_fail (value <= value2, color1);
  g_return_val_if_fail (value1 >= 0.0, color1);
  g_return_val_if_fail (value1 <= 1.0, color1);
  g_return_val_if_fail (value2 >= 0.0, color1);
  g_return_val_if_fail (value2 <= 1.0, color1);

  float h1, s1, v1;
  float h2, s2, v2;
  float h, s, v;
  float r, g, b, a;

  gtk_rgb_to_hsv (color1.red, color1.green, color1.blue, &h1, &s1, &v1);
  gtk_rgb_to_hsv (color2.red, color2.green, color2.blue, &h2, &s2, &v2);

  h = h1 + (value - value1) * (h2 - h1);
  s = s1 + (value - value1) * (s2 - s1);
  v = v1 + (value - value1) * (v2 - v1);
  a = color1.alpha + (value - value1) * (color2.alpha - color1.alpha);

  gtk_hsv_to_rgb (h, s, v, &r, &g, &b);

  return (GdkRGBA) { .red = r, .green = g, .blue = b, .alpha = a };
}

/**
 * slate_utility_rgb_lerp:
 * @value: the value to interpolate for
 * @color1: color represented by value1
 * @value1: the smallest value
 * @color2: color represented by value2
 * @value2: the largest value
 *
 * Linear interpolation in RGB color space.
 *
 * Returns: the interpolated color
 */
GdkRGBA
slate_utility_rgb_lerp (double  value,
                         GdkRGBA color1, double value1,
                         GdkRGBA color2, double value2)
{
  g_return_val_if_fail (value2 > value1, color1);
  g_return_val_if_fail (value >= value1, color1);
  g_return_val_if_fail (value <= value2, color1);
  g_return_val_if_fail (value1 >= 0.0, color1);
  g_return_val_if_fail (value1 <= 1.0, color1);
  g_return_val_if_fail (value2 >= 0.0, color1);
  g_return_val_if_fail (value2 <= 1.0, color1);

  double r, g, b, a;

  r = color1.red + (value - value1) * (color2.red - color1.red);
  g = color1.green + (value - value1) * (color2.green - color1.green);
  b = color1.blue + (value - value1) * (color2.blue - color1.blue);
  a = color1.alpha + (value - value1) * (color2.alpha - color1.alpha);

  return (GdkRGBA) { .red = r, .green = g, .blue = b, .alpha = a };
}

/**
 * slate_utility_hex_to_rgb:
 * @hex: Hexadecimal string to convert
 *
 * Convert a hexadecimal string into the corresponding RGB values.
 *
 * Returns: (element-type gdouble) (transfer full): A list of doubles representing the RGB values
 */
GList *
slate_utility_hex_to_rgb (const char *hex)
{
  GList *rgb = NULL;
  GdkRGBA color;

  if (!gdk_rgba_parse (&color, hex))
    return NULL;

  rgb = g_list_append (rgb, g_malloc (sizeof (double)));
  rgb = g_list_append (rgb, g_malloc (sizeof (double)));
  rgb = g_list_append (rgb, g_malloc (sizeof (double)));

  *(double*)g_list_nth_data (rgb, 0) = color.red;
  *(double*)g_list_nth_data (rgb, 1) = color.green;
  *(double*)g_list_nth_data (rgb, 2) = color.blue;

  return rgb;
}

/**
 * slate_utility_get_color:
 * @desc: The description value to convert
 *
 * Get the GdkRGBA color value for a given descriptive value, e.g. "blue".
 *
 * Returns: The color struct for the given descriptive value
 */
GdkRGBA
slate_utility_get_color (const char *desc)
{
  GdkRGBA color = {0, 0, 0, 1.0};

  if (desc != NULL)
    gdk_rgba_parse (&color, desc);

  return color;
}

/**
 * slate_utility_type_from_name:
 * @name: Name of the type to check
 *
 * Get the GType for a given type name.
 *
 * Returns: Valid GType on success, G_TYPE_INVALID otherwise
 */
GType
slate_utility_type_from_name (const char *name)
{
  if (name == NULL)
    return G_TYPE_INVALID;

  g_autofree char *simplified = g_strdup (name);
  g_strdelimit (simplified, "ui", '\0');

  if (g_strcmp0 (simplified, "box") == 0)
    {
      /* Future: return SLATE_TYPE_BOX when implemented */
      return G_TYPE_INVALID;
    }

  /* Add other type mappings as needed */
  return G_TYPE_INVALID;
}

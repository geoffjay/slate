/* slate-utility.h
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

#include <glib.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

/**
 * SlateColorGradientType:
 * @SLATE_COLOR_GRADIENT_RGB: RGB color gradient
 * @SLATE_COLOR_GRADIENT_HSV: HSV color gradient
 *
 * Color gradient types for color interpolation.
 */
typedef enum {
  SLATE_COLOR_GRADIENT_RGB,
  SLATE_COLOR_GRADIENT_HSV
} SlateColorGradientType;

/* Math utilities */
int    slate_utility_signum (double x);
double slate_utility_degrees_to_radians (double degrees);
double slate_utility_degrees_to_positive (double degrees);

/* Color utilities */
GdkRGBA slate_utility_hsv_lerp (double value,
                                 GdkRGBA color1, double value1,
                                 GdkRGBA color2, double value2);

GdkRGBA slate_utility_rgb_lerp (double value,
                                 GdkRGBA color1, double value1,
                                 GdkRGBA color2, double value2);

GList  *slate_utility_hex_to_rgb (const char *hex);
GdkRGBA slate_utility_get_color (const char *desc);

/* Type utilities */
GType slate_utility_type_from_name (const char *name);

G_END_DECLS

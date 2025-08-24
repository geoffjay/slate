/* hello-world-widget.h
 *
 * Copyright 2025 Slate Development Team
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
#include <slate.h>

G_BEGIN_DECLS

#define HELLO_WORLD_TYPE_WIDGET (hello_world_widget_get_type())

G_DECLARE_FINAL_TYPE (HelloWorldWidget, hello_world_widget, HELLO_WORLD, WIDGET, GtkWidget)

/**
 * HelloWorldWidget:
 *
 * A simple dashboard widget that displays a greeting and demonstrates
 * basic dashboard widget functionality.
 */

/**
 * hello_world_widget_new:
 *
 * Creates a new #HelloWorldWidget.
 *
 * Returns: (transfer full): A new #HelloWorldWidget
 */
HelloWorldWidget *hello_world_widget_new (void);

/**
 * hello_world_widget_set_greeting:
 * @widget: A #HelloWorldWidget
 * @greeting: The greeting text to display
 *
 * Sets the greeting text displayed by the widget.
 */
void hello_world_widget_set_greeting (HelloWorldWidget *widget, const char *greeting);

/**
 * hello_world_widget_get_greeting:
 * @widget: A #HelloWorldWidget
 *
 * Gets the current greeting text.
 *
 * Returns: (transfer none): The current greeting text
 */
const char *hello_world_widget_get_greeting (HelloWorldWidget *widget);

G_END_DECLS

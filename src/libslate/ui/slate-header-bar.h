/* slate-header-bar.h
 *
 * Copyright 2025 Slate Contributors
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
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SLATE_TYPE_HEADER_BAR (slate_header_bar_get_type())

G_DECLARE_FINAL_TYPE (SlateHeaderBar, slate_header_bar, SLATE, HEADER_BAR, GtkWidget)

/**
 * SlateHeaderBar:
 *
 * A header bar widget for Slate applications with plugin extension points.
 *
 * SlateHeaderBar extends AdwHeaderBar to provide a consistent header bar
 * experience across Slate applications with support for plugin extensions,
 * project-specific actions, and responsive design.
 */

/**
 * slate_header_bar_new:
 *
 * Creates a new #SlateHeaderBar.
 *
 * Returns: (transfer full): a new #SlateHeaderBar
 */
SlateHeaderBar *slate_header_bar_new (void);

/**
 * slate_header_bar_set_project_title:
 * @self: a #SlateHeaderBar
 * @title: (nullable): the project title
 *
 * Sets the project title displayed in the header bar.
 */
void slate_header_bar_set_project_title (SlateHeaderBar *self,
                                          const char     *title);

/**
 * slate_header_bar_get_project_title:
 * @self: a #SlateHeaderBar
 *
 * Gets the current project title.
 *
 * Returns: (nullable): the project title
 */
const char *slate_header_bar_get_project_title (SlateHeaderBar *self);

/**
 * slate_header_bar_set_project_subtitle:
 * @self: a #SlateHeaderBar
 * @subtitle: (nullable): the project subtitle
 *
 * Sets the project subtitle displayed in the header bar.
 */
void slate_header_bar_set_project_subtitle (SlateHeaderBar *self,
                                             const char     *subtitle);

/**
 * slate_header_bar_get_project_subtitle:
 * @self: a #SlateHeaderBar
 *
 * Gets the current project subtitle.
 *
 * Returns: (nullable): the project subtitle
 */
const char *slate_header_bar_get_project_subtitle (SlateHeaderBar *self);

/**
 * slate_header_bar_add_start_widget:
 * @self: a #SlateHeaderBar
 * @widget: a #GtkWidget
 *
 * Adds a widget to the start of the header bar.
 * This is typically used by plugins to add custom buttons or controls.
 */
void slate_header_bar_add_start_widget (SlateHeaderBar *self,
                                         GtkWidget      *widget);

/**
 * slate_header_bar_add_end_widget:
 * @self: a #SlateHeaderBar
 * @widget: a #GtkWidget
 *
 * Adds a widget to the end of the header bar.
 * This is typically used by plugins to add custom buttons or controls.
 */
void slate_header_bar_add_end_widget (SlateHeaderBar *self,
                                       GtkWidget      *widget);

/**
 * slate_header_bar_remove_widget:
 * @self: a #SlateHeaderBar
 * @widget: a #GtkWidget
 *
 * Removes a widget from the header bar.
 */
void slate_header_bar_remove_widget (SlateHeaderBar *self,
                                      GtkWidget      *widget);

/**
 * slate_header_bar_set_show_project_actions:
 * @self: a #SlateHeaderBar
 * @show: whether to show project actions
 *
 * Sets whether project-specific actions should be visible.
 */
void slate_header_bar_set_show_project_actions (SlateHeaderBar *self,
                                                 gboolean        show);

/**
 * slate_header_bar_get_show_project_actions:
 * @self: a #SlateHeaderBar
 *
 * Gets whether project actions are currently visible.
 *
 * Returns: %TRUE if project actions are visible
 */
gboolean slate_header_bar_get_show_project_actions (SlateHeaderBar *self);

G_END_DECLS

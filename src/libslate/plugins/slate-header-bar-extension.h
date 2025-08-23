/* slate-header-bar-extension.h
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

#include <glib-object.h>
#include <gtk/gtk.h>
#include "../ui/slate-header-bar.h"

G_BEGIN_DECLS

#define SLATE_TYPE_HEADER_BAR_EXTENSION (slate_header_bar_extension_get_type())

G_DECLARE_INTERFACE (SlateHeaderBarExtension, slate_header_bar_extension, SLATE, HEADER_BAR_EXTENSION, GObject)

/**
 * SlateHeaderBarExtension:
 *
 * Interface for plugins that want to extend the header bar with custom widgets.
 */
struct _SlateHeaderBarExtensionInterface
{
  GTypeInterface parent_interface;

  /**
   * create_start_widgets:
   * @extension: the extension instance
   * @header_bar: the header bar to extend
   *
   * Creates widgets to be added to the start of the header bar.
   *
   * Returns: (transfer full) (element-type GtkWidget) (nullable): list of widgets to add
   */
  GList * (*create_start_widgets) (SlateHeaderBarExtension *extension,
                                   SlateHeaderBar          *header_bar);

  /**
   * create_end_widgets:
   * @extension: the extension instance
   * @header_bar: the header bar to extend
   *
   * Creates widgets to be added to the end of the header bar.
   *
   * Returns: (transfer full) (element-type GtkWidget) (nullable): list of widgets to add
   */
  GList * (*create_end_widgets) (SlateHeaderBarExtension *extension,
                                 SlateHeaderBar          *header_bar);

  /**
   * on_project_changed:
   * @extension: the extension instance
   * @header_bar: the header bar
   * @project_path: (nullable): the path to the current project, or NULL if no project
   *
   * Called when the current project changes.
   */
  void (*on_project_changed) (SlateHeaderBarExtension *extension,
                              SlateHeaderBar          *header_bar,
                              const char              *project_path);
};

GList * slate_header_bar_extension_create_start_widgets (SlateHeaderBarExtension *extension,
                                                         SlateHeaderBar          *header_bar);
GList * slate_header_bar_extension_create_end_widgets (SlateHeaderBarExtension *extension,
                                                       SlateHeaderBar          *header_bar);
void slate_header_bar_extension_on_project_changed (SlateHeaderBarExtension *extension,
                                                    SlateHeaderBar          *header_bar,
                                                    const char              *project_path);

G_END_DECLS

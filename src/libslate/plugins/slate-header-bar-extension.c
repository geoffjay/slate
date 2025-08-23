/* slate-header-bar-extension.c
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

#include "slate-header-bar-extension.h"

G_DEFINE_INTERFACE (SlateHeaderBarExtension, slate_header_bar_extension, G_TYPE_OBJECT)

static void
slate_header_bar_extension_default_init (SlateHeaderBarExtensionInterface *iface)
{
  /* Default implementations can be provided here if needed */
}

/**
 * slate_header_bar_extension_create_start_widgets:
 * @extension: a #SlateHeaderBarExtension
 * @header_bar: the #SlateHeaderBar to extend
 *
 * Creates widgets to be added to the start of the header bar.
 *
 * Returns: (transfer full) (element-type GtkWidget) (nullable): list of widgets to add
 */
GList *
slate_header_bar_extension_create_start_widgets (SlateHeaderBarExtension *extension,
                                                 SlateHeaderBar          *header_bar)
{
  SlateHeaderBarExtensionInterface *iface;

  g_return_val_if_fail (SLATE_IS_HEADER_BAR_EXTENSION (extension), NULL);
  g_return_val_if_fail (SLATE_IS_HEADER_BAR (header_bar), NULL);

  iface = SLATE_HEADER_BAR_EXTENSION_GET_IFACE (extension);
  if (iface->create_start_widgets)
    return iface->create_start_widgets (extension, header_bar);

  return NULL;
}

/**
 * slate_header_bar_extension_create_end_widgets:
 * @extension: a #SlateHeaderBarExtension
 * @header_bar: the #SlateHeaderBar to extend
 *
 * Creates widgets to be added to the end of the header bar.
 *
 * Returns: (transfer full) (element-type GtkWidget) (nullable): list of widgets to add
 */
GList *
slate_header_bar_extension_create_end_widgets (SlateHeaderBarExtension *extension,
                                               SlateHeaderBar          *header_bar)
{
  SlateHeaderBarExtensionInterface *iface;

  g_return_val_if_fail (SLATE_IS_HEADER_BAR_EXTENSION (extension), NULL);
  g_return_val_if_fail (SLATE_IS_HEADER_BAR (header_bar), NULL);

  iface = SLATE_HEADER_BAR_EXTENSION_GET_IFACE (extension);
  if (iface->create_end_widgets)
    return iface->create_end_widgets (extension, header_bar);

  return NULL;
}

/**
 * slate_header_bar_extension_on_project_changed:
 * @extension: a #SlateHeaderBarExtension
 * @header_bar: the #SlateHeaderBar
 * @project_path: (nullable): the path to the current project, or NULL if no project
 *
 * Notifies the extension that the current project has changed.
 */
void
slate_header_bar_extension_on_project_changed (SlateHeaderBarExtension *extension,
                                               SlateHeaderBar          *header_bar,
                                               const char              *project_path)
{
  SlateHeaderBarExtensionInterface *iface;

  g_return_if_fail (SLATE_IS_HEADER_BAR_EXTENSION (extension));
  g_return_if_fail (SLATE_IS_HEADER_BAR (header_bar));

  iface = SLATE_HEADER_BAR_EXTENSION_GET_IFACE (extension);
  if (iface->on_project_changed)
    iface->on_project_changed (extension, header_bar, project_path);
}

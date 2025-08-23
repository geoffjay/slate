/* slate-init.c
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

#include "slate.h"

/**
 * slate_init:
 *
 * Initializes the Slate library and registers all custom types.
 * This should be called before using any Slate widgets in Blueprint files.
 */
void
slate_init (void)
{
  static gboolean initialized = FALSE;

  if (initialized)
    return;

  /* Ensure all our custom types are registered */
  g_type_ensure (SLATE_TYPE_HEADER_BAR);
  g_type_ensure (SLATE_TYPE_BOX);
  g_type_ensure (SLATE_TYPE_WIDGET);

  initialized = TRUE;
}

/* slate.h
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
#include <gtk/gtk.h>

G_BEGIN_DECLS

/* Core headers */
#include "core/slate-buildable.h"
#include "core/slate-config.h"

/* UI headers */
#include "ui/slate-enums.h"
#include "ui/slate-widget.h"
#include "ui/slate-utility.h"
#include "ui/slate-box.h"
#include "ui/slate-header-bar.h"

/* Plugin headers */
#include "plugins/slate-plugin-interface.h"
#include "plugins/slate-header-bar-extension.h"
#include "plugins/slate-plugin-manager.h"

/**
 * slate_init:
 *
 * Initializes the Slate library and registers all custom types.
 * This should be called before using any Slate widgets in Blueprint files.
 */
void slate_init (void);

G_END_DECLS

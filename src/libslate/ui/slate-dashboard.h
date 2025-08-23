/* slate-dashboard.h
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
#include <glib-object.h>

G_BEGIN_DECLS

#define SLATE_TYPE_DASHBOARD (slate_dashboard_get_type())

G_DECLARE_FINAL_TYPE (SlateDashboard, slate_dashboard, SLATE, DASHBOARD, AdwBin)

/**
 * SlateDashboard:
 *
 * A container widget for organizing and displaying dashboard components.
 */

SlateDashboard * slate_dashboard_new (void);

void slate_dashboard_add_widget (SlateDashboard *dashboard,
                                 GtkWidget      *widget,
                                 const char     *widget_id);
void slate_dashboard_remove_widget (SlateDashboard *dashboard,
                                    const char     *widget_id);
GtkWidget * slate_dashboard_get_widget (SlateDashboard *dashboard,
                                        const char     *widget_id);

void slate_dashboard_set_layout (SlateDashboard *dashboard,
                                 const char     *layout_type);
const char * slate_dashboard_get_layout (SlateDashboard *dashboard);

void slate_dashboard_set_columns (SlateDashboard *dashboard,
                                  int             columns);
int slate_dashboard_get_columns (SlateDashboard *dashboard);

void slate_dashboard_clear (SlateDashboard *dashboard);
void slate_dashboard_refresh (SlateDashboard *dashboard);

G_END_DECLS

/* slate-dashboard-card.h
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

#define SLATE_TYPE_DASHBOARD_CARD (slate_dashboard_card_get_type())

G_DECLARE_FINAL_TYPE (SlateDashboardCard, slate_dashboard_card, SLATE, DASHBOARD_CARD, AdwBin)

/**
 * SlateDashboardCard:
 *
 * A card widget for dashboard components with title, actions, and content area.
 */

SlateDashboardCard * slate_dashboard_card_new (void);

void slate_dashboard_card_set_title (SlateDashboardCard *card, const char *title);
const char * slate_dashboard_card_get_title (SlateDashboardCard *card);

void slate_dashboard_card_set_subtitle (SlateDashboardCard *card, const char *subtitle);
const char * slate_dashboard_card_get_subtitle (SlateDashboardCard *card);

void slate_dashboard_card_set_content (SlateDashboardCard *card, GtkWidget *content);
GtkWidget * slate_dashboard_card_get_content (SlateDashboardCard *card);

void slate_dashboard_card_add_action (SlateDashboardCard *card, 
                                      const char         *action_name,
                                      const char         *icon_name,
                                      const char         *tooltip);
void slate_dashboard_card_remove_action (SlateDashboardCard *card, const char *action_name);

void slate_dashboard_card_set_loading (SlateDashboardCard *card, gboolean loading);
gboolean slate_dashboard_card_get_loading (SlateDashboardCard *card);

G_END_DECLS

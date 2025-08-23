/* slate-dashboard-card.c
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

#include "slate-dashboard-card.h"
#include <glib/gi18n.h>

/**
 * SECTION:slate-dashboard-card
 * @short_description: Card widget for dashboard components
 * @title: SlateDashboardCard
 *
 * #SlateDashboardCard provides a card-like container for dashboard components
 * with a header area for title, subtitle, and actions, plus a content area.
 */

struct _SlateDashboardCard
{
  AdwBin parent_instance;

  /* UI elements */
  GtkWidget *main_box;
  GtkWidget *header_box;
  GtkWidget *title_box;
  GtkWidget *title_label;
  GtkWidget *subtitle_label;
  GtkWidget *actions_box;
  GtkWidget *content_area;
  GtkWidget *loading_spinner;
  
  /* Properties */
  char *title;
  char *subtitle;
  gboolean loading;
  
  /* Actions */
  GHashTable *actions;
};

G_DEFINE_FINAL_TYPE (SlateDashboardCard, slate_dashboard_card, ADW_TYPE_BIN)

enum {
  PROP_0,
  PROP_TITLE,
  PROP_SUBTITLE,
  PROP_LOADING,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

enum {
  ACTION_ACTIVATED,
  N_SIGNALS
};

static guint signals [N_SIGNALS];

static void
slate_dashboard_card_dispose (GObject *object)
{
  SlateDashboardCard *self = (SlateDashboardCard *)object;

  g_clear_pointer (&self->title, g_free);
  g_clear_pointer (&self->subtitle, g_free);
  g_clear_pointer (&self->actions, g_hash_table_unref);

  G_OBJECT_CLASS (slate_dashboard_card_parent_class)->dispose (object);
}

static void
slate_dashboard_card_get_property (GObject    *object,
                                   guint       prop_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
  SlateDashboardCard *self = SLATE_DASHBOARD_CARD (object);

  switch (prop_id)
    {
    case PROP_TITLE:
      g_value_set_string (value, slate_dashboard_card_get_title (self));
      break;
    case PROP_SUBTITLE:
      g_value_set_string (value, slate_dashboard_card_get_subtitle (self));
      break;
    case PROP_LOADING:
      g_value_set_boolean (value, slate_dashboard_card_get_loading (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
slate_dashboard_card_set_property (GObject      *object,
                                   guint         prop_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
  SlateDashboardCard *self = SLATE_DASHBOARD_CARD (object);

  switch (prop_id)
    {
    case PROP_TITLE:
      slate_dashboard_card_set_title (self, g_value_get_string (value));
      break;
    case PROP_SUBTITLE:
      slate_dashboard_card_set_subtitle (self, g_value_get_string (value));
      break;
    case PROP_LOADING:
      slate_dashboard_card_set_loading (self, g_value_get_boolean (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
on_action_button_clicked (GtkButton *button, gpointer user_data)
{
  SlateDashboardCard *self = SLATE_DASHBOARD_CARD (user_data);
  const char *action_name = g_object_get_data (G_OBJECT (button), "action-name");
  
  if (action_name)
    g_signal_emit (self, signals[ACTION_ACTIVATED], 0, action_name);
}

static void
slate_dashboard_card_class_init (SlateDashboardCardClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = slate_dashboard_card_dispose;
  object_class->get_property = slate_dashboard_card_get_property;
  object_class->set_property = slate_dashboard_card_set_property;

  /**
   * SlateDashboardCard:title:
   *
   * The title of the card.
   */
  properties [PROP_TITLE] =
    g_param_spec_string ("title", NULL, NULL,
                         NULL,
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));

  /**
   * SlateDashboardCard:subtitle:
   *
   * The subtitle of the card.
   */
  properties [PROP_SUBTITLE] =
    g_param_spec_string ("subtitle", NULL, NULL,
                         NULL,
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));

  /**
   * SlateDashboardCard:loading:
   *
   * Whether the card is in loading state.
   */
  properties [PROP_LOADING] =
    g_param_spec_boolean ("loading", NULL, NULL,
                          FALSE,
                          (G_PARAM_READWRITE |
                           G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);

  /**
   * SlateDashboardCard::action-activated:
   * @card: the card
   * @action_name: the name of the activated action
   *
   * Emitted when an action button is clicked.
   */
  signals [ACTION_ACTIVATED] =
    g_signal_new ("action-activated",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL, NULL,
                  NULL,
                  G_TYPE_NONE, 1,
                  G_TYPE_STRING);

  gtk_widget_class_set_css_name (widget_class, "slate-dashboard-card");
}

static void
slate_dashboard_card_init (SlateDashboardCard *self)
{
  /* Initialize actions hash table */
  self->actions = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_object_unref);

  /* Create main container */
  self->main_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_add_css_class (self->main_box, "card");
  adw_bin_set_child (ADW_BIN (self), self->main_box);

  /* Create header */
  self->header_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
  gtk_widget_set_margin_top (self->header_box, 12);
  gtk_widget_set_margin_bottom (self->header_box, 12);
  gtk_widget_set_margin_start (self->header_box, 12);
  gtk_widget_set_margin_end (self->header_box, 12);
  gtk_box_append (GTK_BOX (self->main_box), self->header_box);

  /* Create title area */
  self->title_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
  gtk_widget_set_hexpand (self->title_box, TRUE);
  gtk_box_append (GTK_BOX (self->header_box), self->title_box);

  self->title_label = gtk_label_new (NULL);
  gtk_widget_add_css_class (self->title_label, "title-4");
  gtk_label_set_xalign (GTK_LABEL (self->title_label), 0.0);
  gtk_box_append (GTK_BOX (self->title_box), self->title_label);

  self->subtitle_label = gtk_label_new (NULL);
  gtk_widget_add_css_class (self->subtitle_label, "dim-label");
  gtk_label_set_xalign (GTK_LABEL (self->subtitle_label), 0.0);
  gtk_box_append (GTK_BOX (self->title_box), self->subtitle_label);

  /* Create actions area */
  self->actions_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_box_append (GTK_BOX (self->header_box), self->actions_box);

  /* Create loading spinner */
  self->loading_spinner = gtk_spinner_new ();
  gtk_box_append (GTK_BOX (self->actions_box), self->loading_spinner);

  /* Create content area */
  self->content_area = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_margin_start (self->content_area, 12);
  gtk_widget_set_margin_end (self->content_area, 12);
  gtk_widget_set_margin_bottom (self->content_area, 12);
  gtk_box_append (GTK_BOX (self->main_box), self->content_area);

  /* Set initial state */
  gtk_widget_set_visible (self->subtitle_label, FALSE);
  gtk_widget_set_visible (self->loading_spinner, FALSE);
}

/**
 * slate_dashboard_card_new:
 *
 * Creates a new #SlateDashboardCard.
 *
 * Returns: (transfer full): a newly created #SlateDashboardCard
 */
SlateDashboardCard *
slate_dashboard_card_new (void)
{
  return g_object_new (SLATE_TYPE_DASHBOARD_CARD, NULL);
}

/**
 * slate_dashboard_card_set_title:
 * @card: a #SlateDashboardCard
 * @title: the title text
 *
 * Sets the title of the card.
 */
void
slate_dashboard_card_set_title (SlateDashboardCard *card,
                                const char         *title)
{
  g_return_if_fail (SLATE_IS_DASHBOARD_CARD (card));

  if (g_strcmp0 (card->title, title) != 0)
    {
      g_free (card->title);
      card->title = g_strdup (title);
      
      gtk_label_set_text (GTK_LABEL (card->title_label), title ? title : "");
      gtk_widget_set_visible (card->title_label, title && strlen (title) > 0);
      
      g_object_notify_by_pspec (G_OBJECT (card), properties[PROP_TITLE]);
    }
}

/**
 * slate_dashboard_card_get_title:
 * @card: a #SlateDashboardCard
 *
 * Gets the title of the card.
 *
 * Returns: (nullable): the title text
 */
const char *
slate_dashboard_card_get_title (SlateDashboardCard *card)
{
  g_return_val_if_fail (SLATE_IS_DASHBOARD_CARD (card), NULL);

  return card->title;
}

/**
 * slate_dashboard_card_set_subtitle:
 * @card: a #SlateDashboardCard
 * @subtitle: the subtitle text
 *
 * Sets the subtitle of the card.
 */
void
slate_dashboard_card_set_subtitle (SlateDashboardCard *card,
                                   const char         *subtitle)
{
  g_return_if_fail (SLATE_IS_DASHBOARD_CARD (card));

  if (g_strcmp0 (card->subtitle, subtitle) != 0)
    {
      g_free (card->subtitle);
      card->subtitle = g_strdup (subtitle);
      
      gtk_label_set_text (GTK_LABEL (card->subtitle_label), subtitle ? subtitle : "");
      gtk_widget_set_visible (card->subtitle_label, subtitle && strlen (subtitle) > 0);
      
      g_object_notify_by_pspec (G_OBJECT (card), properties[PROP_SUBTITLE]);
    }
}

/**
 * slate_dashboard_card_get_subtitle:
 * @card: a #SlateDashboardCard
 *
 * Gets the subtitle of the card.
 *
 * Returns: (nullable): the subtitle text
 */
const char *
slate_dashboard_card_get_subtitle (SlateDashboardCard *card)
{
  g_return_val_if_fail (SLATE_IS_DASHBOARD_CARD (card), NULL);

  return card->subtitle;
}

/**
 * slate_dashboard_card_set_content:
 * @card: a #SlateDashboardCard
 * @content: the content widget
 *
 * Sets the content widget of the card.
 */
void
slate_dashboard_card_set_content (SlateDashboardCard *card,
                                  GtkWidget          *content)
{
  GtkWidget *current_content;

  g_return_if_fail (SLATE_IS_DASHBOARD_CARD (card));
  g_return_if_fail (content == NULL || GTK_IS_WIDGET (content));

  /* Remove current content */
  current_content = gtk_widget_get_first_child (card->content_area);
  if (current_content)
    gtk_box_remove (GTK_BOX (card->content_area), current_content);

  /* Add new content */
  if (content)
    gtk_box_append (GTK_BOX (card->content_area), content);
}

/**
 * slate_dashboard_card_get_content:
 * @card: a #SlateDashboardCard
 *
 * Gets the content widget of the card.
 *
 * Returns: (transfer none) (nullable): the content widget
 */
GtkWidget *
slate_dashboard_card_get_content (SlateDashboardCard *card)
{
  g_return_val_if_fail (SLATE_IS_DASHBOARD_CARD (card), NULL);

  return gtk_widget_get_first_child (card->content_area);
}

/**
 * slate_dashboard_card_add_action:
 * @card: a #SlateDashboardCard
 * @action_name: unique name for the action
 * @icon_name: icon name for the button
 * @tooltip: tooltip text for the button
 *
 * Adds an action button to the card header.
 */
void
slate_dashboard_card_add_action (SlateDashboardCard *card,
                                 const char         *action_name,
                                 const char         *icon_name,
                                 const char         *tooltip)
{
  GtkWidget *button;

  g_return_if_fail (SLATE_IS_DASHBOARD_CARD (card));
  g_return_if_fail (action_name != NULL);
  g_return_if_fail (icon_name != NULL);

  /* Remove existing action with same name */
  slate_dashboard_card_remove_action (card, action_name);

  /* Create button */
  button = gtk_button_new_from_icon_name (icon_name);
  gtk_widget_add_css_class (button, "flat");
  gtk_widget_add_css_class (button, "circular");
  
  if (tooltip)
    gtk_widget_set_tooltip_text (button, tooltip);

  /* Store action name */
  g_object_set_data_full (G_OBJECT (button), "action-name", g_strdup (action_name), g_free);
  
  /* Connect signal */
  g_signal_connect (button, "clicked", G_CALLBACK (on_action_button_clicked), card);

  /* Add to actions */
  g_hash_table_insert (card->actions, g_strdup (action_name), g_object_ref (button));
  gtk_box_append (GTK_BOX (card->actions_box), button);
}

/**
 * slate_dashboard_card_remove_action:
 * @card: a #SlateDashboardCard
 * @action_name: name of the action to remove
 *
 * Removes an action button from the card.
 */
void
slate_dashboard_card_remove_action (SlateDashboardCard *card,
                                    const char         *action_name)
{
  GtkWidget *button;

  g_return_if_fail (SLATE_IS_DASHBOARD_CARD (card));
  g_return_if_fail (action_name != NULL);

  button = g_hash_table_lookup (card->actions, action_name);
  if (button)
    {
      gtk_box_remove (GTK_BOX (card->actions_box), button);
      g_hash_table_remove (card->actions, action_name);
    }
}

/**
 * slate_dashboard_card_set_loading:
 * @card: a #SlateDashboardCard
 * @loading: whether the card is loading
 *
 * Sets the loading state of the card.
 */
void
slate_dashboard_card_set_loading (SlateDashboardCard *card,
                                  gboolean            loading)
{
  g_return_if_fail (SLATE_IS_DASHBOARD_CARD (card));

  if (card->loading != loading)
    {
      card->loading = loading;
      
      gtk_widget_set_visible (card->loading_spinner, loading);
      if (loading)
        gtk_spinner_start (GTK_SPINNER (card->loading_spinner));
      else
        gtk_spinner_stop (GTK_SPINNER (card->loading_spinner));
      
      g_object_notify_by_pspec (G_OBJECT (card), properties[PROP_LOADING]);
    }
}

/**
 * slate_dashboard_card_get_loading:
 * @card: a #SlateDashboardCard
 *
 * Gets the loading state of the card.
 *
 * Returns: %TRUE if the card is loading
 */
gboolean
slate_dashboard_card_get_loading (SlateDashboardCard *card)
{
  g_return_val_if_fail (SLATE_IS_DASHBOARD_CARD (card), FALSE);

  return card->loading;
}

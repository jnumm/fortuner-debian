/*
 * fortuner
 * Copyright (C) 2012 Juhani Numminen <juhaninumminen0@gmail.com>
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

#include <gtk/gtk.h>

#include "config.h"
#include "gtk_ui.h"
#include "main.h"
#include "settings.h"

static GtkWidget *menu = NULL;
static GtkStatusIcon *status_icon = NULL;

static void destroy (GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();
}

static void activate (GtkStatusIcon *status_icon, gpointer user_data)
{
    send_fortune ();
}

// Called when About is selected from right-click menu
static void show_about_dialog (GtkMenuItem *menu_item,
        gpointer user_data)
{
    // This helps prevent multiple instances
    if (!gtk_grab_get_current ())
    {
        const gchar *authors[] =
        { "Juhani Numminen <juhaninumminen0@gmail.com>", NULL };
        const gchar *license =
            "This program is free software; you can redistribute it and/or modify\n"
            "it under the terms of the GNU General Public License as published by\n"
            "the Free Software Foundation; either version 3 of the License, or\n"
            "(at your option) any later version.\n"
            "\n"
            "This program is distributed in the hope that it will be useful,\n"
            "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
            "GNU General Public License for more details.\n"
            "\n"
            "You should have received a copy of the GNU General Public License\n"
            "along with this program.  If not, see <http://www.gnu.org/licenses/>.";

        /* Create the about dialog */
        GtkWidget *about_dialog = gtk_about_dialog_new ();

        gtk_window_set_icon_name (GTK_WINDOW (about_dialog), "fortuner");
        gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG (about_dialog),
        "Fortuner");
        gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (about_dialog),
                VERSION_STRING);
        gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (about_dialog),
                "Show a fortune as a notification.");

        gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (about_dialog),
                "Copyright (C) 2012 Juhani Numminen");
        gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (about_dialog),
                authors);

        gtk_about_dialog_set_license (GTK_ABOUT_DIALOG (about_dialog),
                license);
        gtk_about_dialog_set_logo_icon_name (GTK_ABOUT_DIALOG (about_dialog),
                "fortuner");

        // Run the about dialog
        gtk_dialog_run (GTK_DIALOG (about_dialog));
        gtk_widget_destroy (about_dialog);
    }
}

static void popup_menu (GtkStatusIcon *status_icon, guint button,
    guint activate_time, gpointer user_data)
{
    if (!menu)
    {
        GtkWidget *item;
        menu = gtk_menu_new ();
        
        item = gtk_image_menu_item_new_from_stock (GTK_STOCK_CLOSE, NULL);
        gtk_menu_item_set_label (GTK_MENU_ITEM (item), "Close Fortunes");
        gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
        g_signal_connect (G_OBJECT (item), "activate",
                G_CALLBACK (close_notifications),
                NULL);
        
        item = gtk_image_menu_item_new_from_stock (GTK_STOCK_ABOUT, NULL);
        gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
        g_signal_connect (G_OBJECT (item), "activate",
                G_CALLBACK (show_about_dialog),
                NULL);

        item = gtk_image_menu_item_new_from_stock (GTK_STOCK_QUIT, NULL);
        gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
        g_signal_connect (G_OBJECT (item), "activate",
                G_CALLBACK (destroy), 
                NULL);
    }
    
    gtk_widget_show_all (menu);

    gtk_menu_popup (GTK_MENU (menu),
            NULL,
            NULL,
            gtk_status_icon_position_menu,
            status_icon,
            button,
            activate_time);
}

void display_status_icon ()
{
    status_icon = gtk_status_icon_new_from_icon_name ("fortuner");
    gtk_status_icon_set_visible (status_icon, TRUE); 

    gtk_status_icon_set_tooltip_text (status_icon, "Fortuner");

    // Connect signals
    g_signal_connect (G_OBJECT (status_icon), "popup-menu",
              G_CALLBACK (popup_menu), NULL);

    g_signal_connect (G_OBJECT (status_icon), "activate",
              G_CALLBACK (activate), NULL);
}

// Display an error dialog with some text in it.
void display_error_dialog (const char *message)
{
    GtkWidget *dialog = gtk_message_dialog_new (NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "%s",
            message);
    gtk_window_set_title (GTK_WINDOW (dialog), "Error");
    gtk_window_set_icon_name (GTK_WINDOW (dialog), "fortuner");
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

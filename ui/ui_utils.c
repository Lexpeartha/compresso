#include "ui_utils.h"

int setup_ui(GtkWidget *window) {
    GtkWidget *container;

    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);

    setup_header(window, container);

    gtk_window_set_child(GTK_WINDOW(window), container);

    return 0;
}

int setup_header(GtkWidget *window, GtkWidget *container) {
    GtkWidget *header;

    header = gtk_header_bar_new();
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header), gtk_label_new("Compresso"));

    GtkWidget *menu_button = gtk_menu_button_new();
    gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(menu_button), "open-menu-symbolic");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), menu_button);

    GtkWidget *popover = gtk_popover_new();
    gtk_popover_set_child(GTK_POPOVER(popover), gtk_label_new("Menu"));
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(menu_button), popover);

    gtk_window_set_titlebar(GTK_WINDOW(window), header);

    return 0;
}
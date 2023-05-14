#include <stdio.h>
#include <gtk/gtk.h>

#ifndef COMPRESSO_UI_UTILS_H
#define COMPRESSO_UI_UTILS_H

// MAIN
int ui_main();

// UTILS
int setup_ui(GtkWidget *window);
int setup_header(GtkWidget *window, GtkWidget *container);
int setup_body(GtkWidget *container);
int setup_footer(GtkWidget *container);

#endif

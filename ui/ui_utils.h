#include <stdio.h>
#include <gtk/gtk.h>

#ifndef COMPRESSO_UI_UTILS_H
#define COMPRESSO_UI_UTILS_H

// MAIN
int ui_main();

// UTILS
#define PADDING 16
#define INNER_PADDING 4

int setup_ui(GtkWidget *window);
int setup_header(GtkWidget *window);
int setup_body(GtkWidget *grid);
int initiate_log_container(GtkWidget *grid);
int initiate_files_container(GtkWidget *grid);
int initiate_controls_container(GtkWidget *grid);

void clear_log_buffer();
void append_to_log_buffer(char *text);
void save_log_buffer();

typedef struct grid_position_t {
    int column;
    int row;
    int width;
    int height;
} grid_position;

#endif

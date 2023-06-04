#include <stdio.h>
#include <gtk/gtk.h>

#ifndef COMPRESSO_UI_UTILS_H
#define COMPRESSO_UI_UTILS_H

// MAIN
int ui_main(int argc, char *argv[]);

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
void finish_save_dialog(GObject* source_object, GAsyncResult* res, gpointer user_data);
void save_log_buffer();
void show_about_dialog();
void update_files_list();
void clear_files();
void add_file(char *file);
void finish_open_multiple_dialog(GObject* source_object, GAsyncResult* res, gpointer user_data);
void show_add_files_dialog();
void begin_process();
void on_window_closed();

typedef struct grid_position_t {
    int column;
    int row;
    int width;
    int height;
} grid_position;

#endif

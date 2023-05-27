#include "ui_utils.h"

// Variables I need to access from multiple places
GtkTextBuffer* log_buffer = NULL;
GtkWindow* global_window = NULL;

int setup_ui(GtkWidget *window) {
    // Initiates GridLayout
    GtkWidget *grid = gtk_grid_new();
    global_window = GTK_WINDOW(window);

    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(grid), PADDING);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(grid), PADDING);
    gtk_widget_set_vexpand(grid, TRUE);

    // Adds some padding to the grid
    gtk_widget_set_margin_top(grid, PADDING);
    gtk_widget_set_margin_bottom(grid, PADDING);
    gtk_widget_set_margin_start(grid, PADDING);
    gtk_widget_set_margin_end(grid, PADDING);

    // Setup necessary containers
    setup_header(window);
    setup_body(grid);

    // Adds the grid to the window
    gtk_window_set_child(GTK_WINDOW(window), grid);

    return 0;
}

int setup_header(GtkWidget *window) {
    GtkWidget *header;

    header = gtk_header_bar_new();
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header), gtk_label_new("Compresso"));
    gtk_window_set_titlebar(GTK_WINDOW(window), header);

    return 0;
}

int setup_body(GtkWidget *grid) {
    initiate_log_container(grid);
    initiate_files_container(grid);
    initiate_controls_container(grid);

    return 0;
}

int initiate_log_container(GtkWidget *grid) {
    GtkWidget *log_frame = gtk_frame_new("Logs:");
    grid_position position = {0, 0, 3, 3};

    GtkWidget *log_view = gtk_text_view_new();
    log_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log_view));
    gtk_widget_set_vexpand(log_view, TRUE);
    gtk_widget_set_hexpand(log_view, TRUE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(log_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(log_view), FALSE);

    gtk_widget_set_margin_top(log_view, INNER_PADDING);
    gtk_widget_set_margin_bottom(log_view, INNER_PADDING);
    gtk_widget_set_margin_start(log_view, INNER_PADDING);
    gtk_widget_set_margin_end(log_view, INNER_PADDING);

    GtkWidget *log_buttons_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, INNER_PADDING);
    gtk_widget_set_halign(log_buttons_container, GTK_ALIGN_END);
    GtkWidget *log_clear_button = gtk_button_new_with_label("Clear");
    g_signal_connect(log_clear_button, "clicked", G_CALLBACK(clear_log_buffer), NULL);
    GtkWidget *log_save_button = gtk_button_new_with_label("Save to file");
    g_signal_connect(log_save_button, "clicked", G_CALLBACK(save_log_buffer), NULL);
    gtk_box_append(GTK_BOX(log_buttons_container), log_clear_button);
    gtk_box_append(GTK_BOX(log_buttons_container), log_save_button);

    GtkWidget *wrapper = gtk_box_new(GTK_ORIENTATION_VERTICAL, INNER_PADDING);
    GtkWidget *log_view_scrollable_container = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(log_view_scrollable_container), log_view);
    gtk_box_append(GTK_BOX(wrapper), log_buttons_container);
    gtk_box_append(GTK_BOX(wrapper), log_view_scrollable_container);
    gtk_frame_set_child(GTK_FRAME(log_frame), wrapper);

    for (int i = 0; i < 50; i++) {
        char buffer[1024];
        sprintf(buffer, "Log number %d\n", i);
        gtk_text_buffer_insert_at_cursor(log_buffer, buffer, -1);
    }

    gtk_grid_attach(GTK_GRID(grid), log_frame, position.column, position.row, position.width, position.height);

    return 0;
}

int initiate_files_container(GtkWidget *grid) {
    GtkWidget *files_frame = gtk_frame_new("Chosen Files:");
    grid_position position = {0, 3, 3, 3};

    gtk_grid_attach(GTK_GRID(grid), files_frame, position.column, position.row, position.width, position.height);

    return 0;
}

int initiate_controls_container(GtkWidget *grid) {
    GtkWidget *controls_frame = gtk_frame_new("Controls:");
    grid_position position = {4, 0, 3, 6};

    gtk_grid_attach(GTK_GRID(grid), controls_frame, position.column, position.row, position.width, position.height);

    return 0;
}

void clear_log_buffer() {
    gtk_text_buffer_set_text(log_buffer, "", -1);
}

void append_to_log_buffer(char *text) {
    gtk_text_buffer_insert_at_cursor(log_buffer, text, -1);
}

void save_log_buffer() {
    GtkFileDialog* dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Save log to file");

    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(log_buffer, &start);
    gtk_text_buffer_get_end_iter(log_buffer, &end);

    char *text = gtk_text_buffer_get_text(log_buffer, &start, &end, FALSE);

    FILE *file = fopen(filename, "w");
    fprintf(file, "%s", text);
    fclose(file);
}
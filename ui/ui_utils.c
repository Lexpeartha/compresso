#include "ui_utils.h"
#include "deflate.h"

// Variables I need to access from multiple places
GtkTextBuffer *log_buffer = NULL;
GtkWindow *global_window = NULL;
GtkWidget *compression_switch = NULL;
GtkWidget *files_list = NULL;
GtkWidget *spinner = NULL;
GActionGroup *algorithm_action_group = NULL;
char **files = NULL;
char *current_file = NULL;
ushort is_algorithm_running = 0;

// Values needed for parallelization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

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
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_closed), NULL);

    // Set up variables used throughout the program
    files = calloc(1, sizeof(char *));
    files[0] = NULL;

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
    gtk_widget_set_margin_end(log_buttons_container, INNER_PADDING);

    GtkWidget *wrapper = gtk_box_new(GTK_ORIENTATION_VERTICAL, INNER_PADDING);
    GtkWidget *log_view_scrollable_container = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(log_view_scrollable_container), log_view);
    gtk_box_append(GTK_BOX(wrapper), log_buttons_container);
    gtk_box_append(GTK_BOX(wrapper), log_view_scrollable_container);
    gtk_frame_set_child(GTK_FRAME(log_frame), wrapper);

    gtk_grid_attach(GTK_GRID(grid), log_frame, position.column, position.row, position.width, position.height);

    return 0;
}

int initiate_files_container(GtkWidget *grid) {
    GtkWidget *files_frame = gtk_frame_new("Chosen Files:");
    grid_position position = {0, 3, 3, 3};

    GtkWidget *clear_files_button = gtk_button_new_with_label("Clear");
    g_signal_connect(clear_files_button, "clicked", G_CALLBACK(clear_files), NULL);
    GtkWidget *add_files_button = gtk_button_new_with_label("Add");
    g_signal_connect(add_files_button, "clicked", G_CALLBACK(show_add_files_dialog), NULL);
    GtkWidget *files_buttons_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, INNER_PADDING);
    gtk_widget_set_halign(files_buttons_container, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(files_buttons_container), clear_files_button);
    gtk_box_append(GTK_BOX(files_buttons_container), add_files_button);

    files_list = gtk_list_box_new();
    gtk_widget_set_vexpand(files_list, TRUE);
    gtk_widget_set_hexpand(files_list, TRUE);
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(files_list), GTK_SELECTION_NONE);
    gtk_list_box_set_activate_on_single_click(GTK_LIST_BOX(files_list), FALSE);

    GtkWidget *files_list_scrollable_container = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(files_list_scrollable_container), files_list);

    GtkWidget *wrapper = gtk_box_new(GTK_ORIENTATION_VERTICAL, INNER_PADDING);
    gtk_box_append(GTK_BOX(wrapper), files_buttons_container);
    gtk_box_append(GTK_BOX(wrapper), files_list_scrollable_container);
    gtk_frame_set_child(GTK_FRAME(files_frame), wrapper);

    gtk_widget_set_margin_top(wrapper, INNER_PADDING);
    gtk_widget_set_margin_bottom(wrapper, INNER_PADDING);
    gtk_widget_set_margin_start(wrapper, INNER_PADDING);
    gtk_widget_set_margin_end(wrapper, INNER_PADDING);

    gtk_grid_attach(GTK_GRID(grid), files_frame, position.column, position.row, position.width, position.height);

    return 0;
}

int initiate_controls_container(GtkWidget *grid) {
    GtkWidget *controls_frame = gtk_frame_new("Controls:");
    grid_position position = {4, 0, 3, 6};

    GtkWidget *switch_label = gtk_label_new("Compress/Decompress:");
    compression_switch = gtk_switch_new();
    GtkWidget *switch_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, INNER_PADDING);
    gtk_box_append(GTK_BOX(switch_box), switch_label);
    gtk_box_append(GTK_BOX(switch_box), compression_switch);

    GtkWidget *wrapper = gtk_box_new(GTK_ORIENTATION_VERTICAL, INNER_PADDING);
    gtk_box_append(GTK_BOX(wrapper), switch_box);
    gtk_widget_set_margin_top(wrapper, INNER_PADDING);
    gtk_widget_set_margin_bottom(wrapper, INNER_PADDING);
    gtk_widget_set_margin_start(wrapper, INNER_PADDING);
    gtk_widget_set_margin_end(wrapper, INNER_PADDING);

    spinner = gtk_spinner_new();
    GtkWidget *about_button = gtk_button_new_with_label("About");
    g_signal_connect(about_button, "clicked", G_CALLBACK(show_about_dialog), NULL);
    GtkWidget *begin_button = gtk_button_new_with_label("Begin");
    g_signal_connect(begin_button, "clicked", G_CALLBACK(begin_process), NULL);

    // algorithm_action_group = gtk_action_group_new("AlgorithmActionGroup");
    GtkWidget *algorithm_selection_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, INNER_PADDING);
    gtk_box_append(GTK_BOX(algorithm_selection_box), gtk_label_new("Choose the algorithm:"));
    GtkWidget *deflate_btn = gtk_check_button_new_with_label("Standard deflate (LZW + Adaptive Huffman)");
    GtkWidget *static_deflate_btn = gtk_check_button_new_with_label("Static deflate (LZW + Static Huffman)");
    GtkWidget *lzw_btn = gtk_check_button_new_with_label("LZW");
    GtkWidget *adaptive_huffman_btn = gtk_check_button_new_with_label("Adaptive Huffman");
    GtkWidget *static_huffman_btn = gtk_check_button_new_with_label("Static Huffman");
    gtk_check_button_set_group(GTK_CHECK_BUTTON(static_deflate_btn), GTK_CHECK_BUTTON(deflate_btn));
    gtk_check_button_set_group(GTK_CHECK_BUTTON(lzw_btn), GTK_CHECK_BUTTON(deflate_btn));
    gtk_check_button_set_group(GTK_CHECK_BUTTON(adaptive_huffman_btn), GTK_CHECK_BUTTON(deflate_btn));
    gtk_check_button_set_group(GTK_CHECK_BUTTON(static_huffman_btn), GTK_CHECK_BUTTON(deflate_btn));
    gtk_box_append(GTK_BOX(algorithm_selection_box), deflate_btn);
    gtk_box_append(GTK_BOX(algorithm_selection_box), static_deflate_btn);
    gtk_box_append(GTK_BOX(algorithm_selection_box), lzw_btn);
    gtk_box_append(GTK_BOX(algorithm_selection_box), adaptive_huffman_btn);
    gtk_box_append(GTK_BOX(algorithm_selection_box), static_huffman_btn);
    gtk_box_append(GTK_BOX(wrapper), algorithm_selection_box);

    GtkWidget *control_buttons_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, INNER_PADDING);
    gtk_box_append(GTK_BOX(control_buttons_container), spinner);
    gtk_box_append(GTK_BOX(control_buttons_container), about_button);
    gtk_box_append(GTK_BOX(control_buttons_container), begin_button);
    gtk_widget_set_halign(control_buttons_container, GTK_ALIGN_END);

    gtk_box_append(GTK_BOX(wrapper), control_buttons_container);

    gtk_frame_set_child(GTK_FRAME(controls_frame), wrapper);
    gtk_grid_attach(GTK_GRID(grid), controls_frame, position.column, position.row, position.width, position.height);

    return 0;
}

void clear_log_buffer() {
    gtk_text_buffer_set_text(log_buffer, "", -1);
}

void append_to_log_buffer(char *text) {
    gtk_text_buffer_insert_at_cursor(log_buffer, text, -1);
}

void finish_save_dialog(GObject *source_object, GAsyncResult *res, gpointer text) {
    GFile *save_file = NULL;
    save_file = gtk_file_dialog_save_finish(GTK_FILE_DIALOG(source_object), res, NULL);

    if (save_file != NULL) {
        char *filename = g_file_get_path(save_file);
        FILE *file = fopen(filename, "w");
        fprintf(file, "%s", (char *) text);
        fclose(file);
        g_free(filename);
    }
}

void save_log_buffer() {
    GtkFileDialog *dialog = gtk_file_dialog_new();

    gtk_file_dialog_set_initial_name(dialog, "log.txt");

    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(log_buffer, &start);
    gtk_text_buffer_get_end_iter(log_buffer, &end);

    char *text = gtk_text_buffer_get_text(log_buffer, &start, &end, FALSE);

    gtk_file_dialog_save(dialog, global_window, NULL, finish_save_dialog, text);
}

void show_about_dialog() {
    GtkWidget *dialog = gtk_about_dialog_new();

    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "Compresso");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Compresso is a compression tool written in C. "
                                                            "It is a university project at Faculty Of Electrical Engineering for a course "
                                                            "called Practicum in Programming 2, which covers practical usages of C programming language.");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://github.com/Lexpeartha/compresso");
    gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog), "MIT License");

    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(global_window));
    gtk_widget_set_visible(dialog, TRUE);
}

void update_files_list() {
    GtkListBoxRow *row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(files_list), 0);
    while (row != NULL) {
        gtk_list_box_remove(GTK_LIST_BOX(files_list), GTK_WIDGET(row));
        row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(files_list), 0);
    }

    for (int i = 0; files[i] != NULL; i++) {
        // g_print("File number %d selected: %s\n", i + 1, files[i]);
        gtk_list_box_append(GTK_LIST_BOX(files_list), gtk_label_new(files[i]));
    }
}

void clear_files() {
    for (int i = 0; files[i] != NULL; i++) {
        free(files[i]);
    }
    free(files);
    files = calloc(1, sizeof(char *));
    files[0] = NULL;
    update_files_list();
}

void add_file(char *file) {
    unsigned n = 0;
    for (int i = 0; files[i] != NULL; i++) {
        n++;
    }
    char **tmp = realloc(files, (n + 2) * sizeof(char *));
    if (tmp == NULL) {
        g_warning("Failed to allocate memory for files array");
        return;
    }
    files = tmp;
    files[n] = calloc(strlen(file) + 1, sizeof(char));
    strcpy(files[n], file);
    files[n + 1] = NULL;
    update_files_list();
}

void finish_open_multiple_dialog(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GListModel *selected_files = gtk_file_dialog_open_multiple_finish(GTK_FILE_DIALOG(source_object),
                                                                      res,
                                                                      NULL);
    if (selected_files != NULL) {
        unsigned selected_files_length = g_list_model_get_n_items(selected_files);
        for (int i = 0; i < selected_files_length; i++) {
            GFile *file = g_list_model_get_item(selected_files, i);
            add_file(g_file_get_path(file));
        }
    }
}

void show_add_files_dialog() {
    GtkFileDialog *dialog = gtk_file_dialog_new();

    gtk_file_dialog_open_multiple(dialog, global_window, NULL, finish_open_multiple_dialog, NULL);
}

void *algorithm_thread(void *arg) {
    if (current_file == NULL)
        return NULL;

    algorithm_fn algorithm = (algorithm_fn) arg;
    is_algorithm_running = 1;
    gtk_spinner_set_spinning(GTK_SPINNER(spinner), TRUE);

    // ALGORITHM LOGIC HERE
    algorithm(current_file, append_to_log_buffer);

    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    is_algorithm_running = 0;
    gtk_spinner_set_spinning(GTK_SPINNER(spinner), FALSE);

    pthread_exit(NULL);
}

void begin_process() {
    if (files[0] == NULL) {
        g_print("No files selected\n");
        return;
    }

    if (is_algorithm_running)
        return;

    if (!gtk_switch_get_active(GTK_SWITCH(compression_switch))) {
        compress_ui();
    } else {
        decompress_ui();
    }
}

int compress_ui() {
    pthread_t thread;
    current_file = NULL;

    for (int i = 0; files[i] != NULL; i++) {
        if (is_algorithm_running) {
            // Waiting for the previous thread to finish if the algorithm is already running
            pthread_join(thread, NULL);
        }
        current_file = files[i];
        pthread_create(&thread, NULL, algorithm_thread, (void *) lzw_compression);
    }

    return 0;
}

int decompress_ui() {
    pthread_t thread;
    current_file = NULL;

    for (int i = 0; files[i] != NULL; i++) {
        if (is_algorithm_running) {
            // Waiting for the previous thread to finish if the algorithm is already running
            pthread_join(thread, NULL);
        }
        current_file = files[i];
        pthread_create(&thread, NULL, algorithm_thread, (void *) lzw_decompression);
    }

    return 0;
}

void on_window_closed() {
    for (int i = 0; files[i] != NULL; i++) {
        free(files[i]);
    }
    free(files);
}
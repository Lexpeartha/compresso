#include "ui_utils.h"

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;

    window = gtk_application_window_new (app);
    gtk_widget_set_size_request(GTK_WIDGET(window), 1024, 768);

    if(setup_ui(window)) {
        exit(1);
    }

    gtk_window_present (GTK_WINDOW (window));
}

int ui_main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.etf.compresso", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
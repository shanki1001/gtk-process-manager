#include<gtk/gtk.h>
#include "gui.h"
#include "process.h"

int main(int argc, char**argv) {

    GtkApplication *app = gtk_application_new("com.example.ProcessManager", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(build_gui), NULL);
    int status = g_application_run(G_APPLICATION(app),argc , argv);
    g_object_unref(app);
    return status;

}

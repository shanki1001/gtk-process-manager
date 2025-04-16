#include<gtk/gtk.h>
#include "process.h"

static Process *processList = NULL; //global linekd list head
static GtkWidget *list_box;

void update_process_list() {
    gtk_list_box_invalidate_filter(GTK_LIST_BOX(list_box));
    gtk_container_foreach(GTK_CONTAINER(list_box),(GtkCallback)gtk_widget_destroy, NULL);
    Process *temp = processList;
    while(temp){
        char info[256];
        snprintf(info, sizeof(info), "PID: %d | %s | %d MB | %s",temp->pid,temp->name,temp->memoryUsage, temp->status == RUNNING? "RUNNING" : "TERMINATED");
        GtkWidget *label = gtk_label_new(info);
        gtk_list_box_insert(GTK_LIST_BOX(list_box),label,-1);

        temp = temp->next;
    }
    gtk_widget_show_all(list_box);
}
void on_add_process(GtkButton *button,gpointer user_data){
    printf("Callback triggered.\n");
if (!user_data) {
    printf("user_data is NULL!\n");
    return;
}
GtkWidget **widgets = user_data;
if (!widgets[0] || !widgets[1]) {
    printf("Widget pointers are NULL!\n");
    return;
}

  //  GtkWidget **widgets = user_data;
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(widgets[0]));
    int memory = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets[1]));

    if(strlen(name)==0 || memory <=0) return;
    Process *new_proc = create_process(name,memory);
    add_process(&processList, new_proc);
    update_process_list();

    gtk_entry_set_text(GTK_ENTRY(widgets[0]), "");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widgets[1]),0);

}

void on_terminate_process (GtkButton *button , gpointer user_data){
    GtkWidget *terminate_spin = GTK_WIDGET(user_data);
    int pid = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(terminate_spin));
    if(pid<=0) return;
    terminate_process(processList, pid);
    update_process_list();
}

void on_delete_all_processes(GtkButton *button , gpointer user_data) {
    if(processList == NULL) return;
    free_process_list(processList);
    processList = NULL;
    update_process_list();
    printf("All processes deleted.\n");
}


void build_gui(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *vbox, *name_entry , *memory_spin , *add_button;
    GtkWidget *name_label, *mem_label;
    GtkWidget *input_grid;
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window),"Process Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window),vbox);

    input_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(input_grid),5);
    gtk_grid_set_column_spacing(GTK_GRID(input_grid),10);
    gtk_box_pack_start(GTK_BOX(vbox),input_grid, FALSE,FALSE,10);

    //Name Entry
    name_label = gtk_label_new("Process Name:");
    gtk_grid_attach(GTK_GRID(input_grid), name_label, 0,0,1,1);
    name_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(input_grid),name_entry,1,0,1,1);

    //Memory Entry
    mem_label = gtk_label_new("Memory Usage (MB):");
    gtk_grid_attach(GTK_GRID(input_grid),mem_label,0,1,1,1);
    memory_spin = gtk_spin_button_new_with_range(1,4096,1);
    gtk_grid_attach(GTK_GRID(input_grid),memory_spin, 1,1,1,1);

    //Add Button
    add_button = gtk_button_new_with_label("Add Process");
    gtk_box_pack_start(GTK_BOX(vbox), add_button, FALSE, FALSE, 5);

    //Terminate Section
    GtkWidget *terminate_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(terminate_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(terminate_grid), 10);
    gtk_box_pack_start(GTK_BOX(vbox), terminate_grid, FALSE , FALSE, 10);

    GtkWidget *terminate_label = gtk_label_new("PID to Terminate:");
    gtk_grid_attach(GTK_GRID(terminate_grid), terminate_label, 0,0,1,1);

    GtkWidget *terminate_spin = gtk_spin_button_new_with_range(1, 10000, 1);
    gtk_grid_attach(GTK_GRID(terminate_grid), terminate_spin, 1,0,1,1);
    GtkWidget *terminate_button = gtk_button_new_with_label("Terminate Process");
    gtk_grid_attach(GTK_GRID(terminate_grid), terminate_button ,0,1,2,1);

    g_signal_connect(terminate_button ,"clicked", G_CALLBACK(on_terminate_process),terminate_spin);

    GtkWidget *delete_all_button = gtk_button_new_with_label("Delete All processes");
    gtk_box_pack_start(GTK_BOX(vbox), delete_all_button, FALSE, FALSE, 5);

    g_signal_connect(delete_all_button, "clicked", G_CALLBACK(on_delete_all_processes),NULL);

    // ListBox
    list_box = gtk_list_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), list_box, TRUE, TRUE, 10);

    // Pass widgets to callback
    GtkWidget **input_widgets = g_malloc(sizeof(GtkWidget*) *2);
    input_widgets[0] = name_entry;
    input_widgets[1]= memory_spin;
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_process),input_widgets);

    gtk_widget_show_all(window);
}

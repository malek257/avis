#include <gtk/gtk.h>


void
on_treeview2_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
on_update_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

void
on_save_clicked                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_Good_toggled                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_Medium_toggled                      (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_Bad_toggled                         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_Find_clicked                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_send_clicked                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_envoyer_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_OK_clicked                          (GtkButton       *button,
                                        gpointer         user_data);

void
on_confirm_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_delete_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

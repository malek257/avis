#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "Avis1.h"

int type = 1;

void on_save_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *id_entry, *feedback_entry, *parking_combo, *note_combo;
    GtkWidget *day_spin, *month_spin, *year_spin, *check_s, *check_g;
    GtkWidget *status_label;

    Avis a;
    a.rating1 = type;

    id_entry = lookup_widget(button, "entry222");
    feedback_entry = lookup_widget(button, "entry44");
    parking_combo = lookup_widget(button, "combopark");
    note_combo = lookup_widget(button, "combonote");
    day_spin = lookup_widget(button, "day");
    month_spin = lookup_widget(button, "month");
    year_spin = lookup_widget(button, "year");
    check_s = lookup_widget(button, "check_sms");
    check_g = lookup_widget(button, "check_Gmail");
    status_label = lookup_widget(button, "label83");

    // Récupération des valeurs des champs
    snprintf(a.id, sizeof(a.id), "%s", gtk_entry_get_text(GTK_ENTRY(id_entry)));
    snprintf(a.feedback, sizeof(a.feedback), "%s", gtk_entry_get_text(GTK_ENTRY(feedback_entry)));

    const gchar *parking_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(parking_combo));
    const gchar *note_text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(note_combo));

    if (parking_name != NULL) {
        strncpy(a.parkingName, parking_name, sizeof(a.parkingName));
    } else {
        strcpy(a.parkingName, "");
    }

    if (note_text != NULL) {
        strncpy(a.nott, note_text, sizeof(a.nott));
    } else {
        strcpy(a.nott, "");
    }

    a.day = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(day_spin));
    a.month = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(month_spin));
    a.year = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(year_spin));

    a.option[0] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_g)) ? 1 : 0;
    a.option[1] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_s)) ? 1 : 0;

    // Sauvegarde
    int resultat = ajouterAvis("Avis1.txt", a);

    if (resultat) {
        gtk_label_set_text(GTK_LABEL(status_label), "Review added successfully!");
    } else {

        gtk_label_set_text(GTK_LABEL(status_label), "Failed to add Review.");
    }
}

void on_Good_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        type = 1;
    }
}

void on_Medium_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        type = 2;
    }
}

void on_Bad_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    if (gtk_toggle_button_get_active(togglebutton)) {
        type = 3;
    }
}

void on_Find_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *id_entry, *feedback_entry, *parking_combo, *note_combo;
    GtkWidget *day_spin, *month_spin, *year_spin, *check_s, *check_g, *status_label;
    GtkWidget *good_radio, *medium_radio, *bad_radio;

    // Références des widgets
    id_entry = lookup_widget(button, "entry3");
    feedback_entry = lookup_widget(button, "entry223");
    parking_combo = lookup_widget(button, "combo_p");
    note_combo = lookup_widget(button, "combo_n");
    day_spin = lookup_widget(button, "spinD");
    month_spin = lookup_widget(button, "spinM");
    year_spin = lookup_widget(button, "spinY");
    check_s = lookup_widget(button, "check_S");
    check_g = lookup_widget(button, "check_G");
    good_radio = lookup_widget(button, "radio_g");
    medium_radio = lookup_widget(button, "radio_m");
    bad_radio = lookup_widget(button, "radio_b");
    status_label = lookup_widget(button, "label85");

    // Récupération de l'ID de l'avis à rechercher
    const char *search_id = gtk_entry_get_text(GTK_ENTRY(id_entry));

    // Initialisation de la structure Avis
    Avis a = chercherAvis("Avis1.txt", search_id);

    // Vérification si l'avis a été trouvé
    if (strcmp(a.id, "") == 0) {
        gtk_label_set_text(GTK_LABEL(status_label), "Review ID not found.");
        return;
    }

    // Mise à jour des champs
    gtk_entry_set_text(GTK_ENTRY(feedback_entry), a.feedback);
int get_parking_index(const char *parkingName) {
    // Exemple de logique :
    // 1. Chercher le parking dans la liste.
    // 2. Retourner son indice.
    return 0; // Retourne l'indice par défaut pour le moment.
}

	 gtk_combo_box_set_active(GTK_COMBO_BOX(parking_combo), get_parking_index(a.parkingName));
	

   gtk_combo_box_set_active(GTK_COMBO_BOX(note_combo), atoi(a.nott) - 1);  // Sélectionner l'élément
   sprintf(a.nott, "%d", gtk_combo_box_get_active(GTK_COMBO_BOX(note_combo)) + 1);  // Mettre à jour a.nott

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(day_spin), a.day);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(month_spin), a.month);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(year_spin), a.year);

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_g), a.option[0]);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_s), a.option[1]);

    // Mise à jour des boutons radio
    if (a.rating1 == 1) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(good_radio), TRUE);
    } else if (a.rating1 == 2) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(medium_radio), TRUE);
    } else if (a.rating1 == 3) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bad_radio), TRUE);
    }

    gtk_label_set_text(GTK_LABEL(status_label), "Review found. Modify as needed.");
}

void on_send_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *id_entry, *feedback_entry, *parking_combo, *note_combo;
    GtkWidget *day_spin, *month_spin, *year_spin, *check_s, *check_g;
    GtkWidget *good_radio, *medium_radio, *bad_radio;
    GtkWidget *status_label;

    // Références des widgets
    id_entry = lookup_widget(button, "entry3");
    feedback_entry = lookup_widget(button, "entry223");
    parking_combo = lookup_widget(button, "combo_p");
    note_combo = lookup_widget(button, "combo_n");
    day_spin = lookup_widget(button, "spinD");
    month_spin = lookup_widget(button, "spinM");
    year_spin = lookup_widget(button, "spinY");
    check_s = lookup_widget(button, "check_S");
    check_g = lookup_widget(button, "check_G");
    good_radio = lookup_widget(button, "radio_g");
    medium_radio = lookup_widget(button, "radio_m");
    bad_radio = lookup_widget(button, "radio_b");
    status_label = lookup_widget(button, "label85");

    // Récupération de l'ID de l'avis
    const char *search_id = gtk_entry_get_text(GTK_ENTRY(id_entry));

    // Initialisation de la structure Avis
    Avis a = chercherAvis("Avis1.txt", search_id);

    // Vérification si l'avis a été trouvé
    if (strcmp(a.id, "") == 0) {
        gtk_label_set_text(GTK_LABEL(status_label), "Review ID not found.");
        return;
    }

    // Récupération des nouvelles valeurs des champs
    snprintf(a.feedback, sizeof(a.feedback), "%s", gtk_entry_get_text(GTK_ENTRY(feedback_entry)));
    a.rating1 = gtk_combo_box_get_active(GTK_COMBO_BOX(note_combo)) + 1;
    a.day = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(day_spin));
    a.month = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(month_spin));
    a.year = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(year_spin));
    a.option[0] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_g)) ? 1 : 0;
    a.option[1] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_s)) ? 1 : 0;

    // Récupération du parking sélectionné
    const char *selected_parking = gtk_combo_box_get_active_text(GTK_COMBO_BOX(parking_combo));
    snprintf(a.parkingName, sizeof(a.parkingName), "%s", selected_parking);

    // Mise à jour du rating
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(good_radio))) {
        a.rating1 = 1;
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(medium_radio))) {
        a.rating1 = 2;
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bad_radio))) {
        a.rating1 = 3;
    }

    // Modification de l'avis dans le fichier
    int result = modifierAvis("Avis1.txt", search_id, a);

    // Affichage du message de confirmation
    if (result) {
        gtk_label_set_text(GTK_LABEL(status_label), "Review modified successfully!");
    } else {
        gtk_label_set_text(GTK_LABEL(status_label), "Failed to modify review.");
    }
}





void
on_update_clicked(GtkButton *button, gpointer user_data)
{
    // Récupérer le TreeView où les avis sont affichés
    GtkWidget *liste = lookup_widget(GTK_WIDGET(button), "treeview2");

    if (!GTK_IS_TREE_VIEW(liste)) {
        g_warning("Le widget n'est pas un TreeView!");
        return;
    }

    // Appeler la fonction pour afficher ou actualiser les avis
    afficher_tous_les_avis(liste);

    // Optionnel : Afficher un message de confirmation à l'utilisateur
    gtk_label_set_text(GTK_LABEL(user_data), "Avis mis à jour avec succès!");
}


void
on_treeview2_row_activated             (GtkTreeView *treeview,
                               GtkTreePath *path,
                               GtkTreeViewColumn *column,
                               gpointer user_data)
{
    GtkTreeIter iter;
    gchar *id;
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);

    if (gtk_tree_model_get_iter(model, &iter, path)) {
        // Récupérer l'ID de la ligne sélectionnée (colonne 0 ici)
        gtk_tree_model_get(GTK_LIST_STORE(model), &iter, 0, &id, -1);

        // Appeler votre fonction supprimerAvis pour supprimer l'avis correspondant
        if (supprimerAvis("Avis1.txt", id)) {
            g_print("Avis with ID %s deleted successfully.\n", id);
        } else {
            g_print("Failed to delete Avis with ID %s.\n", id);
        }

        // Rafraîchir le GtkTreeView pour afficher les données mises à jour
        afficher_tous_les_avis(GTK_WIDGET(treeview)); // Assurez-vous que cette fonction recharge les données
        g_free(id);
    }
}
	


void
on_OK_clicked                          (GtkButton       *button,
                                        gpointer         user_data)
{

  
    // Récupération des widgets nécessaires
    GtkWidget *tree = lookup_widget(GTK_WIDGET(button), "treeview3"); // TreeView pour afficher les résultats
    GtkWidget *entry = lookup_widget(GTK_WIDGET(button), "entry225"); // Entry pour saisir l'ID
    GtkWidget *label = lookup_widget(GTK_WIDGET(button), "label110"); // Label pour afficher les messages

    // Vérification de la présence des widgets
    if (!tree || !entry || !label) {
        g_warning("Un ou plusieurs widgets sont introuvables.");
        return;
    }

    // Récupération de l'ID entré
    const char *avis_id = gtk_entry_get_text(GTK_ENTRY(entry));

    // Vérification si l'ID n'est pas vide
    if (strlen(avis_id) == 0) {
        gtk_label_set_text(GTK_LABEL(label), "Veuillez entrer un ID valide.");
        return;
    }

    // Appel de la fonction pour afficher les avis
    afficher_avis_par_id(tree, avis_id);

    // Vérification si des données sont ajoutées au TreeView
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
    GtkTreeIter iter;

    if (gtk_tree_model_get_iter_first(model, &iter)) {
        // Si des données existent dans le modèle, l'ID est valide
        gtk_label_set_text(GTK_LABEL(label), "Avis trouvé avec succès.");
    } else {
        // Si aucune donnée n'a été ajoutée, l'ID n'existe pas
        gtk_label_set_text(GTK_LABEL(label), "Aucun avis trouvé pour cet ID.");
    }
}



int confirmation_result = 0; // 0 = No, 1 = Yes


void
on_confirm_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

    // Récupérer les widgets des boutons radio
    GtkWidget *yes = lookup_widget(GTK_WIDGET(button), "radiobutton2"); // Remplacer par le bon ID de votre bouton radio "Yes"
    GtkWidget *no = lookup_widget(GTK_WIDGET(button), "radiobutton1");  // Remplacer par le bon ID de votre bouton radio "No"

    // Vérifier quel bouton radio est activé
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(yes))) { 
        confirmation_result = 1; // Yes sélectionné
    } else {
        confirmation_result = 0; // No sélectionné
    }
}




void on_delete_clicked(GtkButton *button, gpointer user_data) {
    // Récupérer les widgets nécessaires
    GtkWidget *entry_delete_id = lookup_widget(GTK_WIDGET(button), "entry225"); // Remplacer par le bon ID de votre champ Entry
    GtkWidget *label = lookup_widget(GTK_WIDGET(button), "label109");           // Remplacer par le bon ID de votre label pour le feedback
	
    const char *filename = "Avis1.txt"; // Nom du fichier contenant les avis
    const char *avis_id = gtk_entry_get_text(GTK_ENTRY(entry_delete_id));

    // Vérifier si l'utilisateur a confirmé l'action
    if (confirmation_result == 1) {
        // Appel de votre fonction de suppression
        int result = supprimerAvis(filename, avis_id);

        if (result) {
            gtk_label_set_text(GTK_LABEL(label), "Avis supprimé avec succès.");
        } else {
            gtk_label_set_text(GTK_LABEL(label), "échec de suppression.");
        }
    } else {
        // L'utilisateur a annulé l'action
        gtk_label_set_text(GTK_LABEL(label), "Suppression annulée par l'utilisateur.");
    }
}

#include "Avis1.h"
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <ctype.h>
#define MAX_ID_LENGTH 5
// Fonction qui vérifie si l'ID respecte la longueur exacte et est mixte (lettres et chiffres)
int is_valid_id_type(char *id) {
    // Vérifier que l'ID n'est pas NULL et a exactement 5 caractères
    if (id == NULL || strlen(id) != 5) {
        printf("L'ID doit être exactement de %d caractères.\n", 5);
        return 0;
    }

    // Vérifier que l'ID contient au moins une lettre et un chiffre
    int has_letter = 0, has_digit = 0;
    for (int i = 0; id[i] != '\0'; i++) {
        if (isalpha(id[i])) {
            has_letter = 1;
        } else if (isdigit(id[i])) {
            has_digit = 1;
        } else {
            // Si l'ID contient un caractère qui n'est ni une lettre ni un chiffre
            printf("L'ID doit contenir uniquement des lettres et des chiffres.\n");
            return 0;
        }
    }

    // Si l'ID ne contient pas à la fois une lettre et un chiffre
    if (!has_letter || !has_digit) {
        printf("L'ID doit contenir à la fois des lettres et des chiffres.\n");
        return 0;
    }

    return 1; // L'ID est valide
}

// Fonction qui vérifie si l'ID est unique dans le fichier
int is_unique_id(char *filename, char *id) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;  // Si le fichier n'existe pas, retourner 1 (ID unique)
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char file_id[50];
        // Lire l'ID dans chaque ligne (supposons qu'il est au début de la ligne)
        sscanf(line, "%s", file_id);

        if (strcmp(file_id, id) == 0) {
            fclose(f);
            return 0;  // L'ID existe déjà
        }
    }

    fclose(f);
    return 1;  // L'ID est unique
}

// Fonction qui ajoute un avis dans un fichier
int ajouterAvis(char *filename, Avis a) {
    // Vérifier si l'ID est valide et respecte le type (longueur et mixte)
    if (!is_valid_id_type(a.id)) {
        return 0; // Le message d'erreur est déjà affiché dans is_valid_id_type
    }

    // Vérifier si l'ID est unique
    if (!is_unique_id(filename, a.id)) {
        printf("L'ID existe déjà.\n");
        return 0;
    }

    // Ouvrir le fichier en mode ajout
    FILE *f = fopen(filename, "a");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier pour ajouter un avis");
        return 0;
    }

    // Ajouter l'avis dans le fichier
    fprintf(f, "%s %s %d %d %d %s %d %s %d %d\n",
            a.id, a.parkingName, a.day, a.month, a.year, a.nott, a.rating1, a.feedback, a.option[0], a.option[1]);
    
    fclose(f);
    return 1;
}

// Fonction qui recherche un avis dans le fichier
Avis chercherAvis(char *filename, const char *id) {
    Avis a;
    int found = 0;

    // Vérifier si l'ID est valide et respecte le type (longueur et mixte)
    if (!is_valid_id_type((char *)id)) {
        strcpy(a.id, ""); // L'ID n'est pas valide
        return a;
    }

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier pour chercher un avis");
        strcpy(a.id, ""); // Aucun résultat trouvé
        return a;
    }

    // Lecture ligne par ligne
    while (fscanf(f, "%s %s %d %d %d %s %d %s %d %d\n",
                  a.id, a.parkingName, &a.day, &a.month, &a.year,
                  a.nott, &a.rating1, a.feedback, &a.option[0], &a.option[1]) != EOF) {
        if (strcmp(a.id, id) == 0) {
            found = 1;
            break;
        }
    }
    fclose(f);

    if (!found) {
        strcpy(a.id, ""); // Aucun résultat trouvé
    }

    return a;
}


	


// Modifier un avis
int modifierAvis(char *filename, const char *id, Avis nouv) {
    int tr = 0; // Variable pour suivre si l'avis a été trouvé et modifié
    Avis a;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("nouv.txt", "w");

    if (f == NULL || f2 == NULL) {
        // Gestion des erreurs d'ouverture de fichier
        if (f != NULL) fclose(f);
        if (f2 != NULL) fclose(f2);
        return 0; // Échec de la modification
    }

    // Parcours des avis dans le fichier
    while (fscanf(f, "%s %s %d %d %d %s %d %s %d %d\n",
                  a.id, a.parkingName, &a.day, &a.month, &a.year,
                  a.nott, &a.rating1, a.feedback, &a.option[0], &a.option[1]) != EOF) {
        if (strcmp(a.id, id) == 0) {
            // Écrire les nouvelles données pour l'avis modifié
            fprintf(f2, "%s %s %d %d %d %s %d %s %d %d\n",
                    nouv.id, nouv.parkingName, nouv.day, nouv.month, nouv.year,
                    nouv.nott, nouv.rating1, nouv.feedback, nouv.option[0], nouv.option[1]);
            tr = 1; // Marquer comme trouvé et modifié
        } else {
            // Écrire les données inchangées
            fprintf(f2, "%s %s %d %d %d %s %d %s %d %d\n",
                    a.id, a.parkingName, a.day, a.month, a.year,
                    a.nott, a.rating1, a.feedback, a.option[0], a.option[1]);
        }
    }

    fclose(f);
    fclose(f2);

    // Remplacer le fichier original par le fichier temporaire
    remove(filename);
    rename("nouv.txt", filename);

    return tr; // Retourne 1 si modifié, 0 sinon
}

// Supprimer un avis
int supprimerAvis(char *filename, const char *id) {
    int tr = 0;
    Avis a;
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier d'origine");
        return 0; // Échec de suppression
    }

    FILE *f2 = fopen("nouv.txt", "w");
    if (f2 == NULL) {
        perror("Erreur lors de l'ouverture du fichier temporaire");
        fclose(f);
        return 0; // Échec de suppression
    }

    while (fscanf(f, "%s %s %d %d %d %s %d %s %d %d\n",
                  a.id, a.parkingName, &a.day, &a.month, &a.year,
                  a.nott, &a.rating1, a.feedback, &a.option[0], &a.option[1]) != EOF) {
        if (strcmp(a.id, id) == 0) {
            tr = 1; // ID trouvé, ignorer l'écriture dans le fichier temporaire
        } else {
            fprintf(f2, "%s %s %d %d %d %s %d %s %d %d\n",
                    a.id, a.parkingName, a.day, a.month, a.year,
                    a.nott, a.rating1, a.feedback, a.option[0], a.option[1]);
        }
    }

    fclose(f);
    fclose(f2);

    // Remplacement atomique
    if (tr) {
        if (remove(filename) != 0) {
            perror("Erreur lors de la suppression du fichier d'origine");
            return 0; // Échec de suppression
        }
        if (rename("nouv.txt", filename) != 0) {
            perror("Erreur lors du renommage du fichier temporaire");
            return 0; // Échec de suppression
        }
    } else {
        // Supprimer le fichier temporaire s'il n'y a rien à supprimer
        remove("nouv.txt");
    }

    return tr; // Retourne 1 si l'élément a été supprimé, 0 sinon
}

int trierAvis(char *filename, Avis *avisArray, int *count) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return 0; // Échec
    }

    // Lecture des avis dans un tableau avec la structure spécifiée
    while (fscanf(f, "%s %s %d %d %d %s %d %s %d %d\n",
                  avisArray[*count].id, avisArray[*count].parkingName,
                  &avisArray[*count].day, &avisArray[*count].month, &avisArray[*count].year,
                  avisArray[*count].nott, &avisArray[*count].rating1, avisArray[*count].feedback,
                  &avisArray[*count].option[0], &avisArray[*count].option[1]) != EOF) {
        (*count)++;
    }
    fclose(f);

    // Tri des avis par ordre décroissant de la note (rating1) - Tri par sélection
    for (int i = 0; i < *count - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < *count; j++) {
            // Tri décroissant : si rating1[j] > rating1[maxIdx], on met à jour maxIdx
            if (avisArray[j].rating1 > avisArray[maxIdx].rating1) {
                maxIdx = j;
            }
        }
        // Échanger les éléments
        Avis temp = avisArray[i];
        avisArray[i] = avisArray[maxIdx];
        avisArray[maxIdx] = temp;
    }

    return 1; // Succès
}


void afficher_tous_les_avis(GtkWidget *liste) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeIter iter;
    GtkListStore *store;

    Avis avisArray[100]; 
    int count = 0;

    // Appel de la fonction trierAvis pour charger et trier les avis
    if (trierAvis("Avis1.txt", avisArray, &count) == 0) {
        g_warning("Erreur lors du tri des avis.");
        return;
    }

    // Obtenir le modèle actuel du TreeView
    store = gtk_tree_view_get_model(GTK_TREE_VIEW(liste));

    // Si le modèle existe déjà, on le réinitialise
    if (store) {
        gtk_list_store_clear(GTK_LIST_STORE(store));
    } else {
        // Créer un nouveau modèle
        store = gtk_list_store_new(10,
            G_TYPE_STRING,  // ID
            G_TYPE_STRING,  // Nom du parking
            G_TYPE_INT,     // Jour
            G_TYPE_INT,     // Mois
            G_TYPE_INT,     // Année
            G_TYPE_STRING,  // Note (nott)
            G_TYPE_STRING,  // Rating sous forme de texte
            G_TYPE_STRING,  // Feedback
            G_TYPE_BOOLEAN, // Option Gmail
            G_TYPE_BOOLEAN  // Option SMS
        );

        gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
        g_object_unref(store);

        // Ajouter les colonnes au TreeView
        renderer = gtk_cell_renderer_text_new();

        column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Parking Name", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Day", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Month", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Year", renderer, "text", 4, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Review Type", renderer, "text", 6, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Feedback", renderer, "text", 7, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Gmail", renderer, "text", 8, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("SMS", renderer, "text", 9, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Rating", renderer, "text", 5, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
    }

    // Tableau de conversion de rating1
    const char* ratingText[] = {"", "good", "medium", "bad"};

    // Ajouter les avis triés au modèle
    for (int i = 0; i < count; i++) {
        // Vérifier si rating1 est valide (entre 1 et 3)
        const char *ratingStr = (avisArray[i].rating1 >= 1 && avisArray[i].rating1 <= 3) 
                                ? ratingText[avisArray[i].rating1] 
                                : "unknown";

        gtk_list_store_append(GTK_LIST_STORE(store), &iter);
        gtk_list_store_set(GTK_LIST_STORE(store), &iter,
            0, avisArray[i].id,
            1, avisArray[i].parkingName,
            2, avisArray[i].day,
            3, avisArray[i].month,
            4, avisArray[i].year,
            5, avisArray[i].nott,      // Ajouter la note (en tant que chaîne)
            6, ratingStr,              // Rating converti en texte
            7, avisArray[i].feedback,  // Feedback
            8, avisArray[i].option[0], // Gmail
            9, avisArray[i].option[1], // SMS
            -1);
    }
}


    

void afficher_avis_par_id(GtkWidget *liste, const char *avis_id) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeIter iter;
    GtkListStore *store;
    Avis a;

    // Obtenir le modèle actuel du TreeView
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(liste)));

    // Si le modèle existe, on le vide
    if (store) {
        gtk_list_store_clear(store);
    } else {
        // Créer un nouveau modèle avec 10 colonnes
        store = gtk_list_store_new(10,
            G_TYPE_STRING,  // Avis ID
            G_TYPE_STRING,  // Parking Name
            G_TYPE_INT,     // Day
            G_TYPE_INT,     // Month
            G_TYPE_INT,     // Year
            G_TYPE_STRING,  // Note (nott)
            G_TYPE_STRING,  // Rating (Good, Medium, Bad)
            G_TYPE_STRING,  // Feedback
            G_TYPE_BOOLEAN, // Gmail Option (bool)
            G_TYPE_BOOLEAN  // SMS Option (bool)
        );

        gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
        g_object_unref(store);

        // Ajouter les colonnes au TreeView
        renderer = gtk_cell_renderer_text_new();

        column = gtk_tree_view_column_new_with_attributes("Avis ID", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Parking Name", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Day", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Month", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Year", renderer, "text", 4, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);


        column = gtk_tree_view_column_new_with_attributes("Review Type", renderer, "text", 6, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Feedback", renderer, "text", 7, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("Gmail", renderer, "text", 8, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        column = gtk_tree_view_column_new_with_attributes("SMS", renderer, "text", 9, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
	
	 column = gtk_tree_view_column_new_with_attributes("Rating", renderer, "text", 5, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
    }

    // Rechercher l'avis correspondant à l'ID
    a = chercherAvis("Avis1.txt", avis_id);

    if (strcmp(a.id, "") != 0) {
        // Ajouter l'avis trouvé au modèle
        const char *gmailOption = a.option[0] ? "Yes" : "No";
        const char *smsOption = a.option[1] ? "Yes" : "No";
        const char *reviewType = (a.rating1 == 1) ? "Good" : (a.rating1 == 2 ? "Medium" : "Bad");

        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            0, a.id,
            1, a.parkingName,
            2, a.day,
            3, a.month,
            4, a.year,
            5, a.nott,      // Note (caractère)
            6, reviewType,   // Review Type (Good/Medium/Bad)
            7, a.feedback,
            8, gmailOption,  // Gmail Option (Yes/No)
            9, smsOption,    // SMS Option (Yes/No)
            -1);
    } else {
        // Aucun avis trouvé
        GtkWidget *error_label = lookup_widget(GTK_WIDGET(liste), "label_error");
        if (error_label) {
            gtk_label_set_text(GTK_LABEL(error_label), "Aucun avis trouvé pour cet ID.");
        } else {
            g_warning("Label d'erreur introuvable !");
        }
    }


}









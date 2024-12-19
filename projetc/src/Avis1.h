#ifndef AVIS1_H_INCLUDED
#define AVIS1_H_INCLUDED

#include <stdio.h>
#include <gtk/gtk.h>

// Structure représentant un avis
typedef struct {
    char id[50];               
    char parkingName[50];      
    int day, month, year;    
    int  rating1 ; 
    char nott [50]  ;              
    char feedback[200];        
    int option[2];
	       
} Avis;

// Fonctions pour gérer les avis
int ajouterAvis(char *filename, Avis a);               
int modifierAvis(char *filename, const char *id, Avis nouv);
int supprimerAvis(char *filename, const char *id);     
Avis chercherAvis(char *filename, const char *id);     
int trierAvis(char *filename, Avis *avisArray, int *count);
int is_valid_id_type(char *id);  
int is_unique_id(char *filename, char *id);
void afficher_tous_les_avis(GtkWidget *liste);
void afficher_avis_par_id(GtkWidget *liste, const char *avis_id);

#endif

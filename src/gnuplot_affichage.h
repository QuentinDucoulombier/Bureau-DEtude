#ifndef _GNUPLOT_AFF_H
#define _GNUPLOT_AFF_H

#include "potentiel.h"

typedef struct
{
    int affichage;          // Booléen d'écriture pour la fonction RFK45
    char* data_file_path;   
    char* data_graph_path;
} affichage_struct;

/* Fonction qui crée le graph en png de la fonction d'onde
 *
 */
void affichage_fonction(affichage_struct affich);

/* Fonction qui crée le graph (en png) et le fichier csv de la fonction potentiel
 *
 */
void affichage_potentiel(potentiel pot);
#endif

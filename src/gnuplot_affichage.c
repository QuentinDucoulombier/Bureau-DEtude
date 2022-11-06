#include <stdio.h>
#include "gnuplot_affichage.h"
#include "../config.h"
#include "../config_affichage.h"

void affichage_fonction(affichage_struct affich)
{
    int nbr_commands = 8;

    /* liste des commandes à faire  */ 
    char *commands[] = {"reset\n",
                        "set terminal png\n",
                        "set output '%s'\n",
                        "set title 'Graph de la fonction d`onde phi\n",
                        "set xlabel 'x'\n",
                        "set ylabel 'phi(x)'\n",
                        "set datafile separator ';'\n",
                        "plot '%s' using 1:2 with lines \n"};

    /* ouvre le terminal de gnuplot */ 
    FILE *gnuplot = popen("gnuplot", "w");

    /* Lance les commandes  */
    for (int i = 0; i < nbr_commands; i++)
    {
        if (i == 2)
        {
            fprintf(gnuplot, commands[i], affich.data_graph_path);
        }
        else if (i == 7)
        {
            fprintf(gnuplot, commands[i], affich.data_file_path);
        }
        else
        {
            fprintf(gnuplot, "%s", commands[i]);
        }
        fflush(gnuplot);
    }

    pclose(gnuplot);
}

void affichage_potentiel(potentiel pot){

    FILE *f = fopen(PATH_POT_CSV, "w");
    fprintf(f, "x;V\n");

    for (double x = 0; x < L; x+=PAS_AFF)
    {
        fprintf(f, "%.5e;%.5e\n", x, pot.v(x,pot));
    }
    fclose(f);


    int nbr_commands = 8;

    /* liste des commandes à faire  */ 
    char *commands[] = {"reset\n",
                        "set terminal png\n",
                        "set output '%s'\n",
                        "set title 'Graph de la fonction de potentiel'\n",
                        "set xlabel 'x'\n",
                        "set ylabel 'phi(x)'\n",
                        "set datafile separator ';'\n",
                        "plot '%s' using 1:2 with lines \n"};

    /* ouvre le terminal de gnuplot */ 
    FILE *gnuplot = popen("gnuplot", "w");

    /* Lance les commandes  */
    for (int i = 0; i < nbr_commands; i++)
    {
        if (i == 2)
        {
            fprintf(gnuplot, commands[i], PATH_POT_PNG);
        }
        else if (i == 7)
        {
            fprintf(gnuplot, commands[i], PATH_POT_CSV);
        }
        else
        {
            fprintf(gnuplot, "%s", commands[i]);
        }
        fflush(gnuplot);
    }
    pclose(gnuplot);
}
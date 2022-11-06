#include <stdio.h>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>
#include <gsl/gsl_const_mksa.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multiroots.h>


#include <math.h>

#include "gnuplot_affichage.h"
#include "potentiel.h"
#include "rkf45.h"
#include "powell.h"
#include "../config_affichage.h"
#include "../config.h"
#include "../variables_physiques.h"


void run(potentiel pot){
  affichage_struct affich = {1,PATH_RES_CSV,PATH_RES_PNG};

  powell_result solution = resolve_powell(pot);     // résout l'équation avec le potentiel donné
  
  printf("Résultats obtenus avec les conditions initiales données dans le fichier config.h :\n");
  printf("E = %.5e\nz = %.5e\n",solution.E,solution.z);
  
  resolve_rkf45(pot,solution.E,solution.z,affich);   // pour l'affichage et l'écriture dans le fichier
  affichage_fonction(affich);
}

void run_test(potentiel pot){
  affichage_struct affich = {1,PATH_ATT_CSV,PATH_ATT_PNG};

  int n = N;
  double k = (n*M_PI)/L;
  double ene = (H_BAR*H_BAR*k*k)/(2*M);
  double z = (k*sqrt(2))/(sqrt(L));
  
  
  printf("Résultats attendus : \nE = %.5e\nz = %.5e\n",ene,z);
  resolve_rkf45(pot,ene,z,affich);               // pour l'écriture dans le fichier
  affichage_fonction(affich);
}

void menu();


int main(void)
{
  menu();
  int fin_choix_pot = 0;
  int choix_pot;
  scanf("%d", &choix_pot);

  potentiel pot;
  double Vo;
  double Vi;
  double a;
  double b;

  char test = 'n';
  while(!fin_choix_pot){

    switch (choix_pot)
    {

      // potentiel constant
    case 1:

      printf("\e[1;1H\e[2J");
      printf("Les configurations d'affichage et d'écriture des résultats doivent être modifiées dans le fichier config_affichage.h\n");
      printf("Les configurations de résolution doivent être modifiées dans le fichier config.h\n\n");

      printf("Vous avez choisi le potentiel constant entre 0 et %.5e . \n",L);

      printf("Valeur du potentiel: ");
      scanf("%lf", &Vo);
      printf("\n");
      
      pot = construct("constant",0,0,Vo,0);

      if(Vo==0){
        printf("Le potentiel nul est une valeur de test, voulez-vous lancer la comparaison ? (y/n)");
        scanf(" %c", &test);
        printf("\n");
      }
      
      if(AFF_POT) affichage_potentiel(pot);

      fin_choix_pot = 1;
      break;
    
    
    //  marche de potentiel
    case 2:
      
      printf("\e[1;1H\e[2J");
      printf("Les configurations d'affichage et d'écriture des résultats doivent être modifieées dans le fichier config_affichage.h\n");
      printf("Les configurations de résolution doivent être modifiées dans le fichier config.h\n\n");

      printf("\n\nVous avez choisi la marche de potentiel. \n");

      printf("Abscisse de la discontinuité (valeurs entre 0 et %.5e) : \n",L);
      scanf("%lf", &a);
      printf("\n");
      
      printf("Valeur du potentiel apres la discontinuité: ");
      scanf("%lf", &Vo);
      printf("\n");


      pot = construct("marche",a,0,Vo,0);

      if(AFF_POT) affichage_potentiel(pot);

      fin_choix_pot = 1;
      break;


    //  barrière de potentiel
    case 3:

      printf("\e[1;1H\e[2J");
      printf("Les configurations d'affichage et d'écriture des résultats doivent être modifiées dans le fichier config_affichage.h\n");
      printf("Les configurations de résolution doivent être modifiées dans le fichier config.h\n\n");

      printf("\n\nVous avez choisi la barrière de potentiel. \n");

      printf("Abscisse de la 1ère discontinuité (valeur entre 0 et %.5e) : \n",L);
      scanf("%lf", &a);
      printf("\n");

      printf("Abscisse de la 2ème discontinuité (valeur entre  %.5e et %.5e) : \n",a,L);
      scanf("%lf", &b);
      printf("\n");
      
      printf("Valeur du potentiel entre les discontinuités : ");
      scanf("%lf", &Vo);
      printf("\n");

      pot = construct("barriere",a,b,Vo,0);

      if(AFF_POT) affichage_potentiel(pot);
      
      fin_choix_pot = 1;
      break;


    //  potentiel parabolique
    case 4:

      printf("\e[1;1H\e[2J");
      printf("Les configurations d'affichage et d'écriture des résultats doivent être modifiées dans le fichier config_affichage.h\n");
      printf("Les configurations de résolution doivent être modifiées dans le fichier config.h\n\n");

      printf("\n\nVous avez choisi le potentiel parabolique. \n");

      printf("Abscisse de la 1ère racine de la parabole (valeur entre 0 et %.5e) : \n",L);
      scanf("%lf", &a);
      printf("\n");

      printf("Abscisse de la 2ème racine de la parabole (valeur entre  %.5e et %.5e) : \n",a,L);
      scanf("%lf", &b);
      printf("\n");
      
      printf("Paramètre V1 de la fonction potentiel entre les discontinuités, \n");
      printf("tel que V(x)= V1*(x-%.2e)(x-%.2e)\n",a,b);
      printf("\n");
      printf("V1 = ");
      scanf("%lf", &Vi);
      printf("\n");

      pot = construct("chapeau",a,b,0,Vi);

      if(AFF_POT) affichage_potentiel(pot);

      fin_choix_pot = 1;
      break;

    case 5:
      menu();
      
      break;

    default:
      return 0;
      break;
    }
  }

  if(test == 'y'){
    run_test(pot);
    printf("\n");
  }

  run(pot);
  
  return 0;
}


void menu(){
  printf("\e[1;1H\e[2J");

  printf("Les configurations d'affichage et d'écriture des résultats doivent être modifiées dans le fichier config_affichage.h\n");
  printf("Les configurations de résolution doivent être modifiées dans le fichier config.h\n\n");

  printf("Veuillez choisir une configuration de potentiel :\n");
  printf("  1: constant,\n");
  printf("  2: marche,\n");
  printf("  3: barriere,\n");
  printf("  4: parabolique (non fonctionel),\n");
  printf("  5: retour,\n");
  printf("  autre : quitter.\n");
  printf("Votre choix : ");
  printf("\n");
}

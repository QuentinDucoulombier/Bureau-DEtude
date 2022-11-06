#ifndef _POTENTIEL_H
#define  _POTENTIEL_H


/* Structure de potentiel composé :
 *  d'un pointeur vers la fonction du potentiel;
 *  du nombre de discontinuités
 *  d'un tableau des abscisses des discontinuités
 *  de Vo la valeur de potentiel constant
 *  de Vi la valeur de potentiel suplémentaire pour la configuration parabolique
*/
typedef struct potentiel{
  double (*v)(double t , struct potentiel pot);   // pointeur vers la fonction qui retourne le potentiel V(t) 
  int nb_dis;   
  double *discontinuites;
  double Vo;
  double Vi;
} potentiel;


/* Fonction de construction de la structure potentiel qui prend :
 *  le patern (la configuration) du potentiel (constant, marche, barrière, parabolique)
 *  les bornes (a,b avec a<b) de discontinuité du potentiel (0 s'il n'y en a pas)
 *  
 *  Vo la valeur de potentiel constant
 *  Vi la valeur de potentiel suplémentaire pour la configuration parabolique
 * Retourne le potentiel
*/
potentiel construct(char* patern, double a, double b, double Vo, double Vi);

#endif

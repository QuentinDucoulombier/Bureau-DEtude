#include "rkf45.h"
#include "../config.h"
#include "../variables_physiques.h"



int func(double t, const double y[], double f[], void *params)
{
  (void)(t); // pour éviter les warning 

  double E = ((rkf45_params *)params)->E;                                      // E : énergie
  double (*V)(double t, struct potentiel pot) = ((rkf45_params *)params)->V.v; // V : fonction potentiel

  // Définition de f de l'équation epsilon z
  f[0] = y[1];
  f[1] = -(2 * M * (E - V(t, ((rkf45_params *)params)->V)) * y[0]) / (H_BAR * H_BAR); 
  f[2] = y[0] * y[0];

  return GSL_SUCCESS;
}

rkf45_result resolve_rkf45(potentiel V, double E, double z, affichage_struct affich)
{
  int affichage = affich.affichage;  // booléen qui définie si les résultats sont écrits dans le fichier
  FILE *f;

  if (affichage)
  {
    char *file_path = affich.data_file_path;
    f = fopen(file_path, "w");
    fprintf(f, "x;y;y';N\n");
  }

  rkf45_params params = {E, V}; // Initialisation des paramètres de la fonction

  const gsl_odeiv2_step_type *T = gsl_odeiv2_step_rkf45; // Méthode RKF45

  // Initialisation des variables de résolution de l'équation 
  gsl_odeiv2_step *s = gsl_odeiv2_step_alloc(T, 3);
  gsl_odeiv2_control *c = gsl_odeiv2_control_y_new(ACC_RKF45, 0.0);
  gsl_odeiv2_evolve *e = gsl_odeiv2_evolve_alloc(3);

  /* système à résoudre avec la fonction, le jacobien (ici aucun),
   * la dimension de la solution, les paramètres de la fonction
   */
  gsl_odeiv2_system sys = {func, NULL, 3, &params};

  int region = 0;   // Région dans laquelle on se trouve
  double t = 0.0;   // Départ en x=0
  double t1 ;       // Arrivée
  double h = ACC_RKF45;  // Le pas initial

  double y[3] = {0.0, z, 0.0}; // Valeur initiale en x=0 ( 0, z, 0)

  if (affichage)
    fprintf(f, "%.5e;%.5e;%.5e;%.5e\n", t, y[0], y[1], y[2]);

  do
  { 
    /* Définition de t1, l'abscisse de la prochaine discontinuité
     * L si il y en a pas ou qu'il y en a plus
    */
    if(V.nb_dis !=0 && region<V.nb_dis ){
      t1 = V.discontinuites[region] ;
    }else{
      t1 = L;
    }
    while (t < t1)
    {
      int status = gsl_odeiv2_evolve_apply(e, c, s, &sys, &t, t1, &h, y);

      if (status != GSL_SUCCESS)
        break;
      if (affichage)
        fprintf(f, "%.5e;%.5e;%.5e;%.5e\n", t, y[0], y[1], y[2]);
    }
    region++;
  } while (t < L);

  // Libération des variables
  gsl_odeiv2_evolve_free(e);
  gsl_odeiv2_control_free(c);
  gsl_odeiv2_step_free(s);
  if (affichage)
    fclose(f);

  rkf45_result res = {y[0], y[2]}; 

  return res;
}

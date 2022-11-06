#include "powell.h"
#include "../config.h"

int fonc_S(const gsl_vector * x, void *params, gsl_vector * f)
{
  potentiel pot = *(potentiel *) params;    // récupération des paramètres de la fonction

  const double E = gsl_vector_get (x, 0);  // l'énergie
  const double z = gsl_vector_get (x, 1);  // z valeur de la dérivée de phi en 0

  affichage_struct affich ={0,NULL,NULL};            // structure d'affichage, 0 pour aucun affichage
  rkf45_result res = resolve_rkf45(pot,E,z,affich);  // résout le système avec une energie et un z particulier

  double Y1 = res.y1 ;        // Valeur obtenue (phi en L)
  double Y3 = res.y3 ;        // Valeur obtenue (phi² en L)
  
  /* Calcul de S  */
  const double f0 = Y1 - 0 ;   
  const double f1 = Y3 - 1 ;

  gsl_vector_set (f, 0, f0);
  gsl_vector_set (f, 1, f1);

  return GSL_SUCCESS;
}

powell_result resolve_powell(potentiel pot)
{
  gsl_multiroot_function f = {&fonc_S, 2, &pot};   // fonction à résoudre

  double x_init[2] = {E_INIT, Z_INIT};      // valeurs de l'énergie et du z (valeur de la dérivée de phi en 0) intiaux

  /* Initialisation du vecteur x */
  gsl_vector *x = gsl_vector_alloc (2);
  gsl_vector_set (x, 0, x_init[0]);
  gsl_vector_set (x, 1, x_init[1]);

  // Initialistaion des variables du solveur
  const gsl_multiroot_fsolver_type *T = gsl_multiroot_fsolver_hybrids; // Méthode hybride de Powell
  gsl_multiroot_fsolver *s = gsl_multiroot_fsolver_alloc (T, 2);

  gsl_multiroot_fsolver_set(s, &f, x);

  int status;
  int iter = 0;
  do
    {
      iter++;
      status = gsl_multiroot_fsolver_iterate(s);

      if (status){   // Vérifie si le solveur est bloqué
        printf("Erreur : solveur bloqué\n");
        break;
      }
      status = gsl_multiroot_test_residual(s->f, ACC_POWELL);   // test de la marge d'erreur
    }
  while (status == GSL_CONTINUE && iter < 1000);

  powell_result res = {gsl_vector_get(s->x,0),gsl_vector_get(s->x,1)};  // création de la variable résultat

  // Libération des variables du solveur
  gsl_multiroot_fsolver_free (s);
  gsl_vector_free (x);

  return res;

}

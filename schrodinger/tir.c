/* Recherche d'une racine de la fonction de tir
 * @author Romain DUJOL
 */

#include <math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_odeiv2.h>
#include <gsl/gsl_multiroots.h>
#include "consts.h"
#include "equadiff.h"
#include "tir.h"

/** Fonction permettant de definir la fonction de tir
  * @param[in]  ez     vecteur de longueur 2 contenant les valeurs de E et z
  * @param[in]  params parametres supplementaires : largeur du puits et parametres decrivant l'equation differentielle
  * @param[out] s      vecteur de longueur 2 contenant les valeurs de la fonction de tir
  * @return code de retour GSL de la resolution de l'equation differentielle (GSL_SUCCESS si tout se passe bien)
  */
int f_tir(const gsl_vector *ez, void *params, gsl_vector *s) {
    /* Recuperation des parametres : largeur du puits et parametres du potentiel  */
    ftir_params_type *pars = (ftir_params_type *) params ;
    double l = pars -> l ;
    potentiel_params_type *potentiel = pars -> equadiff -> potentiel ;
    /* Mise a la bonne valeur du niveau d'energie (donnee par la premiere composante du vecteur 'ez') */
    pars -> equadiff -> e = gsl_vector_get(ez, 0) ;

    /* Initialisation du solveur d'equation differentielle */
    gsl_odeiv2_system equadiff = {f, NULL, 3, pars -> equadiff} ;
    gsl_odeiv2_driver *d = gsl_odeiv2_driver_alloc_y_new(&equadiff, gsl_odeiv2_step_rkf45, 1.0e-6 * l, RKF45_ABSTOL, RKF45_RELTOL) ;

    /* Construction des conditions initiales (dont la seconde est donnee par la deuxieme composante du vecteur 'ez') */
    double x = 0.0 ;
    double y[3] = {0.0, gsl_vector_get(ez, 1), 0.0} ;

    /* Resolution de l'equation differentielle successivement et par morceaux */
    int status ;
    switch (potentiel -> type) {
        case MARCHE : {
            status = gsl_odeiv2_driver_apply(d, &x, potentiel -> debut, y) ;
            /* Si le premier morceau a echoue, inutile de faire le second */
            if (status == GSL_SUCCESS)
              status = gsl_odeiv2_driver_apply(d, &x, l, y) ;
            break ;
          }
        case PORTE :
        case PARABOLE : {
            status = gsl_odeiv2_driver_apply(d, &x, potentiel -> debut, y) ;
            /* Si le premier morceau a echoue, inutile de faire les suivantes */
            if (status == GSL_SUCCESS) {
                status = gsl_odeiv2_driver_apply(d, &x, potentiel -> fin, y) ;
                /* Si le second morceau a echoue, inutile de faire le dernier */
                if (status == GSL_SUCCESS)
                  status = gsl_odeiv2_driver_apply(d, &x, l, y) ;
              }
            break ;
          }
        default : status = gsl_odeiv2_driver_apply(d, &x, l, y) ;
      }

    /* Toujours liberer les ressources ! */
    gsl_odeiv2_driver_free(d) ;

    /* Ecriture du resultat de la fonction de tir */
    gsl_vector_set(s, 0, y[0]      ) ;
    gsl_vector_set(s, 1, y[2] - 1.0) ;

    return status ;
  }

/** Cherche les valeurs de E et z associees a une solution de l'equation de SCHRODINGER avec condition de normalisation
  * @param[in]     max_iter nombre maximum d'iterations permises
  * @param[in,out] ez       en entree : approximations initiales de E et z
  *                         en sortie : solution pour E et z
  * @param[in]     params   parametres decrivant la fonction de tir
  * @return code de retour GSL de la recherche de racine (GSL_SUCCESS si tout se passe bien)
  */
int cherche_ez(int max_iter, gsl_vector *ez, ftir_params_type *params) {
    /* Initialisation de la methode de recherche de racine */
    gsl_multiroot_function fonction_tir = {&f_tir, 2, params} ;
    gsl_multiroot_fsolver *s = gsl_multiroot_fsolver_alloc(gsl_multiroot_fsolver_hybrid, 2) ;
    gsl_multiroot_fsolver_set(s, &fonction_tir, ez) ;

    /* Application iterative de la methode */
    int status = GSL_CONTINUE ;
    printf("%5s S(%9s, %9s) = (%9s, %9s)\n", "iter", "E", "z", "phi(L)", "N(L) - 1") ;
    int iter = 0 ;
    for (iter = 1 ; (status == GSL_CONTINUE) && iter <= max_iter ; iter++) {
        status = gsl_multiroot_fsolver_iterate(s) ;
        if (status == GSL_SUCCESS) {
            /* Si le point suivant a pu etre calcule, on verifie si ce dernier est proche d'une solution. */
            status = gsl_multiroot_test_residual(s -> f, HYBRD_TOL) ; // Rappel : renvoie GSL_CONTINUE si la methode n'a pas encore converge
            gsl_vector_memcpy(ez, s -> x) ;
            printf("%5d S(%+.6f, %+.6f) = (%+.2e, %+.2e)\n", iter, gsl_vector_get(ez, 0), gsl_vector_get(ez, 1), gsl_vector_get(s -> f, 0), gsl_vector_get(s -> f, 1)) ;
          }
      }

    /* Toujours liberer les ressources ! */
    gsl_multiroot_fsolver_free(s) ;

    return status ;
  }

/** Calcule les valeurs theoriques de E et z associees a une solution pour le potentiel nul
  * @param[in] n numero du niveau d'energie
  * @param[in] l largeur du puits
  * @param[in] m masse de la particule
  * @return vecteur de longueur 2 contenant les valeurs theoriques de E et z
  */
gsl_vector *theorique_potentielnul(int n, double l, double m) {
    double e = n * n * HBARRE2 * M_PI * M_PI / (2.0 * m * l * l) ;
    double z = sqrt(2.0 / l) * n * M_PI / l ;

    gsl_vector *ez = gsl_vector_calloc(2) ;
    gsl_vector_set(ez, 0, e) ;
    gsl_vector_set(ez, 1, z) ;

    return ez ;
  }

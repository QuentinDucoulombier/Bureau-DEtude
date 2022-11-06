/* Recherche d'une racine de la fonction de tir
 * @author Romain DUJOL
 */

#include <gsl/gsl_vector.h>
#include "equadiff.h"

#ifndef _TIR_H
#define _TIR_H

/** Parametres decrivant la fonction de tir
  * @param l        largeur du puits
  * @param equadiff parametres decrivant l'equation differentielle
  */
typedef struct {
    double                l        ;
    equadiff_params_type *equadiff ;
  } ftir_params_type ;

/** Fonction permettant de definir la fonction de tir
  * @param[in]  ez     vecteur de longueur 2 contenant les valeurs de E et z
  * @param[in]  params parametres supplementaires : largeur du puits et parametres decrivant l'equation differentielle
  * @param[out] s      vecteur de longueur 2 contenant les valeurs de la fonction de tir
  * @return code de retour GSL de la resolution de l'equation differentielle (GSL_SUCCESS si tout se passe bien)
  */
int f_tir(const gsl_vector *ez, void *params, gsl_vector *s) ;

/** Cherche les valeurs de E et z associees a une solution de l'equation de SCHRODINGER avec condition de normalisation
  * @param[in]     max_iter nombre maximum d'iterations permises
  * @param[in,out] ez       en entree : approximations initiales de E et z
  *                         en sortie : solution pour E et z
  * @param[in]     params   parametres decrivant la fonction de tir
  * @return code de retour GSL de la recherche de racine (GSL_SUCCESS si tout se passe bien)
  */
int cherche_ez(int max_iter, gsl_vector *ez, ftir_params_type *params) ;

/** Calcule les valeurs theoriques de E et z associees a une solution pour le potentiel nul
  * @param[in] n numero du niveau d'energie
  * @param[in] l largeur du puits
  * @param[in] m masse de la particule
  * @return vecteur de longueur 2 contenant les valeurs theoriques de E et z
  */
gsl_vector *theorique_potentielnul(int n, double l, double m) ;

#endif // _TIR_H

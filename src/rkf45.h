#ifndef _RKF_H
#define _RKF_H

#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>
#include <gsl/gsl_const_mksa.h>

#include "potentiel.h"
#include "gnuplot_affichage.h"

/* Structure de résultat
 *  contient la valeur y1(L) et y3(L)
 */
typedef struct{
    double y1;
    double y3;
} rkf45_result;

/* Structure des paramètres
 *  contient l'énergie et le potentiel
 */
typedef struct{
    double E;
    potentiel V;
} rkf45_params;

/* Résout l'équation epsilon z avec un potentiel, une énergie et un z particulier
 *  qui écrit les résultats selon la variable d'affichage (cf gnuplot_affichage.h)
 */
rkf45_result resolve_rkf45(potentiel V ,double E ,double z ,affichage_struct affich);

#endif

#ifndef _POWELL_H
#define _POWELL_H

#include <gsl/gsl_errno.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multiroots.h>

#include "potentiel.h"
#include "gnuplot_affichage.h"
#include "rkf45.h"

// Structure de la solution (E,z)
typedef struct{
    double E;
    double z;
} powell_result;

// Retourne la solution (E,z) de l'équation S(E,z)=0
powell_result resolve_powell(potentiel pot );




#endif

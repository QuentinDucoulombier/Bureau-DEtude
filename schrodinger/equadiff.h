/** Resolution numerique de l'equation differentielle avec conditions initiales
  * @author Romain DUJOL
  */

#include <gsl/gsl_vector.h>

#ifndef _EQUADIFF_H
#define _EQUADIFF_H

/** Type de potentiel */
typedef enum {NUL, MARCHE, PORTE, PARABOLE} potentiel_type ;

/** Parametres decrivant la fonction potentiel V
  * @param type   type de potentiel (cf. declaration du type 'potentiel_type')
  * @param debut  si type == MARCHE   : position de debut de la marche
  *               si type == PORTE    : position de debut de la porte
  *               si type == PARABOLE : position de debut de la parabole
  * @param fin    si type == PORTE    : position de fin de la marche
  *               si type == PARABOLE : position de fin de la parabole
  * @param valeur si type == MARCHE   : hauteur de la marche
  *               si type == PORTE    : hauteur de la porte
  *               si type == PARABOLE : hauteur maximale de la parabole atteinte en (debut + fin) / 2
  */
typedef struct {
    potentiel_type type   ;
    double         debut  ;
    double         fin    ;
    double         valeur ;
  } potentiel_params_type ;

/** Parametres decrivant l'equation differentielle a resoudre
  * @param m         masse de la particule
  * @param e         niveau d'energie
  / @param potentiel parametres decrivant le potentiel
  */
typedef struct {
    double                 m         ;
    double                 e         ;
    potentiel_params_type *potentiel ;
  } equadiff_params_type ;

/** Definition de l'equation differentielle a resoudre
  * @param[in]  x      position dans le puits
  * @param[in]  y      vecteur de longueur 3 contenant l'etat (phi, phi1 et N) a l'instant x
  * @param[out] yp     vecteur de longueur 3 contenant la derivee de l'etat a l'instant x
  * @param[in]  params parametres decrivant l'equation differentielle
  * @return code de retour GSL (GSL_SUCCESS si tout se passe bien)
  */
int f(double x, const double y[], double yp[], void *params) ;

/** Stocke la solution dans un format adapte pour gnuplot
  * @param[in] nomfichier nom du fichier ou stocker les donnees
  * @param[in] ez         vecteur de longueur 2 contenant les valeurs de E et z
  * @param[in] l          largeur du puits
  * @param[in] nx         nombre de points à tracer (subdivision reguliere)
  * @param[in] params     parametres decrivant l'equation differentielle
  * @return code de retour GSL de la resolution de l'equation differentielle (GSL_SUCCESS si tout se passe bien)
  */
int ecrire_gnuplot(const char *nomfichier, gsl_vector *ez, double l, int nx, equadiff_params_type *params) ;

#endif // _EQUADIFF_H

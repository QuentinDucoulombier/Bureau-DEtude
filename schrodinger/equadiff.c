/** Resolution numerique de l'equation differentielle avec conditions initiales
  * @author Romain DUJOL
  */

#include <gsl/gsl_vector.h>
#include <gsl/gsl_odeiv2.h>
#include "consts.h"
#include "equadiff.h"

/** Calcule le potentiel (par default, utilise le potentiel nul)
  * @param[in] x      position du calcul du potentiel
  * @param[in] params parametres decrivant le potentiel
  */
double v(double x, potentiel_params_type *params) {
    double v_x = 0.0 ;

    if (params != NULL)
      switch (params -> type) {
          case MARCHE : {
              if (x >= params -> debut)
                v_x = params -> valeur ;
              break ;
            }
          case PORTE  : {
              if (x >= params -> debut && x <= params -> fin)
                v_x = params -> valeur ;
              break ;
            }
          case PARABOLE : {
              if (x >= params -> debut && x <= params -> fin) {
                  double d = params -> fin - params -> debut ;
                  double m = 0.5 * (params -> debut + params -> fin) ;
                  double rho = (x - m) / (0.5 * d) ;
                  v_x = params -> valeur * (1.0 - rho * rho) ;
                }
              break ;
            }
          default : v_x = 0.0 ;
        }

    return v_x ;
  }

/** Definition de l'equation differentielle a resoudre
  * @param[in]  x      position dans le puits
  * @param[in]  y      vecteur de longueur 3 contenant l'etat (phi, phi1 et N) a l'instant x
  * @param[out] yp     vecteur de longueur 3 contenant la derivee de l'etat a l'instant x
  * @param[in]  params parametres supplementaires : masse, niveau d'energie, potentiel
  * @return code de retour GSL (GSL_SUCCESS si tout se passe bien)
  */
int f(double x, const double y[], double yp[], void *params) {
    /* Recuperation des parametres : masse, niveau d'energie et potentiel */
    equadiff_params_type *pars = (equadiff_params_type *) params ;
    double m = pars -> m ;
    double e = pars -> e ;
    double v_x = v(x, pars -> potentiel) ;

    /* Equation differentielle d'ordre 2 pour phi */
    yp[0] = y[1] ;
    yp[1] = -2.0 * m / HBARRE2 * (e - v_x) * y[0] ;
    /* Condition de normalisation */
    yp[2] = y[0] * y[0] ;

    return GSL_SUCCESS ;
  }

/** Stocke la solution dans un format adapte pour gnuplot
  * @param[in] nomfichier nom du fichier ou stocker les donnees (toutes les donnees precedentes sont ecrasees)
  * @param[in] ez         vecteur de longueur 2 contenant les valeurs de E et z
  * @param[in] l          largeur du puits
  * @param[in] nx         nombre de points à tracer (subdivision reguliere)
  * @param[in] params     parametres decrivant l'equation differentielle
  * @return code de retour GSL de la resolution de l'equation differentielle (GSL_SUCCESS si tout se passe bien)
  */
int ecrire_gnuplot(const char *nomfichier, gsl_vector *ez, double l, int nx, equadiff_params_type *params) {
    int non_abs = 0; //0 PAS de valeur abs
                    // 1 valeur abs


    /* Ouverture en ecriture du fichier */
    FILE *fd = NULL ;
    if (nomfichier == NULL)
      fd = stdout ; /* Sortie standard : fprintf(stdout, ...) est equivalent à printf(...) */
    else {
        fd = fopen(nomfichier, "w") ;
        if (fd == NULL) {
            printf("Impossible d'ouvrir le fichier %s : affichage des résultats sur la sortie standard.\n", nomfichier) ;
            fd = stdout ;
          }
      }

    /* Ecriture de l'en-tete (non interprete par gnuplot car en commentaire avec '#') */
    fprintf(fd, "# Equation de SCHRODINGER\n") ;
    fprintf(fd, "# x phi(x) phi'(x) N(x)\n") ;

    /* Initialisation du solveur d'equation differentielle */
    params -> e = gsl_vector_get(ez, 0) ;
    gsl_odeiv2_system equadiff = {f, NULL, 3, params} ;
    gsl_odeiv2_driver *d = gsl_odeiv2_driver_alloc_y_new(&equadiff, gsl_odeiv2_step_rkf45, 1.0e-6 * l, RKF45_ABSTOL, RKF45_RELTOL) ;

    /* Construction des conditions initiales et ecriture des valeurs dans le fichier de donnees */
    double x = 0.0 ;
    double y[3] = {0.0, gsl_vector_get(ez, 1), 0.0} ;
    if(!non_abs){
      fprintf(fd, "%f %+.18e %+.18e %+.18e\n", x, y[0], y[1], y[2]) ;
    }else{
      fprintf(fd, "%f %+.18e %+.18e %+.18e\n", x, fabs(y[0])*fabs(y[0]), y[1], y[2]) ;
    }
      

    double dx = l / (nx - 1) ; /* Valeur du pas entre deux points consecutifs */

    int status = GSL_SUCCESS ;
    for (int i = 1 ; (status == GSL_SUCCESS) && (i < nx) ; i++) {
        /* On resout l'equation differentielle pas à pas... */
        status = gsl_odeiv2_driver_apply(d, &x, i * dx, y) ;
        /* ... et on ecrit les valeurs dans le fichier de donnees pour chaque position. */
        if (status == GSL_SUCCESS){
          if(!non_abs){
            fprintf(fd, "%f %+.18e %+.18e %+.18e\n", x, y[0], y[1], y[2]) ;
          }else{
            fprintf(fd, "%f %+.18e %+.18e %+.18e\n", x, fabs(y[0])*fabs(y[0]), y[1], y[2]) ;
          }
        }else
          fprintf(fd, "# Calcul interrompu prematurement\n") ;
      }

    /* Toujours liberer les ressources ! */
    gsl_odeiv2_driver_free(d) ;
    fclose(fd) ;

    return status ;
  }

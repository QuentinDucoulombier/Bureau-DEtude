/* Fichier de configuration pour les paramètres modifiables liés à l'équation à résoudre
 *
 *
 */

#define L 1.0     // x appartient à [0,L], en nm (nanomètre)


/* Valeurs d'initialisation de la méthode Powell
 */
#define E_INIT 1  // l'énergie
#define Z_INIT 1  // z, valeur de la dérivée de phi en 0


/* Variables pour la résolution test de l'équation à potentiel nul
 *  pour tester le programme et vérifier son fonctionnement
 */
#define N 1       // niveau de quantification de l'énergie 



/* Variables de fonctionnement pour la méthode Powell et RKF45
 *  
 */
#define ACC_RKF45  1e-9    // Précision pour la méthode RKF45

#define ACC_POWELL 1e-7    // Précision pour la méthode de Powell

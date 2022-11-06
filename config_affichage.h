/* Fichier de configuration pour les paramètres modifiables liés à l'affichage et aux résultats
 *
 *
 */

/* Résultats après la résolution de l'équation
 */
#define PATH_RES_CSV "./resultats/resultat.csv"   // chemin vers le fichier résultat.csv
#define PATH_RES_PNG "./resultats/resultat.png"   // chemin vers le graph résultat.png  (extension png)

/* Résultats atendus pour la résolution de l'équation à potentiel nul
 * pour tester le programme et vérifier son fonctionnement
 */
#define PATH_ATT_CSV "./resultats/attendu.csv"   // chemin vers le fichier attendu.csv
#define PATH_ATT_PNG "./resultats/attendu.png"   // chemin vers le graph attendu.png  (extension png)



/* Variables pour l'affichage et la création de fichiers pour la fonction de potentiel
 *
 */
#define AFF_POT 1         // affichage du potentiel par défaut 1 (= vrai)

#define PAS_AFF 1e-3         // le pas pour l'affichage du potentiel
#define PATH_POT_CSV "./resultats/potentiel.csv"   // chemin vers le fichier potentiel.csv
#define PATH_POT_PNG "./resultats/potentiel.png"   // chemin vers le graph potentiel.png  (extension png)
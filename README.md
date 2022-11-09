# BE code

---

Auteurs : Quentin Ducoulombier, Furger Achille, Lucas Thu Ping One, Noé Faucher  
Date: 28 mai 2022 
Email: ducoulombi@cy-tech.fr, furgerachi@cy-tech.fr, thupingone@cy-tech.fr, fauchernoe@cy-tech.fr

---

## Présentation

L'objectif est de résoudre l'équation de Schrödinger dans des configurations où ce n'est pas faisable à la main.
Pour cela nous utilisons deux methodes :
- La methode de rkf45 
- La methode de powell

Pour plus d'explication cliquez [**ici**](./documentation/explication/id%C3%A9es_g%C3%A9n%C3%A9rales_pour_comprendre.md)


## Installation

Pour compiler le programme :
  ``` 
  > make 
  ```

Pour lancer le programme :
  ``` 
  > ./exe
  ```
Puis suivre les indications dans le menu.

<br>

Pour modifier certains paramètres du programme, il faut les modifier directement dans les fichiers :

* config.h : pour les paramètres liés à l'équation à résoudre
* config_affichage.h : pour les paramètres liés à l'affichage des résultats

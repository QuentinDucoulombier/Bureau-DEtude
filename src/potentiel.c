#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "potentiel.h"

/* fonction potentiel constant */
double constant(double t, potentiel pot){
  (void) (t);
  return pot.Vo;
}

/* fonction marche de potentiel */
double marche(double t, potentiel pot){
  double v;
  if(t<= pot.discontinuites[0]){
    v = 0;
  }else{
    v = pot.Vo;
  }
  return v;
}

/* fonction barrière de potentiel */
double barriere(double t, potentiel pot){
  double v;
  if(t< pot.discontinuites[0] || t> pot.discontinuites[1]){
    v= 0;      // hors des discontinuités
  }else{
    v= pot.Vo;
  }
  return v;
}

/* fonction parabole de potentiel */
double chapeau(double t, potentiel pot){
  double v;

  if(t< pot.discontinuites[0] || t> pot.discontinuites[1]){
    v= 0;    // hors des discontinuités
  }else{
    v= - pot.Vi* (t-pot.discontinuites[0])*(t-pot.discontinuites[1]);
  }
  return v;
}


potentiel construct(char* patern, double a, double b, double Vo, double Vi){
  potentiel res;
  if(strcmp(patern, "constant")==0){
    res.v = &constant;
    res.nb_dis =0;
    res.discontinuites = NULL;
    res.Vo =Vo;
    res.Vi =Vi;

  }else if (strcmp(patern, "marche")==0){
    res.v = marche;
    res.nb_dis =1;
    res.discontinuites = (double*) malloc(1*sizeof(double));
    res.discontinuites[0] = a;
    res.Vo =Vo;
    res.Vi =Vi;

  }else if (strcmp(patern, "barriere")==0){
    res.v = barriere;
    res.nb_dis =2;
    res.discontinuites = (double*) malloc(2*sizeof(double));
    res.discontinuites[0] = a;
    res.discontinuites[1] = b;
    res.Vo =Vo;
    res.Vi =Vi;

  }else if (strcmp(patern, "chapeau")==0){
    res.v = chapeau;
    res.nb_dis =2;
    res.discontinuites = (double*) malloc(2*sizeof(double));
    res.discontinuites[0] = a;
    res.discontinuites[1] = b;
    res.Vo =Vo;
    res.Vi =Vi;

  }else{
    printf("Erreur d'allocation du potentiel : patern inconnu\n");
  }

  return res;
}

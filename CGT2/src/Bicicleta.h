#ifndef __BICICLETA_H__
#define __BICICLETA_H__

#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "gl_canvas2d.h"
#include "Matrizes.h"


class figura{
public:
  std::vector<ponto> pontos;
  virtual void desenha(float referenciaX, float referenciaY);
//  ~figura(){
//    for(auto a = pontos.begin(); a < pontos.end(); a++){
//      delete a;
//    }
//  }
};

class roda:public figura{

public:
  float raioExterno;
  float raioInterno;
  int qntAros;
  float anguloEntreAros;

  roda(float raioExterno, float raioInterno, int qntAros);

  void desenha(float referenciaX, float referenciaY);

};

roda::roda(float raioExterno, float raioInterno, int qntAros){
  this->raioExterno = raioExterno;
  this->raioInterno = raioInterno;
  this->qntAros = qntAros;
  anguloEntreAros = PI_2/((float)2*qntAros);
  std::cout << anguloEntreAros << std::endl;
  ponto* p = nullptr;
  for (int i = 0; i < qntAros*2; i++){
    p = new ponto(cos(i*anguloEntreAros),sin(i*anguloEntreAros));
    pontos.push_back(*p);
  }
}


void roda::desenha(float referenciaX, float referenciaY){
  for(int i = raioInterno; i<raioExterno; i++){
    circle(referenciaX, referenciaY, i, 50);
  }
  matriz m;
  m.MatrizEscala(raioInterno,raioInterno);
  m.MatrizTranslacao(referenciaX,referenciaY);
  for(int a = 0; a < pontos.size(); a++){
    pontos[a].MultiplicadoPorMatriz(m.pontos);
  }

}


#endif

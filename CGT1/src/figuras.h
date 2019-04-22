#ifndef __FIGURAS_H__
#define __FIGURAS_H__

#include "gl_canvas2d.h"

#include <vector>
#include <algorithm>

struct Posicao {
  float xIni, yIni, xTam, yTam;
};



class figura{
public:
  char* nome;
  Posicao pos;
  virtual void desenha(){};
  virtual void atualiza(){};
  float posXfim(){
    return pos.xIni+pos.xTam;}
  float posYfim(){
    return pos.yIni+pos.yTam;}
  void set_figura(char* nome, float xIni, float yIni, float xTam, float yTam){
    this->pos.xIni = xIni;
    this->pos.yIni = yIni;
    this->pos.xTam = xTam;
    this->pos.yTam = yTam;
    this->nome = nome;
  }

};


class grafico:public figura{
public:
  std::vector<double> amostras;
  double maior;
  float origemX, origemY, fimX, fimY; //area para desenho dentro do grafico, fimY apenas com o valor positivo
  grafico(char* nome, std::vector<double> amostras){
    set_figura(nome, 10, 10, 50, 50);  //apenas alguns pontos iniciais e tamanhos de teste, nao utilizados na versao final
    this->amostras = amostras;
    atualiza();
    maior = 0;
    for(auto &a : amostras){
      maior = (abs(ceil(a)) > maior) ? abs(ceil(a)) : maior;
    }
  }
  grafico(char* nome, float xIni, float yIni, float xTam, float yTam, std::vector<double> amostras){
    set_figura(nome, xIni, yIni, xTam, yTam);
    this->amostras = amostras;
    atualiza();
    maior = 0;
    for(auto &a : amostras){
      if(a>=0){
        maior = (a > maior) ? a : maior;
      }
      else{
        maior = (-a > maior) ? -a : maior;
      }
    }
    /*
    for(auto &a : amostras){
      maior = (abs(ceil(a)) > maior) ? abs(ceil(a)) : maior;
    }*/
  }

  void desenha(){
    color(1,0,0);
    text(pos.xIni+(0.05*pos.xTam), posYfim()+(0.02*pos.yTam), nome); //escreve o nome do grafico
    color(0,0,0);
    rect(pos.xIni, pos.yIni, posXfim(), posYfim());
    line(pos.xIni+0.05*pos.xTam, origemY, fimX, origemY ); //desenha o eixo X
    line(origemX, pos.yIni+0.05*pos.yTam, origemX, fimY );//desenha o eixo Y
    int i;
    for(i = 0; i < amostras.size()-1; i++){
      color(0,0,1);
      circleFill(origemX+((i+0.5)*(fimX-origemX)/amostras.size()), origemY+(amostras[i]*(fimY-origemY)/maior), std::min(fimX-origemX, fimY-origemY)* 0.025, 50);
      line(origemX+((i+0.5)*(fimX-origemX)/amostras.size()), origemY+(amostras[i]*(fimY-origemY)/maior),
           origemX+((i+1.5)*(fimX-origemX)/amostras.size()), origemY+(amostras[i+1]*(fimY-origemY)/maior));//desenha linha entre pontos

    }
    circleFill(origemX+((i+0.5)*(fimX-origemX)/amostras.size()), origemY+(amostras[i]*(fimY-origemY)/maior), std::min(fimX-origemX, fimY-origemY)* 0.025, 50);
    color(0,0,0);
    line(origemX-pos.xTam*0.02, fimY, origemX+pos.xTam*0.02, fimY);
    color(1,0,1);
    char str[4];
    sprintf(str, "%.0f",  maior);
    text(origemX-pos.xTam*0.075, fimY - 5, str); //-5 adicionado por casua do tamanho da fonte
  }
  void atualiza(){
    origemX = pos.xIni+0.1*pos.xTam;
    origemY = pos.yIni+0.5*pos.yTam;
    fimX = posXfim() - 0.05*pos.xTam;
    fimY = posYfim() - 0.05*pos.yTam;
  }
  void desenha(float xIni, float yIni, float xTam, float yTam){
    set_figura(nome, xIni, yIni, xTam, yTam);
    atualiza();
    desenha();
  }

};

std::vector<grafico*> listaGR;

class checkBox:public figura{
  public:
  bool marcado;
  bool mouseAcima;
  std::vector<grafico>::iterator a;
  grafico* g;
  checkBox(char* nome, float xIni, float yIni, float xTam, float yTam, grafico* g){
    set_figura(nome, xIni, yIni, xTam, yTam);
    marcado = false;
    mouseAcima = false;
    this->g = g;
  }
  checkBox(grafico* g){
    set_figura(g->nome, 0,0,50,50);
    marcado = false;
    mouseAcima = false;
    this->g = g;
  }
  void desenha(){
    if(mouseAcima){
      color(0,0,0);
    }else{
      color(1,0,0);
    }
    rect(pos.xIni, pos.yIni, posXfim(), posYfim());
    if(marcado){
      color(1,0,0);
      line(pos.xIni, pos.yIni, posXfim(), posYfim());
      line(posXfim(), pos.yIni, pos.xIni, posYfim());
    }
    color(0,0,0);
    text(pos.xIni+(0.05*pos.xTam), posYfim()+(0.05*pos.yTam), nome);
  }
  void desenha(float xIni, float yIni, float xTam, float yTam){
    set_figura(nome, xIni, yIni, xTam, yTam);
    desenha();
  }

  void marca(){
    listaGR.push_back(g);
  }
  void desmarca(){
    listaGR.erase(std::find(listaGR.begin(),listaGR.end(), g));
  }
  void atualiza(int button, int state, int x, int y){
    if(x>pos.xIni&&x<posXfim()&&y>pos.yIni&&y<posYfim()){
      mouseAcima = true;
      if(button == 0 && state == 0){
        if (marcado){
          desmarca();
        }
        else{
          marca();
        }
        marcado = !marcado;
      }
    }
    else{
      mouseAcima = false;
    }
  }
};

#endif

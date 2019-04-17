/*********************************************************************
// Canvas para desenho - Versao CPP.
//  Autor: Cesar Tadeu Pozzer
//         10/2007
//
//  Pode ser utilizada para fazer desenhos ou animacoes, como jogos simples.
//  Tem tratamento de mosue
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Instruções:
//    Para compilar em C, basta comentar o comando #define _CPP_
//	  Para alterar a animacao, digite numeros entre 1 e 7
// *********************************************************************/

/*
  Autor: Cesar Tadeu Pozzer
         04/2013

  Instruções:
	  Para alterar a animacao, digite numeros entre 1 e 7
*/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include "gl_canvas2d.h"

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#define M_PI 3.14159265358979323846
#define LARGURA_TELA 1500
#define ALTURA_TELA 900



//variaveis globais
int   opcao  = 50;
float global = 0;

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
  set_figura(char* nome, float xIni, float yIni, float xTam, float yTam){
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
  grafico(char* nome, float xIni, float yIni, float xTam, float yTam, std::vector<double> amostras){
    set_figura(nome, xIni, yIni, xTam, yTam);
    this->amostras = amostras;
    origemX = pos.xIni+0.1*pos.xTam;
    origemY = pos.yIni+0.5*pos.yTam;
    fimX = posXfim() - 0.05*pos.xTam;
    fimY = posYfim() - 0.05*pos.yTam;
    maior = 0;
    for(auto &a : amostras){
      maior = (abs(a) > maior) ? abs(a) : maior;
    }
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

    /*for(auto a = listaGR.begin(); a<listaGR.end(); a++){
      if (a->nome == this->nome){
        listaGR.erase(a);
      }
    }*/
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

std::vector<checkBox> listaCB;


//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
  auto i = 0;
  for(i = 0; i < listaCB.size(); i++){
    listaCB[i].desenha(get_largura()/20, get_altura()*0.9 - (i*(get_altura()/2)/listaCB.size()), std::min(get_largura(), get_altura())/40, std::min(get_largura(), get_altura())/40);
  }
  float desenhavelXini = get_largura()/10;
  float desenhavelYini = get_altura()/20;
  float tamXmax = get_largura()*0.95 - get_largura()/10;
  float tamYmax = get_altura()*0.95 - get_altura()/20;
  int gradeX = ceil(sqrt(listaGR.size()));
  int gradeY = gradeX;
  int qnt1l = gradeX - ((gradeX*gradeX) - listaGR.size()); //quantia de elementos na primeira linha
  qnt1l = (qnt1l > 0)? qnt1l : gradeX;
  if(qnt1l > 1 && qnt1l > listaGR.size()/2){
    qnt1l--;
    if (qnt1l==1)gradeX--;
  }
  for(i = 0; i < qnt1l; i++){
    listaGR[i]->desenha(desenhavelXini+(i*1.1*tamXmax/qnt1l), desenhavelYini+tamYmax-(tamYmax/gradeY), (tamXmax/qnt1l)*0.95, tamYmax*0.95/gradeY);
  }
  for(i = qnt1l; i < listaGR.size(); i++){
    int l;
    int c;
    c = (i-qnt1l)%(gradeX);
    l = (i-qnt1l)/gradeY;
    listaGR[i]->desenha(desenhavelXini+(c*1.1*tamXmax/gradeX), desenhavelYini+tamYmax-(tamYmax/gradeY)*(gradeY-l), (tamXmax/gradeX)*0.95, tamYmax*0.95/gradeY);

  }

}

//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
   printf("\nTecla: %d" , key);
   if( key < 200 )
   {
      opcao = key;
   }

   switch(key)
   {
      case 27:
	     exit(0);
	  break;

   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   //printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
   for(auto &a: listaCB){
    a.atualiza(button, state, x, get_altura()-y);
   }
}

float C(float u){
  return (u==0)? (1/sqrt(2)) : 1;
}

std::vector<double> aplica_dct(std::vector<double> amostras){
  std::vector<double> dct;
  float qnt = amostras.size();
  for(int u = 0; u < qnt; u++){
    float soma = 0;
    for(int i = 0; i < qnt; i++){
      soma += C(u) * amostras[i] * cos(((M_PI*u)/(2*qnt))*(2*i + 1));
    }
    dct.push_back(round(sqrt(2/qnt)*soma));
  }
  return dct;
}

std::vector<double> aplica_idct(std::vector<double> dct){
  std::vector<double>idct;
  float qnt = dct.size();
  for(int i = 0; i < qnt; i++){
    float soma = 0;
    for(int u = 0; u < qnt; u++){
      soma += C(u) * dct[u] * cos(((M_PI*u)/(2*qnt))*(2*i + 1));
    }
    idct.push_back(round(sqrt(2/qnt)*soma));
  }
  return idct;
}

std::vector<double> verifica_diff(std::vector<double>vet1, std::vector<double>vet2){
  std::vector<double> diff;
  for(int i = 0; i < vet1.size(); i++){
    diff.push_back(vet1[i] - vet2[i]);
    std::cout << diff[i] << std::endl;
  }
  return diff;
}

std::vector<int> le_arquivo(char* arquivo){
  signed char qntChar[4];
  std::ifstream dctFile;
  dctFile.open(arquivo ,std::ios::binary);
  std::vector<signed char> buffer(std::istreambuf_iterator<char>(dctFile), {});
  dctFile.close();
  int i;
  float qnt;
  unsigned int *j;
  for (i = 0; i<4; i++){
    qntChar[i] = buffer[i];
  }
  j = (unsigned int*) qntChar;
  std::cout << std::endl;
  std::cout << (*j) << std::endl << std::endl;
  qnt = *j;
  if(qnt<8||qnt>64||qnt>(buffer.size()-4)){
    std::cout<<"input fora do padrao"<<std::endl;
    exit(0);
  }
  std::vector<int> amostras;
  for(i = 4; i < qnt+4; i++){
    amostras.push_back(buffer[i]);
    std::cout << amostras[i-4] << std::endl;
  }
  return amostras;
}

int main(void)
{
  initCanvas(LARGURA_TELA,ALTURA_TELA, "Trabalho CGT1");

  std::vector<int> amostras, amostras2;
  amostras = le_arquivo("base2.dct");
  int i;
  for(i = 0; i<amostras.size(); i++){
    if(amostras[i]>=0){
      amostras[i]-=127;
    }
    else{
      amostras[i]+=128;
    }
  }

  std::vector<double> amostras_d (amostras.begin(), amostras.end());
  std::vector<double> dct, dct2;

  dct = aplica_dct(amostras_d);
  for(i = 0; i < dct.size(); i++){
    std::cout << "dct "<< i << " " << dct[i] << std::endl;
  }

  std::vector<double> idct,idct2;
  idct = aplica_idct(dct);


  for(i = 0; i < idct.size(); i++){
    std::cout << "idct "<< i << " " << idct[i] << std::endl;
  }
  amostras2 = le_arquivo("input3.dct");
  std::vector<double> amostras_d2 (amostras2.begin(), amostras2.end());
  dct2=aplica_dct(amostras_d2);
  idct2=aplica_idct(dct2);

  std::cout << std::endl;
  std::vector<double> diff,diff2;

  diff=verifica_diff(amostras_d,idct);

  diff2=verifica_diff(amostras_d2,idct2);

   checkBox *cb = NULL;
   grafico *g = NULL;
   g = new grafico("input", 2,2,496,496, amostras_d);
   cb = new checkBox("input", 50, 50, 50, 50, g);
   listaCB.push_back(*cb);
   g = new grafico("dct", 2,2,496,496, dct);
   cb = new checkBox("dct", 50, 50, 50, 50, g);
   listaCB.push_back(*cb);
   g = new grafico("idct", 2,2,496,496, idct);
   cb = new checkBox("idct", 50, 50, 50, 50, g);
   listaCB.push_back(*cb);
   g = new grafico("diff", 2,2,496,496, diff);
   cb = new checkBox("diff", 50, 50, 50, 50, g);
   listaCB.push_back(*cb);

   g = new grafico("input2", 2,2,496,496, amostras_d2);
   cb = new checkBox("input2", 50, 50, 50, 50, g);
   listaCB.push_back(*cb);
   g = new grafico("dct2", 2,2,496,496, dct2);
   cb = new checkBox("dct2", 50, 50, 50, 50, g);
   listaCB.push_back(*cb);
   g = new grafico("idct2", 2,2,496,496, idct2);
   cb = new checkBox("idct2", 50, 50, 50, 50, g);
   listaCB.push_back(*cb);
   g = new grafico("diff2", 2,2,496,496, diff2);
   cb = new checkBox("diff2", 50, 50, 50, 50, g);
   listaCB.push_back(*cb);


   runCanvas();
}

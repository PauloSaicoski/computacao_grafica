/*********************************************************************
// Canvas para desenho - Versao CPP.
//  Autor: Cesar Tadeu Pozzer
//         10/2007
//
//  Pode ser utilizada para fazer desenhos ou animacoes, como jogos simples.
//  Tem tratamento de mosue
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Instru��es:
//    Para compilar em C, basta comentar o comando #define _CPP_
//	  Para alterar a animacao, digite numeros entre 1 e 7
// *********************************************************************/

/*
  Autor: Cesar Tadeu Pozzer
         04/2013

  Instru��es:
     Para alterar a animacao, digite numeros entre 1 e 7
*/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include "gl_canvas2d.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//#include "Matrizes.h"
#include "Bicicleta.h"

#define LARGURA_TELA 1500
#define ALTURA_TELA 900

//variaveis globais
int opcao = 50;
float global = 0;
ponto *teste = nullptr;
figura *r1 = nullptr;
figura *r2 = nullptr;
quadro *q = nullptr;
pedal *p1 = nullptr;
pedal *p2 = nullptr;
figura *t = nullptr;
perna *pern1 = nullptr;
perna *pern2 = nullptr;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{

  static float angulo = 0;
  angulo = -0.01;
  color(1, 0.5, 0.5);
  p1->desenha(750, 250);
  p1->rotaEmEixo(angulo / 2);
  color(0, 0, 0);
  //r->desenha(250, 250);
  r1->rotaEmEixo(angulo);
  r2->rotaEmEixo(angulo);
  color(1, 0, 0);
  q->desenha(750, 250);
  ponto po, po2(750, 250);
  color(0.5, 0.5, 0.5);
  po = q->retornaRoda(false);
  r1->desenha(po.x, po.y);
  po = q->retornaRoda(true);
  r2->desenha(po.x, po.y);
  color(1, 0.5, 0.5);
  p2->desenha(750, 250);
  p2->rotaEmEixo(angulo / 2);
  color(0, 1, 0);
  t->desenha(750, 250);
  po = q->retornaBanco();
  po2 = p1->retornaFim(pern1->calcAngulo(po2, po));
  pern1->calcTamanhos(pern1->calcDiff(po, po2));
  pern2->calcTamanhos(pern2->calcDiff(po, po2));
  po2 = p2->retornaFim();
  pern1->SetBancoPedal(po, po2);
  pern1->desenha();
}

//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
  printf("\nTecla: %d", key);
  if (key < 200)
  {
    opcao = key;
  }

  switch (key)
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
}

int main(void)
{
  initCanvas(LARGURA_TELA, ALTURA_TELA, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3, 4 ou 5");

  r1 = new roda(85, 75, 4);
  r2 = new roda(85, 75, 4);
  q = new quadro(100);
  p1 = new pedal(30, 0);
  p2 = new pedal(30, PI_2 * 0.5);
  t = new torso(100);
  pern1 = new perna();
  pern2 = new perna();

  //b = new Bola();
  //r = new Relogio();

  runCanvas();
}

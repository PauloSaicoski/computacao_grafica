/*********************************************************************
// Canvas para desenho - Versao CPP.
//  Autor: Cesar Tadeu Pozzer
//         10/2007
// *********************************************************************/

/*
  Autor: Paulo Saicoski Sarkis
         05/2019

  Matricula: 201612057

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
bicicleta *bike = nullptr;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{

  bike->desenha(LARGURA_TELA/2, ALTURA_TELA/4);
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

}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{

}

int main(void)
{
  initCanvas(LARGURA_TELA, ALTURA_TELA, "CGT2 pssarkis");

  bike = new bicicleta(100, 0.01);

  runCanvas();
}

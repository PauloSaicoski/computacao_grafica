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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Matrizes.h"
#include "Bicicleta.h"

//variaveis globais
int   opcao  = 50;
float global = 0;
ponto* teste = nullptr;
roda* r = nullptr;


//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
  r = new roda(50, 40, 3);
  r->desenha(250, 250);
  teste->SetCoordenadas(300, 300);
  color(1, 0, 0);
  //circleFill(teste->x, teste->y, 50, 10);
  static float angulo = 0;
  angulo+=0.01;
  matriz transforma;
  transforma.MatrizRotaEmEixo(angulo, 250, 250);
  teste->MultiplicadoPorMatriz(transforma.pontos);

  color(0, 0, 1);
  //circle(teste->x, teste->y, 50, 10);


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
}

int main(void)
{
   initCanvas(500,500, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3, 4 ou 5");

   teste = new ponto(300, 300);

   //b = new Bola();
   //r = new Relogio();

   runCanvas();
}

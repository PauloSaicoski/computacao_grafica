/*********************************************************************
// Canvas para desenho - Versao CPP.
//  Autor: Cesar Tadeu Pozzer
//         10/2007
// *********************************************************************/

/*
  Autor: Paulo Saicoski Sarkis
         06/2019

  Matricula: 201612057

*/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include "gl_canvas2d.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Matrizes2d.h"
#include "Matrizes3d.h"
#include "curvas.h"
#include "arrastaveis.h"

#define LARGURA 1200
#define ALTURA 900
#define DISTANCIA_IMAGEM 600
#define DISTANCIA_TELA 300

//variaveis globais
int   opcao  = 50;
float global = 0;
Curva * curva = nullptr;
Arrastavel * a = nullptr;
std::vector<Arrastavel*> arrastaveis;
std::vector<Ponto2d*> pontos;
Ponto3d ** mesh = nullptr;
Ponto3d ** meshPerspectiva = nullptr;
int qntVertical, qntHorizontal;
bool perspectiva = true;
Ponto2d limiteMin(LARGURA*0.1, ALTURA*0.1);
Ponto2d limiteMax(LARGURA*0.45, ALTURA*0.9);
Matriz3d rotacao;
float velocidadeRotacao;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
    color(1, 0, 1);
    text(5, getAltura()-10, "Espaco alterna entre perspectiva e ortogonal");
    text(5, getAltura()-25, "Enter restaura a orientacao original da imagem");
    text(5, getAltura()-40, "z e x alteram a quantia de amostras da mesh, na vertical");
    text(5, getAltura()-55, "a e s alteram a quantia de amostras da mesh, na vertical");
    text(5, 10, "c e v giram a imagem no eixo Y");
    text(5, 25, "d e f giram a imagem no eixo Z");
    text(5, 40, "g e b giram a imagem no eixo X");
    text(5, 55, "h e n adicionam e removem pontos de controle da curva");
    limiteMin.SetCoordenadas(getLargura()*0.1, getAltura()*0.1);
    limiteMax.SetCoordenadas(getLargura()*0.45, getAltura()*0.9); // limites de movimentacao dos arrastaveis e da curva de bezier
    color(0.5, 0.5, 0.5);
    rect(limiteMin.x, limiteMin.y, limiteMax.x, limiteMax.y); // representacao visual dos limites
    for(int a = 0; a<arrastaveis.size(); a++){
        color(0,0,0);
        arrastaveis[a]->desenha();
        //pontos[a]->SetCoordenadas(arrastaveis[a]->getPonto()->x, arrastaveis[a]->getPonto()->y);
        if(a>0){
            color(0.8, 0.8, 0.8);
            line(pontos[a]->x, pontos[a]->y, pontos[a-1]->x, pontos[a-1]->y);
        }
    }
    Ponto3d aux;
    Matriz3d m;
    for(int c = 0; c < qntHorizontal; c++){ // for que interage com uma coluna inteira antes de passar para a proxima, de forma que o angulo de rotacao y para todos os pontos de cada execucao do laco seja o mesmo
        m.LimpaMatriz();
        m.MatrizTranslacao(-limiteMin.x, -(limiteMax.y + limiteMin.y)/2, 0); // considera o meio da area dos arrastaveis como origem
        m.MatrizRotacaoEixoY(c*(PI_2/(float)qntHorizontal)); // aplica rotacao em y
        for(int l = 0; l < qntVertical; l++){
            aux = curva->GetPonto(l*(1/(float)(qntVertical-1)));
            aux.MultiplicadoPorMatriz(m.pontos); // multiplica todos os pontos de uma mesma coluna pela mesma matriz calculada anteriormente
            mesh[l][c] = aux;

        }
    }

    m.LimpaMatriz();
    m.MultiplicadoPorMatriz(rotacao.pontos); // multiplica a matriz por uma matriz de rotacao que eh alterada pelo input do usuario
    m.MatrizTranslacao(0, 0, DISTANCIA_IMAGEM); // adiciona profundidade ao objeto apos as rotacoes
    if(perspectiva){
        m.MatrizPerspectiva(DISTANCIA_TELA); // a matriz eh multiplicada por uma matriz de perspectiva caso o modo perspectiva esteja ativo
    }
    m.MatrizTranslacao(getLargura()*0.75, (limiteMax.y + limiteMin.y)/2, 0); // posiciona a representacao do objeto a direita da area dos arrastaveis

    for(int l = 0; l < qntVertical; l++){
        for(int c = 0; c < qntHorizontal; c++){
            meshPerspectiva[l][c] = mesh[l][c];
            meshPerspectiva[l][c].MultiplicadoPorMatriz(m.pontos); // apenas a matriz a ser desenhada sofre alteracoes, enquanto o objeto original continua na origem

            color(0,0,1);
            // desenha o wireframe
            if(l<qntVertical-1){
                line(meshPerspectiva[l][c].x, meshPerspectiva[l][c].y, meshPerspectiva[l+1][c].x, meshPerspectiva[l+1][c].y); // linhas verticais
                line(meshPerspectiva[l][c].x, meshPerspectiva[l][c].y, meshPerspectiva[l+1][(c+1)%qntHorizontal].x, meshPerspectiva[l+1][(c+1)%qntHorizontal].y); // linhas na diagonal
            }
            line(meshPerspectiva[l][c].x, meshPerspectiva[l][c].y, meshPerspectiva[l][(c+1)%qntHorizontal].x, meshPerspectiva[l][(c+1)%qntHorizontal].y); // linhas horizontais


        }
    }

    color(1,0,1);
    curva->desenha();

}

void realoca_matrizes(int linhasAntes, int linhasDepois, int colunasDepois){
   for (int i = 0; i < linhasAntes; ++i){
       delete mesh[i];
       delete meshPerspectiva[i];
   }
   delete mesh;
   delete meshPerspectiva;
   mesh = new Ponto3d*[linhasDepois];
   meshPerspectiva = new Ponto3d*[linhasDepois];
   for (int i = 0; i < linhasDepois; ++i){
       mesh[i] = new Ponto3d[colunasDepois];
       meshPerspectiva[i] = new Ponto3d[colunasDepois];
   }
}

//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
   printf("\nTecla: %d" , key);
   /*
   if( key < 200 )
   {
      opcao = key;
   }
   */

   switch(key)
   {
      case 27: // esc
	     exit(0);
         break;
	  case 32: // espaco
         perspectiva = !perspectiva;
         break;
      case 122: // z
         if(qntVertical > 2){
            realoca_matrizes(qntVertical, qntVertical-1, qntHorizontal);
            qntVertical--;
         }
         break;
      case 120: // x
         realoca_matrizes(qntVertical, qntVertical+1, qntHorizontal);
         qntVertical++;
         break;
      case 97: // a
         if(qntHorizontal > 1){
            realoca_matrizes(qntVertical, qntVertical, qntHorizontal-1);
            qntHorizontal--;
         }
         break;
      case 115: // s
         realoca_matrizes(qntVertical, qntVertical, qntHorizontal+1);
         qntHorizontal++;
         break;
      case 99: // c
         rotacao.MatrizRotacaoEixoY(velocidadeRotacao);
         break;
      case 118: // v
         rotacao.MatrizRotacaoEixoY(-velocidadeRotacao);
         break;
      case 103: // g
         rotacao.MatrizRotacaoEixoX(velocidadeRotacao);
         break;
      case 98: // b
         rotacao.MatrizRotacaoEixoX(-velocidadeRotacao);
         break;
      case 100: // d
         rotacao.MatrizRotacaoEixoZ(velocidadeRotacao);
         break;
      case 102: // f
         rotacao.MatrizRotacaoEixoZ(-velocidadeRotacao);
         break;
      case 13: // enter
         rotacao.LimpaMatriz();
         break;
      case 104: // h
         arrastaveis.push_back(new Arrastavel(LARGURA*0.25, arrastaveis.size()*150, 10, &limiteMin, &limiteMax));
         pontos.push_back(arrastaveis[arrastaveis.size()-1]->getPonto());
         curva->atualizaPontos(pontos);
         break;
      case 110: // n
         if(pontos.size()>1){
             arrastaveis.erase(arrastaveis.end()-1);
             pontos.erase(pontos.end()-1);
             curva->atualizaPontos(pontos);
         }
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
   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, getAltura() - y);
   for(int a = 0; a<arrastaveis.size(); a++){
      arrastaveis[a]->atualiza(button, state, x, getAltura() - y);
   }

}

int main(void)
{
   initCanvas(LARGURA, ALTURA, "CGT3");
   arrastaveis.push_back(new Arrastavel(LARGURA*0.25, 100, 10, &limiteMin, &limiteMax));
   arrastaveis.push_back(new Arrastavel(LARGURA*0.25, 200, 10, &limiteMin, &limiteMax));
   arrastaveis.push_back(new Arrastavel(LARGURA*0.25, 300, 10, &limiteMin, &limiteMax));
   arrastaveis.push_back(new Arrastavel(LARGURA*0.25, 400, 10, &limiteMin, &limiteMax));
   velocidadeRotacao = 0.1;
   for(int a = 0; a<arrastaveis.size(); a++){
       pontos.push_back(arrastaveis[a]->getPonto());
       //pontos.push_back(new Ponto2d(arrastaveis[a]->getPonto().x, arrastaveis[a]->getPonto().y));
   }
   curva = new Curva(pontos);
   qntVertical = 10;
   qntHorizontal = 10;
   mesh = new Ponto3d*[qntVertical];
   meshPerspectiva = new Ponto3d*[qntVertical];
   for (int i = 0; i < qntVertical; ++i){
       mesh[i] = new Ponto3d[qntHorizontal];
       meshPerspectiva[i] = new Ponto3d[qntHorizontal];
   }


   runCanvas();
}

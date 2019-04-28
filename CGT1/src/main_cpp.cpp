/*********************************************************************
// Canvas para desenho - Versao CPP.
//  Autor: Cesar Tadeu Pozzer
//         10/2007
// *********************************************************************/

/*
  Autor: Paulo Saicoski Sarkis
         04/2019

  Matricula: 201612057

  Instru��es:
	  Selecione quais graficos devem ser impressos na tela, utilize as setas do teclado para alterar qual funcao base a ser exibida
*/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include "gl_canvas2d.h"
#include "figuras.h"

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



grafico *base = NULL;          //ponteiro utilizado para interacao com o grafico de funcoes bases
std::vector<checkBox> listaCB; //lista de botoes

std::vector<double> funcao_base(int base, int qnt){ //calcula as bases da dct
  std::vector<double> resultado;
  for(double u = 0; u < qnt; u++){
    resultado.push_back(cos(((M_PI*base)/(2*qnt))*(2*u + 1)));
  }
  return resultado;
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
  static float desenhavelXini, desenhavelYini, tamXmax, tamYmax; //variaveis referentes a area desenhavel para os graficos, dando espaco para os botoes a esquerda
  static int gradeX, gradeY, qnt1l, l, c, i;                     //variaveis utilizadas para desenhar a quantia de graficos selecionados na tela, tentando manter sempre um aspecto de grade
  color(0.5, 0, 1);
  text(5,5,"Utilize as setas para alterar qual funcao base a ser exibida");
  for(i = 0; i < listaCB.size(); i++){                           //desenha os botoes na tela
    listaCB[i].desenha(get_largura()/20, get_altura()*0.9 - (i*(get_altura()/2)/listaCB.size()), std::min(get_largura(), get_altura())/40, std::min(get_largura(), get_altura())/40);
  }
  desenhavelXini = get_largura()/10;
  desenhavelYini = get_altura()/20;
  tamXmax = get_largura()*0.95 - get_largura()/10;
  tamYmax = get_altura()*0.95 - get_altura()/20;
  gradeX = ceil(sqrt(listaGR.size()));
  gradeY = gradeX;
  qnt1l = gradeX - ((gradeX*gradeX) - listaGR.size()); //quantia de elementos na primeira linha, caso n�o hajam elementos suficientes para fechar uma grade quadrada a primeira linha tera menos elementos
  qnt1l = (qnt1l > 0)? qnt1l : gradeX;
  if(qnt1l > 1 && qnt1l > listaGR.size()/2){
    qnt1l--;
    if (qnt1l==1)gradeX--;                             //confere o caso de apenas 2 graficos estarem sendo desenhados
  }
  //
  if (listaGR.size()>2 && listaGR.size() <= gradeY*gradeY-gradeY){  //conserta grade com, por exemplo, 5 ou 6 elementos
    gradeY--;
  }/* */

  for(i = 0; i < qnt1l; i++){                //desenha os elementos da primeira linha, caso n�o hajam elementos suficientes para fechar uma grade quadrada os elementos da primeira linha ficam mais esticados
    listaGR[i]->desenha(desenhavelXini+(i*1.1*tamXmax/qnt1l), desenhavelYini+tamYmax-(tamYmax/gradeY), (tamXmax/qnt1l)*0.95, tamYmax*0.95/gradeY);
  }
  for(i = qnt1l; i < listaGR.size(); i++){   //desenha o resto dos graficos da grade
    c = (i-qnt1l)%(gradeX); //numero da coluna a ser desenhado
    l = (i-qnt1l)/gradeX;   //numero da linha
    listaGR[i]->desenha(desenhavelXini+(c*1.1*tamXmax/gradeX), desenhavelYini+tamYmax-(tamYmax/gradeY)*(gradeY-l), (tamXmax/gradeX)*0.95, tamYmax*0.95/gradeY);
  }
  //todas as posicoes na tela foram feitas utilizando porcentagens ou divisoes da tela, desta forma, caso a tela seja redimencionada, os desenhos se reajustam em "tempo real"

}

//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
   printf("\nTecla: %d" , key);
   if( key < 200 )
   {
      opcao = key;
   }
   static int i = 0;
   switch(key)
   {
      case 27:
	     exit(0);                                                 //tecla esc finaliza o programa
	     break;

      case 201:                                                 //seta cima
        i = (i+1)%base->amostras.size();
        base->amostras = funcao_base(i, base->amostras.size()); //passa para a proxima funcao base
      break;

      case 202:                                                 //seta direita
        i = (i+1)%base->amostras.size();
        base->amostras = funcao_base(i, base->amostras.size()); //passa para a proxima funcao base
      break;

      case 203:                                                 //seta baixo
        i = (i-1)%base->amostras.size();
        base->amostras = funcao_base(i, base->amostras.size()); //retorna para a funcao base anterior
      break;

      case 200:                                                 //seta esquerda
        i = (i-1)%base->amostras.size();
        base->amostras = funcao_base(i, base->amostras.size()); //retorna para a funcao base anterior
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
    a.atualiza(button, state, x, get_altura()-y); //atualiza os botoes conferindo a posicao e clique do mouse
   }
}

float C(float u){
  return (u==0)? (1/sqrt(2)) : 1;
}

std::vector<double> aplica_dct(std::vector<double> amostras){
  std::vector<double> dct;
  double qnt = amostras.size();
  for(int u = 0; u < qnt; u++){
    double soma = 0;
    for(int i = 0; i < qnt; i++){
      soma += C(u) * amostras[i] * cos(((M_PI*u)/(2*qnt))*(2*i + 1));
    }
    dct.push_back(sqrt(2/qnt)*soma);
  }
  return dct;
}

std::vector<double> aplica_idct(std::vector<double> dct){
  std::vector<double>idct;
  double qnt = dct.size();
  for(int i = 0; i < qnt; i++){
    double soma = 0;
    for(int u = 0; u < qnt; u++){
      soma += C(u) * dct[u] * cos(((M_PI*u)/(2*qnt))*(2*i + 1));
    }
    idct.push_back(sqrt(2/qnt)*soma);
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

void escreve_arquivo (char* arquivo, std::vector<signed char> vetor){
   std::ofstream idctfile;
   unsigned int j = vetor.size();
   char *c;
   c = (char*)&j;
   idctfile.open(arquivo, std::ios::binary);
   idctfile.write(c, 4);
   for (const auto &e : vetor) idctfile << e;
   idctfile.close();
}


int main(void)
{
  initCanvas(LARGURA_TELA,ALTURA_TELA, "Trabalho CGT1");

  std::vector<int> amostras;
  amostras = le_arquivo("input2.dct");

  int i;
  /*/
  for(i = 0; i<amostras.size(); i++){
    if(amostras[i]>=0){
      amostras[i]-=127;
    }
    else{
      amostras[i]+=128;
    }
  } /* */



  std::vector<double> amostras_d (amostras.begin(), amostras.end());
  std::vector<double> dct, idct, diff;

  dct = aplica_dct(amostras_d);
  idct = aplica_idct(dct);
  diff=verifica_diff(amostras_d,idct);

/*/ //prints de teste dos vetores dct e idct
  for(i = 0; i < dct.size(); i++){
    std::cout << "dct "<< i << " " << dct[i] << std::endl;
  }

  for(i = 0; i < idct.size(); i++){
    std::cout << "idct "<< i << " " << idct[i] << std::endl;
  }
/* */

   /* adiciona os vetores aos graficos, os graficos aos botoes e os botoes as listas */
   checkBox *cb = NULL;
   grafico *g = NULL;
   g = new grafico("input", amostras_d);
   cb = new checkBox(g);
   listaCB.push_back(*cb);
   g = new grafico("dct", dct);
   cb = new checkBox(g);
   listaCB.push_back(*cb);
   g = new grafico("idct", idct);
   cb = new checkBox(g);
   listaCB.push_back(*cb);
   g = new grafico("diff", diff);
   cb = new checkBox(g);
   listaCB.push_back(*cb);
   g = new grafico("base", funcao_base(0, amostras.size()));
   base = g;
   cb = new checkBox(g);
   listaCB.push_back(*cb);


   /*/  //descomentar esta secao para imprimir graficos referentes a mais um input
   std::vector<int> amostras2;
   amostras2 = le_arquivo("input2.dct");
   std::vector<double> amostras2_d (amostras2.begin(), amostras2.end());
   std::vector<double> dct2, idct2, diff2;
   dct2 = aplica_dct(amostras2_d);
   idct2 = aplica_idct(dct2);
   diff2 = verifica_diff(amostras2_d, idct2);
   g = new grafico("input2", amostras2_d);
   cb = new checkBox(g);
   listaCB.push_back(*cb);
   g = new grafico("dct2", dct2);
   cb = new checkBox(g);
   listaCB.push_back(*cb);
   g = new grafico("idct2", idct2);
   cb = new checkBox(g);
   listaCB.push_back(*cb);
   g = new grafico("diff2", diff2);
   cb = new checkBox(g);
   listaCB.push_back(*cb);
   /* */

   std::vector<signed char> saida; //preenche o vetor de saida com os valores da idct, conferindo se os mesmos nao sao maiores do que uma variavel char aceita
   for (i = 0; i < idct.size(); i++){
      if(idct[i]> 127){
        saida.push_back(127);
      }
      else if (idct[i]<-128){
        saida.push_back(-128);
      }
      else{
        saida.push_back(round(idct[i]));
      }
   }

   escreve_arquivo("output.dct", saida);

   runCanvas();


   for(auto a = listaCB.begin(); a<listaCB.end(); a++){ //laco de liberacao da lista de botoes
     delete a->g;  //libera o grafico associado ao botao
     delete &(*a); //libera o botao em si
   }

}

/*
  Autor: Paulo Saicoski Sarkis
         05/2019

  Matricula: 201612057

*/

//  Descricao:
//    Este TAD eh o responsavel por todas as classes utilizadas para desenhar a bicicleta e seus elementos



#ifndef __BICICLETA_H__
#define __BICICLETA_H__

#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "gl_canvas2d.h"
#include "Matrizes.h"

class figura
{
public:
  std::vector<ponto *> pontos; // vetor de pontos para o desenho das figuras, posicionados sem escala ou posicionamento final, com a origem como referencia

  virtual void desenha(float referenciaX, float referenciaY) {}
  virtual void rotaEmEixo(float rad); // gira os pontos do vetor da classe em volta da origem em rad graus (valor em radianos)
};

void figura::rotaEmEixo(float rad)
{
  matriz m;
  m.MatrizRotacao(rad);

  for (int i = 0; i < pontos.size(); i++)
  {
    pontos[i]->MultiplicadoPorMatriz(m.pontos);
  }
}

class roda : public figura
{

public:
  float raioExterno;
  float raioInterno;
  float anguloEntreAros;

  roda(float raioExterno, float raioInterno, int qntAros);

  void desenha(float referenciaX, float referenciaY);
};

roda::roda(float raioExterno, float raioInterno, int qntAros)
{
  this->raioExterno = raioExterno;
  this->raioInterno = raioInterno;
  anguloEntreAros = PI_2 / ((float)2 * qntAros);  //calcula o angulo entre os aros para distribuilos de forma simetrica dentro da roda
  for (int i = 0; i < qntAros * 2; i++)
  {
    pontos.push_back(new ponto(cos(i * anguloEntreAros), sin(i * anguloEntreAros))); //adiciona os pontos dos aros sem escala e em volta da origem, para que possam ser escalados e rotacionados com facilidade posteriormente
  }
}

void roda::desenha(float referenciaX, float referenciaY)
{
  for (int i = raioInterno; i < raioExterno; i++)
  {
    circle(referenciaX, referenciaY, i, 50);
  }
  matriz m;
  m.MatrizEscala(raioInterno, raioInterno);
  m.MatrizTranslacao(referenciaX, referenciaY);  // as matrizes de escala e translacao são calculadas uma unica vez antes de serem utilizadas para multiplicar os pontos
  ponto p;
  for (int i = 0; i < pontos.size(); i++)
  {
    p.SetCoordenadas(pontos[i]->x, pontos[i]->y); // os poontos reposicionados sao temporarios, para que os originais continuem na origem
    p.MultiplicadoPorMatriz(m.pontos);            // todos os pontos sao multiplicados pela mesma matriz
    line(referenciaX, referenciaY, p.x, p.y);
  }
}

class quadro : public figura
{
private:
  ponto rodaFrontal;
  ponto rodaTraseira;
  ponto banco;
  float escala;

public:
  quadro(float escala);
  void atualizaRodasBanco(ponto rodaFrontal, ponto rodaTraseira, ponto banco);  // atualiza as variaveis que serao necessarias para outras classes
  void desenha(float referenciaX, float referenciaY);
  ponto retornaRoda(bool frontal); // retorna a posicao da roda determinada pelo argumento, necessario para a classe roda
  ponto retornaBanco();            // retorna a posicao do banco, necessario para a classe perna
};

quadro::quadro(float escala)
{

  this->escala = escala;
  pontos.push_back(new ponto(0, 0));    //i = 0 -> catraca
  pontos.push_back(new ponto(-1, 0));   //i = 1 -> roda traseira
  pontos.push_back(new ponto(-0.5, 1)); //i = 2 -> banco
  pontos.push_back(new ponto(1, 1));    //i = 3 -> ponto quadro abaixo do guidao
  pontos.push_back(new ponto(1.5, 0));  //i = 4 -> roda frontal
  float y = 1.3;                        //altura desejada para o guidao
  float x = (3 - y) / 2;                //x calculado para guidao permanescer na reta criada entre roda frontal e ultimo ponto do quadro
  pontos.push_back(new ponto(x, y));    //i = 5 -> guidao
}

void quadro::atualizaRodasBanco(ponto rodaFrontal, ponto rodaTraseira, ponto banco)
{
  this->rodaFrontal = rodaFrontal;
  this->rodaTraseira = rodaTraseira;
  this->banco = banco;
}

void Bezier(ponto p1, ponto p2, ponto p3) // funcao utilizada para desenhar a curva de bezier com 3 pontos utilizada na parte superior do quadro da bicicleta
{
  float x, y;
  for (float t = 0; t < 1; t += 0.001)
  {
    x = pow(1 - t, 2) * p1.x + 2 * t * (1 - t) * p2.x + pow(t, 2) * p3.x;
    y = pow(1 - t, 2) * p1.y + 2 * t * (1 - t) * p2.y + pow(t, 2) * p3.y;
    point(x, y);
  }
}
void quadro::desenha(float referenciaX, float referenciaY)
{

  matriz m;
  ponto p[6];
  m.MatrizEscala(escala, escala);
  m.MatrizTranslacao(referenciaX, referenciaY);
  for (int i = 0; i < pontos.size(); i++)
  {
    p[i].SetCoordenadas(pontos[i]->x, pontos[i]->y);
    p[i].MultiplicadoPorMatriz(m.pontos);
    circleFill(p[i].x, p[i].y, 5, 10);
  }

  for (int i = 1; i <= 3; i++)
  { //desenha as linhas que se ligam a catraca
    line(p[0].x, p[0].y, p[i].x, p[i].y);
  }

  Bezier(p[1], p[2], p[3]); //bezier para desenhar a parte superior do quadro

  line(p[4].x, p[4].y, p[5].x, p[5].y); //linha entre a roda frontal e guidao, passando pelo final do quadro
  atualizaRodasBanco(p[4], p[1], p[2]);
}

ponto quadro::retornaRoda(bool frontal)
{
  if (frontal)
  {
    return rodaFrontal;
  }
  else
  {
    return rodaTraseira;
  }
}

ponto quadro::retornaBanco()
{
  return banco;
}

class pedal : public figura
{
private:
  ponto fim; // referencia para a posicao atual do pedal, necessario para a classe perna
  float raio;
  float referenciaX;
  float referenciaY;

public:
  pedal(float raio, float anguloInicial);
  void desenha(float referenciaX, float referenciaY);
  ponto retornaFim();          // retorna a posicao atual do pedal, necessario para a classe perna
  ponto retornaFim(float rad); // retorna a posicao do pedal quando este estiver com o angulo rad, necessario para calcular as larguras da classe perna
  void atualizaFim(ponto fim); // atualiza a variavel fim
};

pedal::pedal(float raio, float anguloInicial)
{
  this->raio = raio;
  pontos.push_back(new ponto(cos(anguloInicial), sin(anguloInicial)));
}

void pedal::atualizaFim(ponto fim)
{
  this->fim = fim;
}

void pedal::desenha(float referenciaX, float referenciaY)
{

  this->referenciaX = referenciaX;
  this->referenciaY = referenciaY;
  matriz m;
  m.MatrizEscala(raio, raio);
  m.MatrizTranslacao(referenciaX, referenciaY);
  ponto p;
  p.SetCoordenadas(pontos[0]->x, pontos[0]->y);
  p.MultiplicadoPorMatriz(m.pontos);
  atualizaFim(p);
  line(referenciaX, referenciaY, p.x, p.y);
  circleFill(p.x, p.y, 5, 10);
}

ponto pedal::retornaFim()
{
  return fim;
}

ponto pedal::retornaFim(float rad)
{
  ponto p(cos(rad), sin(rad));
  matriz m;
  m.MatrizEscala(raio, raio);
  m.MatrizTranslacao(referenciaX, referenciaY);
  p.MultiplicadoPorMatriz(m.pontos);
  return p;
}

class torso : public figura
{
private:
  float escala;

public:
  torso(float escala);
  void desenha(float referenciaX, float referenciaY);
  void setMao(ponto guidao);
};

torso::torso(float escala)
{

  this->escala = escala;
  pontos.push_back(new ponto(-0.5, 1)); //i = 0 -> cintura
  pontos.push_back(new ponto(0, 2));    //i = 1 -> cabeca
  float y = 1.6;                        //altura desejada para o ombro
  float x = (y - 2) / 2;                //x calculado para ombro permanescer na reta criada entre cabeca e cintura
  pontos.push_back(new ponto(x, y));    //i = 2 -> ombro
  y = 1.3;                              //altura do guidao
  x = (3 - y) / 2;                      //posicao x do guidao
  pontos.push_back(new ponto(x, y));    //i = 3 -> mao
                                        //pontos.push_back(new ponto(0,0));    //i = 4 -> cotovelo
}

void torso::desenha(float referenciaX, float referenciaY)
{
  matriz m;
  m.MatrizEscala(escala, escala);
  m.MatrizTranslacao(referenciaX, referenciaY);
  ponto p[4];
  for (int i = 0; i < pontos.size(); i++)
  {
    p[i].SetCoordenadas(pontos[i]->x, pontos[i]->y);
    p[i].MultiplicadoPorMatriz(m.pontos);
  }
  circleFill(p[1].x, p[1].y, escala*0.3, 50);
  line(p[1].x, p[1].y, p[0].x, p[0].y);  // linha da cintura ate a cabeca
  line(p[2].x, p[2].y, p[3].x, p[3].y);  // linha do ombro ate a mao
}

class perna : public figura
{
private:
  float tamA, tamB;  // tamanhos da primeira e segunda parte da perna
  ponto banco, pedal;// referencias das posicoes do banco e pedal da bicicleta

public:
  perna() {}
  float calcDiff(ponto A, ponto B);             // calcula a distancia entre 2 pontos
  float calcAngulo(ponto A, ponto B);           // calcula o angulo entre 2 pontos
  void calcTamanhos(float distMax);             // calcula os tamanhos de tamA e tamB
  void SetBancoPedal(ponto banco, ponto pedal); // recebe as referencias de posicao do banco e do pedal
  void desenha();
};

float perna::calcDiff(ponto A, ponto B)
{
  float dist;
  dist = sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
  return dist;
}

float perna::calcAngulo(ponto A, ponto B)
{
  float rad;
  rad = atan2(A.y - B.y, A.x - B.x);
  return rad;
}

void perna::calcTamanhos(float distMax)
{
  tamA = distMax * 0.51;
  tamB = distMax * 0.51;
}

void perna::SetBancoPedal(ponto banco, ponto pedal)
{
  this->banco = banco;
  this->pedal = pedal;
}

void perna::desenha()
{
  float angBancoPedal;
  float dist;
  angBancoPedal = calcAngulo(pedal, banco); // calcula o angulo entre o vertice banco->pedal e o eixo X
  dist = calcDiff(banco, pedal);            // calcula a distancia entre o banco e o pedal
  float anguloJoelhoTemporario = acos((pow(tamA, 2) + pow(dist, 2) - pow(tamB, 2)) / (2 * tamA * dist)); // calcula o angulo entre os vertices banco->joelho e banco->pedal, considerando um triangulo (banco, joelho, pedal) onde os lados sao tamA, tamB e dist
  float radFinal = angBancoPedal + anguloJoelhoTemporario; // calcula o angulo do vertice banco->joelho em relacao ao eixo X, considerando que angBancoPedal eh um valor negativo e alguloJoelhoTemporario eh um valor positivo
  ponto p(cos(radFinal), sin(radFinal)); // calcula a posicao do joelho ao ter o angulo em relacao ao eixo x
  matriz m;
  m.MatrizEscala(tamA, tamA);
  m.MatrizTranslacao(banco.x, banco.y);
  p.MultiplicadoPorMatriz(m.pontos);
  line(banco.x, banco.y, p.x, p.y);  // desenha a parte superior da perna
  line(p.x, p.y, pedal.x, pedal.y);  // desenha a parte inferior da perna
  circleFill(p.x, p.y, 3, 10);       // desenha um circulo no joelho para melhor visualisacao quando este estiver na frente ou atras de outro objeto
}


class bicicleta:public figura{  // classe para reuniao das outras classes

private:
    roda *rodaFrontal;
    roda *rodaTraseira;
    quadro *q;
    pedal *pedalFrente;
    pedal *pedalTras;
    torso *t;
    perna *pernaFrente;
    perna *pernaTras;
    float velocidade;
    ponto auxRoda, auxBanco, auxPedal, auxReferencia;

public:
    bicicleta(float escala, float velocidade);
    void desenha(float referenciaX, float referenciaY);

};

bicicleta::bicicleta(float escala, float velocidade){
    this->velocidade = -velocidade; // angulo recebido como valor positivo giraria a roda para tras (anti-horario), entao negativo o valor
    rodaFrontal = new roda(escala * 0.85, escala * 0.75, 4);  // proporcao definida para que as rodas tenham tamanhos visualmente similares ao resto da bicicleta
    rodaTraseira = new roda(escala * 0.85, escala * 0.75, 4);
    q = new quadro(escala);
    pedalFrente = new pedal(escala * 0.3, 0);
    pedalTras = new pedal(escala * 0.3, PI_2*0.5); // enquanto um pedal comeca para frente, o outro comeca para tras
    t = new torso(escala);
    pernaFrente = new perna();
    pernaTras = new perna();

}

void bicicleta::desenha(float referenciaX, float referenciaY){


    auxReferencia.SetCoordenadas(referenciaX, referenciaY);
    color(0.5,0.2,0.2);
    pedalTras->desenha(referenciaX, referenciaY);
    pedalTras->rotaEmEixo(velocidade*0.5);                                            //os pedais giram mais de vagar que as rodas
    auxPedal = pedalTras->retornaFim(pernaTras->calcAngulo(auxReferencia, auxBanco)); // calcula o angulo entre o banco e a catraca (que esta na referencia), para entao aplicar este angulo no pedal e verificar qual a distancia maxima entre o banco e o pedal
    pernaTras->calcTamanhos(pernaTras->calcDiff(auxBanco, auxPedal));                 // os tamanhos das pernas sao decididos a partir da distancia maxima entre banco e pedal
    auxPedal = pedalTras->retornaFim();
    pernaTras->SetBancoPedal(auxBanco, auxPedal);
    color(0.4,0.4,0.4); // a perna de tras eh cinza, enquanto a da frente eh preta
    pernaTras->desenha();

    color(1,0,0);
    q->desenha(referenciaX, referenciaY);
    auxBanco = q->retornaBanco();
    auxRoda = q->retornaRoda(false);
    color(0.4,0.4,0.6);
    rodaTraseira->desenha(auxRoda.x, auxRoda.y);
    rodaTraseira->rotaEmEixo(velocidade);
    auxRoda = q->retornaRoda(true);
    rodaFrontal->desenha(auxRoda.x, auxRoda.y);
    rodaFrontal->rotaEmEixo(velocidade);
    color(0,0,0);
    t->desenha(referenciaX, referenciaY);
    color(1,0.5,0.5);
    pedalFrente->desenha(referenciaX, referenciaY);
    pedalFrente->rotaEmEixo(velocidade*0.5);
    auxPedal = pedalFrente->retornaFim(pernaFrente->calcAngulo(auxReferencia, auxBanco));
    pernaFrente->calcTamanhos(pernaFrente->calcDiff(auxBanco, auxPedal));
    auxPedal = pedalFrente->retornaFim();
    pernaFrente->SetBancoPedal(auxBanco, auxPedal);
    color(0,0,0);
    pernaFrente->desenha();


}

#endif

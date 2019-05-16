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
  std::vector<ponto *> pontos;

  virtual void desenha(float referenciaX, float referenciaY) {}
  virtual void rotaEmEixo(float rad);
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
  anguloEntreAros = PI_2 / ((float)2 * qntAros);
  std::cout << anguloEntreAros << std::endl;
  for (int i = 0; i < qntAros * 2; i++)
  {
    pontos.push_back(new ponto(cos(i * anguloEntreAros), sin(i * anguloEntreAros)));
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
  m.MatrizTranslacao(referenciaX, referenciaY);
  ponto p;
  for (int i = 0; i < pontos.size(); i++)
  {
    p.SetCoordenadas(pontos[i]->x, pontos[i]->y);
    p.MultiplicadoPorMatriz(m.pontos);
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
  void atualizaRodasBanco(ponto rodaFrontal, ponto rodaTraseira, ponto banco);
  void desenha(float referenciaX, float referenciaY);
  ponto retornaRoda(bool frontal);
  ponto retornaBanco();
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
  float x = (3 - y) / 2;
  //x calculado para guidao permanescer na reta criada entre roda frontal e ultimo ponto do quadro
  pontos.push_back(new ponto(x, y)); //i = 5 -> guidao
}

void quadro::atualizaRodasBanco(ponto rodaFrontal, ponto rodaTraseira, ponto banco)
{
  this->rodaFrontal = rodaFrontal;
  this->rodaTraseira = rodaTraseira;
  this->banco = banco;
}

void Bezier(ponto p1, ponto p2, ponto p3)
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
  ponto fim;
  float raio;
  float referenciaX;
  float referenciaY;

public:
  pedal(float raio, float anguloInicial);
  void desenha(float referenciaX, float referenciaY);
  ponto retornaFim();
  ponto retornaFim(float rad);
  void atualizaFim(ponto fim);
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

void torso::setMao(ponto guidao)
{
  pontos[3]->SetCoordenadas(guidao.x, guidao.y);
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
  color(0, 0, 0);
  circleFill(p[1].x, p[1].y, 30, 10);
  color(0, 0, 0);
  line(p[1].x, p[1].y, p[0].x, p[0].y);
  line(p[2].x, p[2].y, p[3].x, p[3].y);
}

class perna : public figura
{
private:
  float tamA, tamB; //tamanhos da primeira e segunda parte da perna
  ponto banco, pedal;

public:
  perna() {}
  float calcDiff(ponto A, ponto B);
  float calcAngulo(ponto A, ponto B);
  void calcTamanhos(float distMax);
  void SetBancoPedal(ponto banco, ponto pedal);
  void desenha();
};

float perna::calcDiff(ponto A, ponto B)
{
  float dist;
  dist = sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
  std::cout << dist << std::endl;
  return dist;
}

float perna::calcAngulo(ponto A, ponto B)
{
  float rad;
  rad = atan2(A.y - B.y, A.x - B.x);
  std::cout << rad << std::endl;
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
  angBancoPedal = calcAngulo(pedal, banco);
  dist = calcDiff(banco, pedal);
  float anguloJoelhoTemporario = acos((pow(tamA, 2) + pow(dist, 2) - pow(tamB, 2)) / (2 * tamA * dist));
  float radFinal = angBancoPedal + anguloJoelhoTemporario;
  ponto p(cos(radFinal), sin(radFinal));
  matriz m;
  m.MatrizEscala(tamA, tamA);
  m.MatrizTranslacao(banco.x, banco.y);
  p.MultiplicadoPorMatriz(m.pontos);
  line(banco.x, banco.y, p.x, p.y);
  line(p.x, p.y, pedal.x, pedal.y);
  circleFill(p.x, p.y, 3, 10);
}


class bicicleta:public figura{

};

#endif

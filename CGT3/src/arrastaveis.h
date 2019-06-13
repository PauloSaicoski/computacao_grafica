/*
  Autor: Paulo Saicoski Sarkis
         06/2019

  Matricula: 201612057

*/

//  Descricao:
//    Este TAD eh o responsavel por todas as criacoes e interacoes de elementos arrastaveis com o mouse

#ifndef __ARRASTAVEIS_H__
#define __ARRASTAVEIS_H__

#include "gl_canvas2d.h"
#include "Matrizes2d.h"

class Arrastavel{
private:

    Ponto2d ponto;
    float raio;
    bool sobreposto;
    bool segurado;
    Ponto2d * limiteMin; // limites de posicao do arrastavel
    Ponto2d * limiteMax;

    void limitaPosicao(); // metodo que limita a movimentacao do arrastavel

public:

    Arrastavel(){
        ponto.SetCoordenadas(0,0);
        raio = 10;
        sobreposto = false;
        segurado = false;
        limiteMin = new Ponto2d(0,0);
        limiteMax = new Ponto2d(getLargura(), getAltura());
    }
    Arrastavel(float raio){
        ponto.SetCoordenadas(0,0);
        this->raio = raio;
        sobreposto = false;
        segurado = false;
        limiteMin = new Ponto2d(0,0);
        limiteMax = new Ponto2d(getLargura(), getAltura());
    }
    Arrastavel(float x, float y){
        ponto.SetCoordenadas(x, y);
        raio = 10;
        sobreposto = false;
        segurado = false;
        limiteMin = new Ponto2d(0,0);
        limiteMax = new Ponto2d(getLargura(), getAltura());
    }
    Arrastavel(float x, float y, float raio){
        ponto.SetCoordenadas(x, y);
        this->raio = raio;
        sobreposto = false;
        segurado = false;
        limiteMin = new Ponto2d(0,0);
        limiteMax = new Ponto2d(getLargura(), getAltura());
    }
    Arrastavel(Ponto2d *limiteMin, Ponto2d *limiteMax){
        ponto.SetCoordenadas(0,0);
        raio = 10;
        sobreposto = false;
        segurado = false;
        this->limiteMin = limiteMin;
        this->limiteMax = limiteMax;
    }
    Arrastavel(float raio, Ponto2d *limiteMin, Ponto2d *limiteMax){
        ponto.SetCoordenadas(0,0);
        this->raio = raio;
        sobreposto = false;
        segurado = false;
        this->limiteMin = limiteMin;
        this->limiteMax = limiteMax;
    }
    Arrastavel(float x, float y, Ponto2d *limiteMin, Ponto2d *limiteMax){
        ponto.SetCoordenadas(x, y);
        raio = 10;
        sobreposto = false;
        segurado = false;
        this->limiteMin = limiteMin;
        this->limiteMax = limiteMax;
    }
    Arrastavel(float x, float y, float raio, Ponto2d *limiteMin, Ponto2d *limiteMax){
        ponto.SetCoordenadas(x, y);
        this->raio = raio;
        sobreposto = false;
        segurado = false;
        this->limiteMin = limiteMin;
        this->limiteMax = limiteMax;
    }
    void atualiza(int button, int state, int x, int y); // metodo que confere as interacoes do arrastavel com o mouse
    void desenha();
    Ponto2d* getPonto(); // retorna o ponteiro da posicao atual do arrastavel


};

void Arrastavel::limitaPosicao(){
    ponto.x = (ponto.x > limiteMin->x)? ponto.x : limiteMin->x;
    ponto.x = (ponto.x < limiteMax->x)? ponto.x : limiteMax->x;
    ponto.y = (ponto.y > limiteMin->y)? ponto.y : limiteMin->y;
    ponto.y = (ponto.y < limiteMax->y)? ponto.y : limiteMax->y;
}

void Arrastavel::desenha(){
    if(segurado){
        color(1,0,0);
    }else if(sobreposto){
        color(0,0,1);
    }else{
        color(0,0,0);
    }
    circle(ponto.x, ponto.y, raio, 10);

    // garante que o ponto fique dentro dos limites
    limitaPosicao();

}

float dist(Ponto2d p1, Ponto2d p2){
    float dist;
    dist = sqrt(pow(p2.x-p1.x, 2)+pow(p2.y-p1.y, 2));
    return dist;
}

void Arrastavel::atualiza(int button, int state, int x, int y){

    Ponto2d mouse(x,y);
    if(dist(mouse, ponto) < raio){
        sobreposto = true;
    }
    else{
        sobreposto = false;
    }
    if(sobreposto && button == 0 && state == 0){
        segurado = true;
    }
    if(segurado){
        ponto = mouse; // o arrastavel segue o mouse

        if(button == 0 && state == 1){ // confere se o arrastavel foi solto
            segurado = false;
        }
    }
    // garante que o ponto fique dentro dos limites novamente, para melhor interacao com o mouse
    limitaPosicao();

}

Ponto2d* Arrastavel::getPonto(){
    return &ponto;
}


#endif

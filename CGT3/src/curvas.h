/*
  Autor: Paulo Saicoski Sarkis
         06/2019

  Matricula: 201612057

*/

//  Descricao:
//    Este TAD eh o responsavel por todas as criacoes e interacoes da curva de bezier

#ifndef __CURVA_H__
#define __CURVA_H__


#include "gl_canvas2d.h"
#include "Matrizes2d.h"
#include <vector>

class Curva{
private:
    std::vector<Ponto2d*> pontos; // os pontos de controle da curva
    int qntPontos; // quantia de pontos de controle
    float FuncBezier(int i, float t); // as funcoes de bezier, com quantia variavel de pontos de controle

public:
    Curva(std::vector<Ponto2d*> pontos);
    void desenha();
    Ponto2d GetPonto(float t); // retorna um ponto2d para determinado valor de t
    void atualizaPontos(std::vector<Ponto2d*> pontos); // atualiza os pontos de controle, em caso de adicao ou remocao de algum elemento


};

void Curva::atualizaPontos(std::vector<Ponto2d*> pontos){
    this->pontos = pontos;
    qntPontos = pontos.size();
}

Curva::Curva(std::vector<Ponto2d*> pontos){
    atualizaPontos(pontos);

}

void Curva::desenha(){
    Ponto2d p;
    float t;
    for(t = 0; t <= 1; t+= 0.0001){
        p.x = 0;
        p.y = 0;
        for(int i = 0; i < qntPontos; i++){
            p.x += FuncBezier(i, t)*pontos[i]->x;
            p.y += FuncBezier(i, t)*pontos[i]->y;
        }
        point(p.x, p.y);

    }
}

float fatorial(int num){
    float res = 1;
    if(num > 0){
        for (int i = 1; i <= num; i++){
            res*=i;
        }
    }
    return res;
}

float Curva::FuncBezier(int i, float t){
    float ni;
    float qnt = qntPontos - 1;

    ni = fatorial(qnt)/(fatorial(i)*fatorial(qnt-i));
    return ni*pow(t, i)*pow(1-t, qnt-i);

}

Ponto2d Curva::GetPonto(float t){
    if(t>1){
        t=1;
    }else if(t<0){
        t=0;
    }
    Ponto2d p;

    for(int i = 0; i < qntPontos; i++){
        p.x += FuncBezier(i, t)*pontos[i]->x;
        p.y += FuncBezier(i, t)*pontos[i]->y;
    }

    return p;
}



#endif

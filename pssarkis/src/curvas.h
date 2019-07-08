/*
  Autor: Paulo Saicoski Sarkis
         07/2019

  Matricula: 201612057

*/

//  Descricao:
//    Este TAD eh o responsavel por todas as criacoes e interacoes da curva de b-spline
#ifndef __CURVA_H__
#define __CURVA_H__

#include "Ponto3D.h"
#include <vector>
#include <iostream>

Ponto3D BSpline(double t, Ponto3D p0, Ponto3D p1, Ponto3D p2, Ponto3D p3)
{
    Ponto3D point;
    point.x = t * t * t * ((-1) * p0.x + 3 * p1.x - 3 * p2.x + p3.x) / 6;
    point.x += t * t * (3 * p0.x - 6 * p1.x + 3 * p2.x) / 6;
    point.x += t * ((-3) * p0.x + 3 * p2.x) / 6;
    point.x += (p0.x + 4 * p1.x + p2.x) / 6;

    point.y = t * t * t * ((-1) * p0.y + 3 * p1.y - 3 * p2.y + p3.y) / 6;
    point.y += t * t * (3 * p0.y - 6 * p1.y + 3 * p2.y) / 6;
    point.y += t * ((-3) * p0.y + 3 * p2.y) / 6;
    point.y += (p0.y + 4 * p1.y + p2.y) / 6;

    point.z = t * t * t * ((-1) * p0.z + 3 * p1.z - 3 * p2.z + p3.z) / 6;
    point.z += t * t * (3 * p0.z - 6 * p1.z + 3 * p2.z) / 6;
    point.z += t * ((-3) * p0.z + 3 * p2.z) / 6;
    point.z += (p0.z + 4 * p1.z + p2.z) / 6;

    return point;
}

std::vector<Ponto3D> getBSplineList(std::vector<Ponto3D> pontos, int qntSegmentos)
{
    if (pontos.size() < 4)
    {
        std::cout << "Quantia de pontos insuficientes para B-Spline." << std::endl;
        exit(1);
    }
    std::vector<Ponto3D> retorno;
    for (int i = 0; i < pontos.size(); i++)
    {
        for (double j = 0; j < 1; j += 1 / (double)qntSegmentos)
        {
            retorno.push_back(BSpline(j, pontos[i], pontos[(i + 1) % pontos.size()], pontos[(i + 2) % pontos.size()], pontos[(i + 3) % pontos.size()]));
        }
    }
    return retorno;
}
#endif // __CURVA_H__

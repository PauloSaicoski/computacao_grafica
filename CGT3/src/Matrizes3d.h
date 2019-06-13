/*
  Autor: Paulo Saicoski Sarkis
         06/2019

  Matricula: 201612057

*/

//  Descricao:
//    Este TAD eh o responsavel por todas as criacoes e interacoes entre matrizes ou entre ponto e matriz em 3d

#ifndef __MATRIZES_3D_H__
#define __MATRIZES_3D_H__

#include <math.h>
#include "Matrizes2d.h"

class Ponto3d
{

private:
  float coordenadas[4]; // vetor 4x1 com as coordenadas

public:
  float x; //valores de facil acesso para x e y
  float y;
  float z;

  Ponto3d(float x, float y, float z)
  {
    SetCoordenadas(x, y, z);
  }
  Ponto3d(float x, float y)
  {
    SetCoordenadas(x, y, 0);
  }
  Ponto3d()
  {
    SetCoordenadas(0, 0, 0);
  }

  void operator = (Ponto2d p);
  void AtualizaCoordenadas();                     // unico metodo que realmente altera o vetor 4x1 de coordenadas
  void SetCoordenadas(float x, float y, float z); // metodo para determinar as coordenadas x, y e z
  void MultiplicadoPorMatriz(float mat[4][4]);    // metodo utilizado para reposicionamento dos pontos utilizando matriz
};

void Ponto3d::AtualizaCoordenadas()
{
  coordenadas[0] = x;
  coordenadas[1] = y;
  coordenadas[2] = z;
  coordenadas[3] = 1;
}

void Ponto3d::SetCoordenadas(float x, float y, float z)
{
  this->x = x;
  this->y = y;
  this->z = z;
  AtualizaCoordenadas();
}

void Ponto3d::operator = (Ponto2d p){
    SetCoordenadas(p.x, p.y, 0);
}

void Ponto3d::MultiplicadoPorMatriz(float mat[4][4])
{
  float res[4];
  for (int i = 0; i < 4; i++)
  {
    res[i] = 0;
    for (int k = 0; k < 4; k++)
    {
      res[i] += mat[i][k] * coordenadas[k];
    }
  }
  if(res[3]!= 0){
      x = (res[0]/res[3]);
      y = (res[1]/res[3]);
      z = (res[2]/res[3]);
  }else{

  }
  AtualizaCoordenadas();
}

class Matriz3d
{
public:
  float pontos[4][4]; // a matriz em si, atualizada a cada metodo chamado
  Matriz3d();         // construtor, sempre inicia a matriz como matriz identidade

  void CopiaMatriz(float mat[4][4]);                              // iguala elemento por elemento da matriz da classe com a matriz passada como argumento
  void MultiplicaMatriz(float mat[4][4]);                         // multiplica a matriz passada como argumento pela matriz da classe, na ordem (classe X argumento), visto que a ordem altera o resultado
  void MultiplicadoPorMatriz(float mat[4][4]);                    // multiplica a matriz da classe pela matriz passada como argumento, na ordem (argumento X classe), visto que a ordem altera o resultado
  void MatrizRotacaoEixoX(float rad);                             // multiplica a matriz da classe por uma matriz de rotacao que utiliza o angulo rad (em radianos)
  void MatrizRotacaoEixoY(float rad);                             //
  void MatrizRotacaoEixoZ(float rad);                             //
  void MatrizTranslacao(float x, float y, float z);               // multiplica a matriz da classe por uma matriz de translacao
  void MatrizEscala(float multiX, float multiY, float multiZ);    // multiplica a matriz da classe por uma matriz de escala
  void MatrizPerspectiva(float d);                                // multiplica a matriz da classe por uma matriz de perspectiva, considerando a distancia do plano em relacao a camera como d
  void LimpaMatriz();                                             // volta a matriz ao estado de matriz identidade
};

void Matriz3d::LimpaMatriz(){
  pontos[0][0] = 1;  pontos[0][1] = 0;  pontos[0][2] = 0;  pontos[0][3] = 0;
  pontos[1][0] = 0;  pontos[1][1] = 1;  pontos[1][2] = 0;  pontos[1][3] = 0;
  pontos[2][0] = 0;  pontos[2][1] = 0;  pontos[2][2] = 1;  pontos[2][3] = 0;
  pontos[3][0] = 0;  pontos[3][1] = 0;  pontos[3][2] = 0;  pontos[3][3] = 1;
}

Matriz3d::Matriz3d()
{
  LimpaMatriz();
}

void Matriz3d::CopiaMatriz(float mat[4][4])
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      pontos[i][j] = mat[i][j];
    }
  }
}

void Matriz3d::MultiplicaMatriz(float mat[4][4])
{
  float res[4][4];
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      res[i][j] = 0;
      for (int k = 0; k < 4; k++)
      {
        res[i][j] += pontos[i][k] * mat[k][j];
      }
    }
  }
  CopiaMatriz(res);
}

void Matriz3d::MultiplicadoPorMatriz(float mat[4][4])
{
  float res[4][4];
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      res[i][j] = 0;
      for (int k = 0; k < 4; k++)
      {
        res[i][j] += mat[i][k] * pontos[k][j];
      }
    }
  }
  CopiaMatriz(res);
}

void Matriz3d::MatrizRotacaoEixoX(float rad)
{
  float rot[4][4];
  rot[0][0] = 1;  rot[0][1] =        0;  rot[0][2] =         0;  rot[0][3] = 0;
  rot[1][0] = 0;  rot[1][1] = cos(rad);  rot[1][2] = -sin(rad);  rot[1][3] = 0;
  rot[2][0] = 0;  rot[2][1] = sin(rad);  rot[2][2] =  cos(rad);  rot[2][3] = 0;
  rot[3][0] = 0;  rot[3][1] =        0;  rot[3][2] =         0;  rot[3][3] = 1;

  MultiplicadoPorMatriz(rot);
}

void Matriz3d::MatrizRotacaoEixoY(float rad)
{
  float rot[4][4];
  rot[0][0] = cos(rad);  rot[0][1] = 0;  rot[0][2] = -sin(rad);  rot[0][3] = 0;
  rot[1][0] =        0;  rot[1][1] = 1;  rot[1][2] =         0;  rot[1][3] = 0;
  rot[2][0] = sin(rad);  rot[2][1] = 0;  rot[2][2] =  cos(rad);  rot[2][3] = 0;
  rot[3][0] =        0;  rot[3][1] = 0;  rot[3][2] =         0;  rot[3][3] = 1;

  MultiplicadoPorMatriz(rot);
}

void Matriz3d::MatrizRotacaoEixoZ(float rad)
{
  float rot[4][4];
  rot[0][0] = cos(rad);  rot[0][1] = -sin(rad);  rot[0][2] = 0;  rot[0][3] = 0;
  rot[1][0] = sin(rad);  rot[1][1] =  cos(rad);  rot[1][2] = 0;  rot[1][3] = 0;
  rot[2][0] =        0;  rot[2][1] =         0;  rot[2][2] = 1;  rot[2][3] = 0;
  rot[3][0] =        0;  rot[3][1] =         0;  rot[3][2] = 0;  rot[3][3] = 1;

  MultiplicadoPorMatriz(rot);
}

void Matriz3d::MatrizTranslacao(float x, float y, float z)
{
  float trans[4][4];
  trans[0][0] = 1;  trans[0][1] = 0;  trans[0][2] = 0;  trans[0][3] = x;
  trans[1][0] = 0;  trans[1][1] = 1;  trans[1][2] = 0;  trans[1][3] = y;
  trans[2][0] = 0;  trans[2][1] = 0;  trans[2][2] = 1;  trans[2][3] = z;
  trans[3][0] = 0;  trans[3][1] = 0;  trans[3][2] = 0;  trans[3][3] = 1;

  MultiplicadoPorMatriz(trans);
}

void Matriz3d::MatrizEscala(float multiX, float multiY, float multiZ)
{
  float escala[4][4];
  escala[0][0] = multiX;  escala[0][1] =      0;  escala[0][2] =      0;  escala[0][3] = 0;
  escala[1][0] =      0;  escala[1][1] = multiY;  escala[1][2] =      0;  escala[1][3] = 0;
  escala[2][0] =      0;  escala[2][1] =      0;  escala[2][2] = multiZ;  escala[2][3] = 0;
  escala[3][0] =      0;  escala[3][1] =      0;  escala[3][2] =      1;  escala[3][3] = 1;

  MultiplicadoPorMatriz(escala);
}

void Matriz3d::MatrizPerspectiva(float d){
  float perspectiva[4][4];
  perspectiva[0][0] = 1;perspectiva[0][1] = 0;perspectiva[0][2] =          0;perspectiva[0][3] = 0;
  perspectiva[1][0] = 0;perspectiva[1][1] = 1;perspectiva[1][2] =          0;perspectiva[1][3] = 0;
  perspectiva[2][0] = 0;perspectiva[2][1] = 0;perspectiva[2][2] =          0;perspectiva[2][3] = 0;
  perspectiva[3][0] = 0;perspectiva[3][1] = 0;perspectiva[3][2] = (float)1/d;perspectiva[3][3] = 0;

  MultiplicadoPorMatriz(perspectiva);
}

#endif

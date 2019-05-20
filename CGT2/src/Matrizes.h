/*
  Autor: Paulo Saicoski Sarkis
         05/2019

  Matricula: 201612057

*/

//  Descricao:
//    Este TAD eh o responsavel por todas as criacoes e interacoes entre matrizes ou entre ponto e matriz




#ifndef __MATRIZES_H__
#define __MATRIZES_H__

#include <math.h>



class ponto
{

private:
  float coordenadas[3];  // vetor 3x1 com as coordenadas

public:
  float x;  //valores de facil acesso para x e y
  float y;

  ponto(float x, float y)
  {
    SetCoordenadas(x, y);
  }
  ponto()
  {
    SetCoordenadas(0, 0);
  }

  void AtualizaCoordenadas();                   // unico metodo que realmente altera o vetor 3x1 de coordenadas
  void SetCoordenadas(float x, float y);        // metodo para determinar as coordenadas x e y
  void MultiplicadoPorMatriz(float mat[3][3]);  // metodo utilizado para reposicionamento dos pontos utilizando matriz
};

void ponto::AtualizaCoordenadas()
{
  coordenadas[0] = x;
  coordenadas[1] = y;
  coordenadas[2] = 1;
}

void ponto::SetCoordenadas(float x, float y)
{
  this->x = x;
  this->y = y;
  AtualizaCoordenadas();
}

void ponto::MultiplicadoPorMatriz(float mat[3][3])
{
  float res[3];
  for (int i = 0; i < 3; i++)
  {
    res[i] = 0;
    for (int k = 0; k < 3; k++)
    {
      res[i] += mat[i][k] * coordenadas[k];
    }
  }
  x = res[0];
  y = res[1];
  AtualizaCoordenadas();
}

class matriz
{
public:
  float pontos[3][3]; // a matriz em si, atualizada a cada metodo chamado
  matriz();           // construtor, sempre inicia a matriz como matriz identidade

  void CopiaMatriz(float mat[3][3]);                              // iguala elemento por elemento da matriz da classe com a matriz passada como argumento
  void MultiplicaMatriz(float mat[3][3]);                         // multiplica a matriz passada como argumento pela matriz da classe, na ordem (classe X argumento), visto que a ordem altera o resultado
  void MultiplicadoPorMatriz(float mat[3][3]);                    // multiplica a matriz da classe pela matriz passada como argumento, na ordem (argumento X classe), visto que a ordem altera o resultado
  void MatrizRotacao(float rad);                                  // multiplica a matriz da classe por uma matriz de rotacao que utiliza o angulo rad (em radianos)
  void MatrizTranslacao(float x, float y);                        // multiplica a matriz da classe por uma matriz de translacao
  void MatrizEscala(float multiX, float multiY);                  // multiplica a matriz da classe por uma matriz de escala
  void MatrizRotaEmEixo(float rad, float centroX, float centroY); // utiliza os metodos MatrizTranslacao e MatrizRotacao para girar o ponto multiplicado pela matriz em volta do ponto (centroX, centroY)
};

matriz::matriz()
{
  pontos[0][0] = 1;  pontos[0][1] = 0;  pontos[0][2] = 0;
  pontos[1][0] = 0;  pontos[1][1] = 1;  pontos[1][2] = 0;
  pontos[2][0] = 0;  pontos[2][1] = 0;  pontos[2][2] = 1;
}


void matriz::CopiaMatriz(float mat[3][3])
{
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      pontos[i][j] = mat[i][j];
    }
  }
}

void matriz::MultiplicaMatriz(float mat[3][3])
{
  float res[3][3];
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      res[i][j] = 0;
      for (int k = 0; k < 3; k++)
      {
        res[i][j] += pontos[i][k] * mat[k][j];
      }
    }
  }
  CopiaMatriz(res);
}

void matriz::MultiplicadoPorMatriz(float mat[3][3])
{
  float res[3][3];
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      res[i][j] = 0;
      for (int k = 0; k < 3; k++)
      {
        res[i][j] += mat[i][k] * pontos[k][j];
      }
    }
  }
  CopiaMatriz(res);
}

void matriz::MatrizRotacao(float rad)
{
  float rot[3][3];
  rot[0][0] = cos(rad);  rot[0][1] = -sin(rad);  rot[0][2] = 0;
  rot[1][0] = sin(rad);  rot[1][1] =  cos(rad);  rot[1][2] = 0;
  rot[2][0] =        0;  rot[2][1] =         0;  rot[2][2] = 1;

  MultiplicadoPorMatriz(rot);
}

void matriz::MatrizTranslacao(float x, float y)
{
  float trans[3][3];
  trans[0][0] = 1;  trans[0][1] = 0;  trans[0][2] = x;
  trans[1][0] = 0;  trans[1][1] = 1;  trans[1][2] = y;
  trans[2][0] = 0;  trans[2][1] = 0;  trans[2][2] = 1;

  MultiplicadoPorMatriz(trans);
}

void matriz::MatrizEscala(float multiX, float multiY)
{
  float escala[3][3];
  escala[0][0] = multiX;  escala[0][1] =      0;  escala[0][2] = 0;
  escala[1][0] =      0;  escala[1][1] = multiY;  escala[1][2] = 0;
  escala[2][0] =      0;  escala[2][1] =      0;  escala[2][2] = 1;

  MultiplicadoPorMatriz(escala);
}

void matriz::MatrizRotaEmEixo(float rad, float centroX, float centroY)
{
  MatrizTranslacao(-centroX, -centroY);
  MatrizRotacao(rad);
  MatrizTranslacao(centroX, centroY);
}

#endif

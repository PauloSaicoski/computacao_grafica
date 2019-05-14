#ifndef __MATRIZES_H__
#define __MATRIZES_H__


#include <math.h>

class ponto{

private:
  float coordenadas[3];

public:
  float x;
  float y;

  ponto(float x, float y){
    SetCoordenadas(x,y);
  }
  ponto(){
    SetCoordenadas(0,0);
  }

  void AtualizaCoordenadas();
  void SetCoordenadas(float x, float y);
  void MultiplicadoPorMatriz(float mat[3][3]);
};

void ponto::AtualizaCoordenadas(){
  coordenadas[0] = x;
  coordenadas[1] = y;
  coordenadas[2] = 1;
}

void ponto::SetCoordenadas(float x, float y){
  this->x = x;
  this->y = y;
  AtualizaCoordenadas();
}

void ponto::MultiplicadoPorMatriz(float mat[3][3]){
  float res[3];
  for (int i = 0; i < 3; i++){
      res[i] = 0;
      for (int k = 0; k < 3; k++){
        res[i] += mat[i][k] * coordenadas[k];
      }
  }
  x = res[0];
  y = res[1];
  AtualizaCoordenadas();

}

class matriz{
  public:
  float pontos[3][3];

  matriz();
  matriz(float x, float y);

  void CopiaMatriz(float mat[3][3]);
  void MultiplicaMatriz(float mat[3][3]);
  void MultiplicadoPorMatriz(float mat[3][3]);
  void MatrizRotacao(float rad);
  void MatrizTranslacao(float x, float y);
  void MatrizEscala(float multiX, float multiY);
  void MatrizRotaEmEixo(float rad, float centroX, float centroY);

};

matriz::matriz(){
  pontos[0][0] = 1; pontos[0][1]= 0; pontos[0][2]= 0;
  pontos[1][0] = 0; pontos[1][1]= 1; pontos[1][2]= 0;
  pontos[2][0] = 0; pontos[2][1]= 0; pontos[2][2]= 1;
}

matriz::matriz(float x, float y){
  pontos[0][0] = x; pontos[0][1]= 0; pontos[0][2]= 0;
  pontos[1][0] = 0; pontos[1][1]= y; pontos[1][2]= 0;
  pontos[2][0] = 0; pontos[2][1]= 0; pontos[2][2]= 1;
}

void matriz::CopiaMatriz(float mat[3][3]){
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      pontos[i][j] = mat[i][j];
    }
  }
}

void matriz::MultiplicaMatriz(float mat[3][3]){
  float res[3][3];
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 3; j++){
      res[i][j] = 0;
      for (int k = 0; k < 3; k++){
        res[i][j] += pontos[i][k] * mat[k][j];
      }
    }
  }
  CopiaMatriz(res);
}

void matriz::MultiplicadoPorMatriz(float mat[3][3]){
  float res[3][3];
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 3; j++){
      res[i][j] = 0;
      for (int k = 0; k < 3; k++){
        res[i][j] += mat[i][k] * pontos[k][j];
      }
    }
  }
  CopiaMatriz(res);
}

void matriz::MatrizRotacao(float rad){
  float rot[3][3];
  rot[0][0] = cos(rad); rot[0][1]= -sin(rad); rot[0][2]= 0;
  rot[1][0] = sin(rad); rot[1][1]=  cos(rad); rot[1][2]= 0;
  rot[2][0] =        0; rot[2][1]=         0; rot[2][2]= 1;

  MultiplicadoPorMatriz(rot);
}

void matriz::MatrizTranslacao(float x, float y){
  float trans[3][3];
  trans[0][0] = 1; trans[0][1]= 0; trans[0][2]= x;
  trans[1][0] = 0; trans[1][1]= 1; trans[1][2]= y;
  trans[2][0] = 0; trans[2][1]= 0; trans[2][2]= 1;

  MultiplicadoPorMatriz(trans);
}

void matriz::MatrizEscala(float multiX, float multiY){
  float escala[3][3];
  escala[0][0] = multiX; escala[0][1]=       0; escala[0][2]= 0;
  escala[1][0] =      0; escala[1][1]=  multiY; escala[1][2]= 0;
  escala[2][0] =      0; escala[2][1]=       0; escala[2][2]= 1;

  MultiplicadoPorMatriz(escala);
}

void matriz::MatrizRotaEmEixo(float rad, float centroX, float centroY){
  MatrizTranslacao(-centroX, -centroY);
  MatrizRotacao(rad);
  MatrizTranslacao(centroX, centroY);
}



#endif

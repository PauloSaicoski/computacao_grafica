#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>


using namespace std;

int qntSolucoes = 0;

void printaMatriz(vector<int> vetor, int tam){
    for(int i = 0; i< tam; i++){
        for(int j = 0; j<tam; j++){
            std::cout << vetor[i*tam+j] << " ";
        }
        std::cout << std::endl;
    }
}

int somaLinhaMatriz(vector<int> matriz, int tam, int linha){
    int soma = 0;
    for(int i = 0; i < tam; i++){
        soma+= matriz[linha*tam+i];
    }
    return soma;
}

int somaColunaMatriz(vector<int> matriz, int tam, int coluna){
    int soma = 0;
    for(int i = 0; i < tam; i++){
        soma+= matriz[i*tam+coluna];
    }
    return soma;
}

int somaDiagonalMatriz(vector<int> matriz, int tam, bool principal){
    int soma = 0;
    if(principal){
        for(int i = 0; i < tam; i++){
            soma+= matriz[i*tam+i];
        }
    }else{
        for(int i = 0; i < tam; i++){
            soma+= matriz[i*tam+(tam-i)];
        }
    }

    return soma;
}


void confereQuadrado(vector<int> matriz, int tam){
    int somaAtual = 0;
    int somaAnterior = 0;
    for(int i = 0; i < tam; i++){
        somaAtual = somaLinhaMatriz(matriz, tam, i);
        if (somaAnterior !=0 && somaAtual != somaAnterior){
            return;
        }
        somaAnterior = somaAtual;
    }
    for(int i = 0; i < tam; i++){
        somaAtual = somaColunaMatriz(matriz, tam, i);
        if (somaAtual != somaAnterior){
            return;
        }
        somaAnterior = somaAtual;
    }
    if(somaAnterior != somaDiagonalMatriz(matriz, tam, 0) || somaAnterior != somaDiagonalMatriz(matriz, tam, 1)){
        return;
    }
    qntSolucoes++;
}

void procuraQuadMagicos(vector<int> restantes, int tam, vector<int> vecAtual, int remover){
    if(remover>=0){
        vecAtual.push_back(restantes[remover]);
        restantes.erase(restantes.begin()+remover);
    }
    if(restantes.size() <= 0){
        confereQuadrado(vecAtual, tam);
    }
    else{
        for(int i = 0; i < restantes.size(); i++){
            procuraQuadMagicos(restantes, tam, vecAtual, i);
        }
    }

}

int main (){
    int qnt;
    printf("Digite o tamanho da matriz:\n");
    cin >> qnt;
    vector<int> vetor, aux;
    for(int i = 0; i< qnt*qnt; i++){
        vetor.push_back(i+1);
    }
    time_t tIni, tFim;
    time(&tIni);
    procuraQuadMagicos(vetor, qnt, aux, -1);
    time(&tFim);
    double tTotal = difftime(tFim, tIni);

    cout << "Quantia de solucoes: "<< qntSolucoes << " em " << tTotal<< " segundos" << endl;


    return 0;
}

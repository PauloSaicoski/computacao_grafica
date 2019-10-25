//  Nome: Paulo Saicoski Sarkis
//  Matricula: 201612057

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>

using namespace std;

int qntSolucoes = 0;

void printaMatriz(vector<int> vetor, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        for (int j = 0; j < tam; j++)
        {
            std::cout << vetor[i * tam + j] << " ";
        }
        std::cout << std::endl;
    }
}

int somaLinhaMatriz(vector<int> matriz, int tam, int linha)
{
    int soma = 0;
    for (int i = 0; i < tam; i++)
    {
        soma += matriz[linha * tam + i];
    }
    return soma;
}

int somaColunaMatriz(vector<int> matriz, int tam, int coluna)
{
    int soma = 0;
    for (int i = 0; i < tam; i++)
    {
        soma += matriz[i * tam + coluna];
    }
    return soma;
}

int somaDiagonalMatriz(vector<int> matriz, int tam, bool principal)
{
    int soma = 0;
    if (principal)
    {
        for (int i = 0; i < tam; i++)
        {
            soma += matriz[i * tam + i];
        }
    }
    else
    {
        for (int i = 0; i < tam; i++)
        {
            soma += matriz[i * tam + (tam - i - 1)];
        }
    }
    return soma;
}

void confereQuadrado(vector<int> matriz, int tam, bool printar)
{
    int somaAtual = 0;
    int somaAnterior = 0;
    for (int i = 0; i < tam; i++)
    {
        somaAtual = somaLinhaMatriz(matriz, tam, i);
        if (somaAnterior != 0 && somaAtual != somaAnterior)
        {
            return;
        }
        somaAnterior = somaAtual;
    }
    for (int i = 0; i < tam; i++)
    {
        somaAtual = somaColunaMatriz(matriz, tam, i);
        if (somaAtual != somaAnterior)
        {
            return;
        }
        somaAnterior = somaAtual;
    }
    int somaDiagonalPrincipal = somaDiagonalMatriz(matriz, tam, true);
    int somaDiagonalSegundaria = somaDiagonalMatriz(matriz, tam, false);
    if (somaAnterior != somaDiagonalSegundaria || somaAnterior != somaDiagonalPrincipal)
    {
        return;
    }
    qntSolucoes++;
    if (printar)
    {
        printf("Solucao %d\n", qntSolucoes);
        printaMatriz(matriz, tam);
    }
}

void procuraQuadMagicos(vector<int> restantes, int tam, vector<int> vecAtual, int remover, bool printar)
{
    if (remover >= 0) //remover eh o indice do numero a ser adicionado a matriz
    {
        vecAtual.push_back(restantes[remover]);       //adiciona o numero no vetor que servira de matriz
        restantes.erase(restantes.begin() + remover); //remove o numero do vetor de numeros ainda nao utilizados
    }
    if (restantes.size() <= 0)
    { //confere se a matriz eh um quadrado magico quando todos os numeros foram inseridos
        confereQuadrado(vecAtual, tam, printar);
    }
    else
    {
        for (int i = 0; i < restantes.size(); i++)
        { //executa uma recurcao para cada numero possivel de se adicionar no proximo campo da matriz
            procuraQuadMagicos(restantes, tam, vecAtual, i, printar);
        }
    }
}

int main()
{
    int qnt;
    bool printar;
    printf("Digite o tamanho da matriz (numero positivo e maior que zero):\n");
    cin >> qnt;
    if (qnt <= 0)
    {
        printf("Tamanho invalido\n");
        return 0;
    }
    printf("Voce deseja ver as solucoes? (interfere no tempo de execucao)\n(1 = sim, 0 = nao)\n");
    cin >> printar;
    vector<int> vetor, aux;
    for (int i = 0; i < qnt * qnt; i++)
    {
        vetor.push_back(i + 1);
    }
    clock_t tIni, tFim;
    tIni = clock();

    procuraQuadMagicos(vetor, qnt, aux, -1, printar);

    tFim = clock();
    double tTotal = (double)(tFim - tIni) / (double)CLOCKS_PER_SEC;

    cout << "Quantia de solucoes: " << qntSolucoes << " em " << tTotal << " segundos" << endl;

    return 0;
}

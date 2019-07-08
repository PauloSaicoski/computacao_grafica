#ifndef __CARRO_H__
#define __CARRO_H__

#include <GL/glut.h>
#include "Ponto3D.h"
#include <math.h>
#define M_PI 3.14159265358979323846

//  Descricao:
//    Este TAD eh o responsavel pela classe utilizada para desenhar o carro e seus elementos


class Carro
{
private:
    double velocidade;
    double desaceleracao;
    double aceleracao;

public:
    double larguraX;
    double alturaY;
    double comprimentoZ;
    double rotacaoEixoY;
    double rotacaoEixoX;
    double velMax;
    bool cockpit;
    bool vetorDir;
    Ponto3D pos;

    Carro();
    Carro(double larguraX, double alturaY, double comprimentoZ, Ponto3D pos);
    void setTam(double larguraX, double alturaY, double comprimentoZ);
    void setRotacaoY(double rotacaoEixoY);
    void setRotacaoX(double rotacaoEixoX);
    double getVelocidade();
    void acelera(double aceleracao);
    void desacelera();
    void atualiza();
    void desenha();
};

void Carro::setTam(double larguraX, double alturaY, double comprimentoZ)
{
    this->larguraX = larguraX;
    this->alturaY = alturaY;
    this->comprimentoZ = comprimentoZ;
}

Carro::Carro()
{
    setTam(1, 1, 1);
    velocidade = 0;
    rotacaoEixoY = 0;
    rotacaoEixoX = 0;
    velMax = 1;
    desaceleracao = 0.005;
    aceleracao = 0;
    cockpit = false;
}

Carro::Carro(double larguraX, double alturaY, double comprimentoZ, Ponto3D pos)
{
    setTam(larguraX, alturaY, comprimentoZ);
    velocidade = 0;
    rotacaoEixoY = 0;
    rotacaoEixoX = 0;
    velMax = 1;
    desaceleracao = 0.005;
    aceleracao = 0;
    cockpit = false;
    this->pos = pos;
}

void Carro::setRotacaoY(double rotacaoEixoY)
{
    this->rotacaoEixoY = rotacaoEixoY;
}

void Carro::setRotacaoX(double rotacaoEixoX)
{
    this->rotacaoEixoX = rotacaoEixoX;
}

double Carro::getVelocidade()
{
    return velocidade;
}

void Carro::acelera(double aceleracao)
{
    this->aceleracao = aceleracao;
}

void Carro::desacelera()
{
    aceleracao = 0;
}

void Carro::atualiza()
{
    pos.z += velocidade * cos((rotacaoEixoY * M_PI / 180));
    pos.x += velocidade * sin((rotacaoEixoY * M_PI / 180));

    if (velocidade < velMax)
    {
        velocidade += aceleracao;
    }
    if (-velocidade > velMax / 2)
    {
        velocidade = -velMax / 2;
    }

    if (velocidade > desaceleracao * 0.5)
    {
        velocidade -= desaceleracao;
    }
    else if (-velocidade > desaceleracao * 0.5)
    {
        velocidade += desaceleracao;
    }
    else
    {
        velocidade = 0;
    }
}

void Carro::desenha()
{
    double comprimentoMenor = comprimentoZ / 3;
    double comprimentoMaior = comprimentoZ - comprimentoMenor;
    if (!cockpit)
    {
        glPushMatrix();
        glColor3f(0, 0, 1);
        glTranslated(pos.x, pos.y, pos.z);
        glRotated(rotacaoEixoY, 0, 1, 0);
        glRotated(rotacaoEixoX, 1, 0, 0);
        glTranslated(0, 0, -comprimentoMaior / 2);
        glScaled(larguraX, alturaY, comprimentoMaior);
        glutSolidCube(1);
        glPopMatrix();
    }
    glPushMatrix();
    {
        glColor3f(1, 0, 0);
        glTranslated(pos.x, pos.y, pos.z);
        glRotated(rotacaoEixoY, 0, 1, 0);
        glRotated(rotacaoEixoX, 1, 0, 0);
        glTranslated(0, -alturaY / 4, comprimentoMenor / 2);
        glScaled(larguraX, alturaY / 2, comprimentoMenor);
        glutSolidCube(1);
    }
    glPopMatrix();
    if (vetorDir)
    {
        glPushMatrix();
        {
            glBegin(GL_LINES);
            {
                glColor3f(0, 1, 0);
                glVertex3d(pos.x, pos.y, pos.z);
                glVertex3d(pos.x + (sin(rotacaoEixoY * M_PI / 180) * comprimentoZ), pos.y, pos.z + (cos((rotacaoEixoY * M_PI / 180)) * comprimentoZ));
            }
            glEnd();
        }
        glPopMatrix();
    }
}

#endif

/*
  Autor: Paulo Saicoski Sarkis
         07/2019

  Matricula: 201612057

*/

#include <GL/glut.h>
#include "Carro.h"
#include "Ponto3D.h"
#include "curvas.h"
//#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <ctime>

#define M_PI 3.14159265358979323846

#define SCREEN_X 1000
#define SCREEN_Y 800

double frameTime = 0, executionTime = 0, previousTime = 0, acumuladoTime = 0;

int polygonMode = 1;
float rx = 0, rz = 0;

float abertura = 60.0;
float znear = 1;
float zfar = 500;
float aspect = 1;

Carro *carro = nullptr;
Ponto3D posCarro(20,0,20);
double rcx = 0, rcy = 0;
double distanciaCam = 100;
double alturaCam = 30;
double larguraPista = 22;
bool cockpit, superior;
bool vetorDir = false;
std::vector<Ponto3D> estradaPontosControle;
std::vector<Ponto3D> bspline, estrada;
GLfloat posicaoLuz[4] = {0.0, 0.0, 0.0, 1.0};
GLfloat luzAmbiente[4] = {0.15, 0.15, 0.15, 1.0};
GLfloat luzDifusa[4] = {3, 3, 3, 3.0};
GLfloat luzEspecular[4] = {1.5, 1.5, 1.5, 1.5};
GLfloat especularidade[4] = {1.0, 1.0, 1.0, 1.0};

double getAngulo2D(Ponto3D A, Ponto3D B)
{
    return atan2(B.z - A.z, B.x - A.x);
}

std::vector<Ponto3D> getPista()
{
    double angle;
    std::vector<Ponto3D> vetor;
    for (int i = 0; i < bspline.size(); i++)
    {
        angle = getAngulo2D(bspline[i], bspline[(i + 1) % bspline.size()]);
        vetor.push_back(Ponto3D(bspline[i].x + cos(angle + (M_PI / 2)) * larguraPista, bspline[i].y, bspline[i].z + sin(angle + (M_PI / 2)) * larguraPista));
        vetor.push_back(Ponto3D(bspline[i].x + cos(angle - (M_PI / 2)) * larguraPista, bspline[i].y, bspline[i].z + sin(angle - (M_PI / 2)) * larguraPista));
    }
    return vetor;
}

void init()
{

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(abertura, aspect, znear, zfar);
    glMatrixMode(GL_MODELVIEW);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //glEnable(GL_DEPTH_TEST);

    // Capacidade de brilho do material
    GLint especMaterial = 100;

    // Especifica que a cor de fundo da janela ser� preta
    glClearColor(0.7f, 0.7f, 1.0f, 1.0f);

    // Habilita o modelo de coloriza��o de Gouraud
    glShadeModel(GL_SMOOTH);

    // Define a reflet�ncia do material
    glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);

    // Define a concentra��o do brilho
    glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

    // Define os par�metros da luz de n�mero 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);

    // Habilita a defini��o da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    //Habilita o uso de ilumina��o
    glEnable(GL_LIGHTING);
    // Habilita a luz de n�mero 0
    glEnable(GL_LIGHT0);

    // Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
}

////////////////////////////////////////////////////////////////////////////////////////
void display(void)
{

    executionTime = clock() / CLOCKS_PER_SEC;
    frameTime = executionTime - previousTime;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //printf("\n%f", abertura);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-1,1,-1,1,1,-1);

    gluPerspective(abertura, aspect, znear, zfar);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    carro->cockpit = cockpit;
    carro->vetorDir = vetorDir;
    if (cockpit)
    {
        gluLookAt(carro->pos.x, carro->pos.y + 1, carro->pos.z,                                                                                                               //from. Posicao onde a camera esta posicionada
                  carro->pos.x + distanciaCam * sin((carro->rotacaoEixoY * M_PI / 180)), carro->pos.y, carro->pos.z + distanciaCam * cos((carro->rotacaoEixoY * M_PI / 180)), //to. Posicao absoluta onde a camera esta vendo
                  0, 1, 0);                                                                                                                                                   //up. Vetor Up.
    }
    else if (superior)
    {
        gluLookAt(carro->pos.x, carro->pos.y + alturaCam * 5, carro->pos.z,                         //from. Posicao onde a camera esta posicionada
                  carro->pos.x, carro->pos.y, carro->pos.z,                                         //to. Posicao absoluta onde a camera esta vendo
                  sin(carro->rotacaoEixoY * M_PI / 180), 0, cos(carro->rotacaoEixoY * M_PI / 180)); //up. Vetor Up.
    }
    else
    {
        gluLookAt(carro->pos.x + distanciaCam * sin((carro->rotacaoEixoY * M_PI / 180) + M_PI), carro->pos.y + alturaCam, carro->pos.z + distanciaCam * cos((carro->rotacaoEixoY * M_PI / 180) + M_PI), //from. Posicao onde a camera esta posicionada
                  carro->pos.x, carro->pos.y, carro->pos.z,                                                                                                                                             //to. Posicao absoluta onde a camera esta vendo
                  0, 1, 0);                                                                                                                                                                             //up. Vetor Up.
    }

    glPushMatrix();
    glTranslated(carro->pos.x + (sin(carro->rotacaoEixoY * M_PI / 180) * carro->comprimentoZ / 3), carro->pos.y, carro->pos.z + (cos((carro->rotacaoEixoY * M_PI / 180)) * carro->comprimentoZ / 3));
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);
    glPopMatrix();

    //todos os objetos estao definidos na origem do sistema global fixo
    //e sao transladados para a posicao destino.
    carro->atualiza();
    carro->desenha();
    glPushMatrix();
    glColor3d(0.8, 0.8, 0.8);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < estrada.size(); i++)
    {
        glNormal3d(0, 1, 0);
        glVertex3d(estrada[i].x, estrada[i].y, estrada[i].z);
    }
    glNormal3d(0, 1, 0);
    glVertex3d(estrada[0].x, estrada[0].y, estrada[0].z);
    glNormal3d(0, 1, 0);
    glVertex3d(estrada[1].x, estrada[1].y, estrada[1].z);
    glEnd();
    glPopMatrix();

    //bule
    glColor3f(0, 0, 1);
    glPushMatrix();
    glutSolidTeapot(5);
    glPopMatrix();

    glutSwapBuffers();
}

//faz a leitura da entrada do usuario
void keyboard(unsigned char key, int x, int y)
{
    //printf("%c", key);
    key = tolower(key);
    switch (key)
    {
    case 27:
        exit(0);
        break;

    case 'w': //wireframe
        if (polygonMode == 1)
        {
            polygonMode = 0;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            polygonMode = 1;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        break;
    case '+':
        abertura += 1;
        break;
    case 'c':
        if (cockpit)
        {
            cockpit = false;
            superior = true;
        }
        else if (superior)
        {
            cockpit = false;
            superior = false;
        }
        else
        {
            cockpit = true;
            superior = false;
        }
        break;
    case 'a':
        vetorDir = !vetorDir;
        break;
    }
}
void keyboardSpecial(int key, int x, int y)
{
    switch (key)
    {
    case 101: //Frente
        carro->acelera(0.5);
        break;
    case 103: //Traz

        carro->acelera(-0.5);
        break;
    case 100: //Esquerda
        rcy += 5;
        carro->setRotacaoY(rcy);
        break;
    case 102: //Direita
        rcy -= 5;
        carro->setRotacaoY(rcy);
        break;
    }
}

void keyboardSpecialUp(int key, int x, int y)
{
    switch (key)
    {
    case 101: //Frente
        carro->desacelera();
        break;
    case 103: //Traz

        carro->desacelera();
        break;
        /*
        case 100: //Esquerda
            rcy += 5;
            carro->setRotacaoY(rcy);
            break;
        case 102: //Direita
            rcy -= 5;
            carro->setRotacaoY(rcy);
            break;*/
    }
}

void MotionFunc(int x, int y)
{
    //   float dx, dy;
    //printf("\nX = %d Y = %d", x, y);
}

void MouseFunc(int botao, int estado, int x, int y)
{
    //printf("\n%d %d %d %d", botao, estado, x, y);
}

////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    int argc = 0;
    double tamX = 8, tamY = 7, tamZ = 14;
    posCarro.y += tamY / 2;
    carro = new Carro(8, 7, 14, posCarro);
    estradaPontosControle.push_back(Ponto3D(0, 0, 0));
    estradaPontosControle.push_back(Ponto3D(0, 0, 40));
    estradaPontosControle.push_back(Ponto3D(500, 0, 700));
    estradaPontosControle.push_back(Ponto3D(1000, 0, 300));
    estradaPontosControle.push_back(Ponto3D(1300, 0, 100));
    estradaPontosControle.push_back(Ponto3D(600, 0, -50));

    bspline = getBSplineList(estradaPontosControle, 49);
    estrada = getPista();

    //char *argv[1] = {"teste"};
    glutInit(&argc, NULL);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(SCREEN_X, SCREEN_Y);
    glutInitWindowPosition(450, 10);
    /* create window */
    glutCreateWindow("GluLookAt Demo");
    init();
    printf("Digite W para mudar o modo de visualizacao: wireframe/fill\n");
    printf("Digite C para mudar a posicao da camera: traseira/cockpit/superior\n");
    printf("Digite A para ativar ou desativar o vetor direcao do carro\n");

    glutDisplayFunc(display);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseFunc);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    glutSpecialUpFunc(keyboardSpecialUp);

    glutMainLoop();
    return 0;
}

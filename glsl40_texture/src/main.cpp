/*
	GLSL 4.x demo
	Mostra o uso de:
	-GLM - Math library
	-VBO & VAO
	-GL error detection
	-glfwGetKeyOnce

	August 2015 - Tiago Augusto Engel - tengel@inf.ufsm.br
*/


//Include GLEW - always first 
#include "GL/glew.h"
#include <GLFW/glfw3.h>

//Include the standard C++ headers 
#include "Scene.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include "Plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Jogador.h"
#include "lab_framebuffer.hpp"

#define WINDOW_WIDTH	1000
#define WINDOW_HEIGHT	1000


Plane *chao, *tela;
lab::Framebuffer *fbuff, *fbfzoom;

Jogador* jogador;
GLFWwindow* window;
bool wireframe = false;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
float posX, posZ;
float miraFOV = 30;



void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	miraFOV -= yoffset;
	if (miraFOV < 10) {
		miraFOV = 10;
	}
	if (miraFOV > 50) {
		miraFOV = 50;
	}
}

void mainLoop()
{
	double thisTime;
	double lastTime = glfwGetTime();
	posX = 0.0f;
	posZ = 0.0f;
	glfwSetCursorPos(window, 0, 0);
	glfwSetScrollCallback(window, scroll_callback);
	do
	{
		
		// toggle wireframe
		if (glfwGetKeyOnce(window, 'Q')){
			wireframe = !wireframe;
			if (wireframe){
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		jogador->update();
		chao->setFire(jogador->fire, jogador->getPos(), jogador->getDir());
		viewMatrix = jogador->getViewMatrix();

		thisTime = glfwGetTime();
		//tela->setViewProjectionMatrix(viewMatrix, projectionMatrix);


		projectionMatrix = jogador->getProjectionMatrix(60.0f);
		chao->setViewProjectionMatrix(viewMatrix, projectionMatrix);
		
		fbuff->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		chao->update(thisTime - lastTime);
		chao->render();
		fbuff->unbind();
		
		projectionMatrix = jogador->getProjectionMatrix(miraFOV);
		chao->setViewProjectionMatrix(viewMatrix, projectionMatrix);
		
		fbfzoom->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		chao->update(thisTime - lastTime);
		chao->render();
		fbfzoom->unbind();

		tela->setZoom(jogador->zoom);
		tela->getBuffers(fbuff, fbfzoom);
		tela->update(thisTime - lastTime);
		tela->render();
		lastTime = thisTime;
		

		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();
	} while (!glfwWindowShouldClose(window));
}

// Define an error callback  
void errorCallback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}
// Key callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// Initialize GLFW  
void initGLFW()
{
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GLSL4.3 + GLM + VBO + VAO", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		system("pause");
		exit(EXIT_FAILURE);
	}
}
void initCallbacks()
{
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetErrorCallback(errorCallback);
}
void initGLEW()
{
	// Initialize GLEW
	glewExperimental = GL_TRUE; //ensures that all extensions with valid entry points will be exposed.
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		system("pause");
		exit(EXIT_FAILURE);
	}
}
void initializeGL()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}
// Close OpenGL window and terminate GLFW  
void closeApplication()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int main(void)
{
	initGLFW();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	initCallbacks();
	initGLEW();
	initializeGL();

	std::vector<vec3> *verticesChao, *verticesTela;
	/**/
	verticesChao = new std::vector<vec3>();
	verticesChao->push_back(vec3(-1, -1.0f, 1));
	verticesChao->push_back(vec3(-1, -1.0f, -1));
	verticesChao->push_back(vec3(1, -1.0f, -1));
	verticesChao->push_back(vec3(1, -1.0f, 1));
	/**/

	verticesTela = new std::vector<vec3>();
	verticesTela->push_back(vec3(-1, 1, 0));
	verticesTela->push_back(vec3(-1, -1, 0));
	verticesTela->push_back(vec3(1, -1, 0));
	verticesTela->push_back(vec3(1, 1, 0));

	chao = new Plane(window, verticesChao, false, 1);
	tela = new Plane(window, verticesTela, true, 1);
	fbuff = new lab::Framebuffer();
	fbuff->GenerateFBO(WINDOW_WIDTH, WINDOW_HEIGHT);
	fbfzoom = new lab::Framebuffer();
	fbfzoom->GenerateFBO(WINDOW_WIDTH, WINDOW_HEIGHT);
	
	chao->init();
	tela->init();
	jogador = new Jogador(window);
	std::cout << std::endl << "Q: wireframe" << std::endl;

	mainLoop();

	exit(EXIT_SUCCESS);
}
#pragma once

#include <GL/glew.h>

#include <GLFW\glfw3.h>
#include "Scene.h"
#include <vector>
#include "glslprogram.h"
#include "TextureManager.h"
#include <iostream>
#include "lab_framebuffer.hpp"

class Plane : public Scene {
public:
	Plane(GLFWwindow* window, std::vector<vec3> *vertices_, bool framebuffer, int size = 1);

	// mesh virtual functions
	void init();
	void update(double t);
	void setViewProjectionMatrix(glm::mat4 viewMatrix_, glm::mat4 projectionMatrix_);
	void render();
	void resize(int, int);
	void getBuffers(lab::Framebuffer* fbuff, lab::Framebuffer* fbfzoom);
	void setZoom(bool zoom);
	void setFire(bool fire, vec3 pos, vec3 dir);

private:
	void genPlane();
	void genBuffers();
	void loadTextures();

	GLuint vaoID;
	int size;
	bool framebuffer, zoom, fire;
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> texUV;
	std::vector<unsigned int> indices;

	GLSLProgram shader;
	GLFWwindow* window;

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat4 modelViewMatrix;

	lab::Framebuffer *fbuff, *fbfzoom;

	vec3 planePos;
	vec3 pos, dir;
};
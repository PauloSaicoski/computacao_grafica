#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#define M_PI   3.14159265358979323846264338327950288

char keyOnce[GLFW_KEY_LAST + 1];
#define glfwGetKeyOnce(WINDOW, KEY)             \
    (glfwGetKey(WINDOW, KEY) ?              \
     (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) :   \
     (keyOnce[KEY] = false))


class Jogador
{
private:
	vec3 pos;
	vec3 dir;
	GLFWwindow* window;

public:
	Jogador(GLFWwindow* window);
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix(float angulo);
	void update();
	bool zoom;
	bool fire;
	vec3 getPos();
	vec3 getDir();
};

Jogador::Jogador(GLFWwindow* window) {
	pos = vec3(0.0f, 0.5f, 0.0f);
	dir.x = 0;
	dir.y = 0;
	dir.z = -1;
	this->window = window;
	zoom = false;
}

glm::mat4 Jogador::getViewMatrix(){
	glm::mat4 viewMatrix = glm::lookAt(
		vec3(pos), //eye
		vec3(pos+dir), //center
		vec3(0.0f, 1.0f, 0.0f)); //up

	viewMatrix *= glm::scale(vec3(1.0f, 1.0f, -1.0f));
	return viewMatrix;
}

glm::mat4 Jogador::getProjectionMatrix(float angulo) {
	return glm::perspective(glm::radians(angulo), 1.0f, 0.1f, 100.0f);
}


void Jogador::update() {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	ypos *= -1;
	float moveSpeed = 0.01, mouseSpeed = 0.1;
	if (ypos > 89 / mouseSpeed) {
		ypos = 89 / mouseSpeed;
		glfwSetCursorPos(window, xpos, -ypos);
	}
	if (ypos < -89 / mouseSpeed) {
		ypos = -89 / mouseSpeed;
		glfwSetCursorPos(window, xpos, -ypos);
	}
	if (glfwGetMouseButton(window, 0) == GLFW_PRESS) {
		fire = true;
	}
	else {
		fire = false;
	}
	if (glfwGetMouseButton(window, 1) == GLFW_PRESS) {
		zoom = true;
	}
	if (glfwGetMouseButton(window, 1) == GLFW_RELEASE) {
		zoom = false;
	}
	xpos *= mouseSpeed;
	ypos *= mouseSpeed;
	dir.x = cos(xpos / 180 * M_PI) * cos(ypos / 180 * M_PI);
	dir.z = sin(xpos / 180 * M_PI) * cos(ypos / 180 * M_PI);
	dir.y = sin(ypos/180*M_PI);
	dir = glm::normalize(dir);
	vec3 forward = glm::normalize(vec3(0.1f, 0.0f, 0.1f) * dir);
	vec3 right = glm::normalize(vec3(0.1f, 0.0f, 0.1f) * vec3(-dir.z, 0.0f, dir.x));
	if (glfwGetKey(window, 'W')) {
		pos += forward*moveSpeed;
	}
	if (glfwGetKey(window, 'S')) {
		pos -= forward*moveSpeed;
	}
	if (glfwGetKey(window, 'D')) {
		pos += right*moveSpeed;
	}
	if (glfwGetKey(window, 'A')) {
		pos -= right*moveSpeed;
	}
	
}
vec3 Jogador::getPos() {
	return pos;
}
vec3 Jogador::getDir() {
	return dir;
}
#include "Camera.hpp"
#include <iostream>
#include <cstdlib>

extern GLFWwindow* window;

Camera::Camera() {
}

Camera::~Camera() {

}

Camera::Camera(glhf::Program prog, glm::vec3 pos, double angle){
	_prog = prog;
	_pos = pos;
	_angle = angle;
	_projection = glm::perspective(45.0f, 1024.0f/800.0f, 0.1f, 100.0f);
	_cameraID = glGetUniformLocation(_prog.getId(), "viewProjection");
}


void Camera::translate(glm::vec3 v){
	_pos += v;

	updateUniform();	
}

void Camera::rotate(double angle){
	_angle += angle;
	updateUniform();	
}

void Camera::updateUniform() {
	glm::mat4 view = glm::lookAt(
		_pos,
		_pos + glm::vec3(0, 0, 10),
		glm::normalize(glm::vec3(-glm::cos(_angle), -glm::sin(_angle), 0)));

	glm::mat4 camera = _projection * view;
	glUniformMatrix4fv(_cameraID, 1, GL_FALSE, &camera[0][0]);
}

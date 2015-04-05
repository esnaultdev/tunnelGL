#include "Camera.hpp"

#include <iostream>

Camera::Camera() {

}

Camera::~Camera() {

}

Camera::Camera(glhf::Program prog, Player *player) {
	_prog = prog;
	_player = player;
	_projection = glm::perspective(45.0f, 1024.0f/800.0f, 0.1f, 100.0f);
	_cameraID = glGetUniformLocation(_prog.getId(), "viewProjection");
}

void Camera::update(double dt) {
	glm::vec3 posPlayer = _player->getPos();
	glm::vec3 offsetCamera(0, 0, -0);
	posPlayer.x *= 0.25;
	posPlayer.y *= 0.25;

	glm::mat4 view = glm::lookAt(
		posPlayer + offsetCamera,
		posPlayer + glm::vec3(0,0,10),
		glm::normalize(glm::vec3(-posPlayer.x,-posPlayer.y,0)));

	glm::mat4 camera = _projection * view;
	glUniformMatrix4fv(_cameraID, 1, GL_FALSE, &camera[0][0]);
}
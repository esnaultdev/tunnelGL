#include "Camera.hpp"
#include <iostream>
#include <cstdlib>

extern GLFWwindow* window;

Camera::Camera() {
}

Camera::~Camera() {

}

Camera::Camera(glhf::Program prog, Player *player) {
	_prog = prog;
	_player = player;
	_projection = glm::perspective(45.0f, 1024.0f/800.0f, 0.1f, 100.0f);
	_cameraID = glGetUniformLocation(_prog.getId(), "viewProjection");

	_offsetCameraNormal.y = -player->getRadius()*2;
	_offsetCameraNormal.z = 0;
	_offsetCameraBoost.y = -player->getRadius()*2;
	_offsetCameraBoost.z = player->getRadius()*10;
}

void Camera::update(double dt) {
	glm::vec3 posPlayer = _player->getPos();
	glm::vec3 posCamera = posPlayer;

	_offsetCameraNormal.y = -_player->getRadius()*4 + std::min(0.5f * _player->getSpeed(), _player->getRadius()*1.5f);
	_offsetCameraNormal.z = 0 + std::min(0.5f * _player->getSpeed(), _player->getRadius()*10.f);


	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ) {
		_timeMove += dt;
		if (_timeMove > DURATION_BOOST_ANIM)
			_timeMove = DURATION_BOOST_ANIM;
		move(dt);

		posCamera = glm::vec3(posCamera.x + ((std::rand() % 3 + 1 - 3/2.f)/200.f), posCamera.y + ((std::rand() % 3 + 1 - 3/2.f)/200.f), posCamera.z);
	} else {
		_timeMove -= dt;
		if (_timeMove < 0)
			_timeMove = 0;
		move(dt);
	}

	//std::cout << _offsetCamera[0] << " " << _offsetCamera[1] << " " << _offsetCamera[2] << std::endl;
	posCamera += _offsetCamera;

	glm::mat4 view = glm::lookAt(
		posCamera,
		posPlayer + glm::vec3(0,0,10),
		glm::normalize(glm::vec3(-posPlayer.x,-posPlayer.y,0)));

	glm::mat4 camera = _projection * view;
	glUniformMatrix4fv(_cameraID, 1, GL_FALSE, &camera[0][0]);
}

void Camera::move(double dt) {
	for (int i = 1; i < 3; ++i) {
		float c = _offsetCameraBoost[i] - _offsetCameraNormal[i];
		float b = _offsetCameraNormal[i];
		float d = DURATION_BOOST_ANIM;
		float t = _timeMove / (d / 2);

		t = _timeMove/d;
		t -= 1;
		_offsetCamera[i] = c*(t*t*t+1)+b;
		/*
		if (t < 1){
			_offsetCamera[i] = c/2 * std::pow(2, 10 * (t - 1)) + b;
		}
		else {
			t -= 1;
			_offsetCamera[i] = c/2 * (- std::pow(2, -10 * t) + 2) + b;
		} */
	}
	_offsetCamera[0] = cos(_player->getAngle()) * _offsetCamera[1];
	_offsetCamera[1] = sin(_player->getAngle()) * _offsetCamera[1];
}
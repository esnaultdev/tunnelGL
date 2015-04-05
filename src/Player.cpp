#include "Player.hpp"
#include <iostream>
#include <GLFW/glfw3.h>

extern GLFWwindow* window;

Player::Player() {

}

Player::Player(glhf::Program prog) {
	//build the player object with prog
	_pos = glm::vec3(0, 0, 0);
	_speed = glm::vec3(0, 0, 0.01);
	_obj.setProgram(prog);
	_obj.load("cube.obj");
	_obj.initVao();
	_obj.scale(_radius / 0.5);
}

Player::~Player() {
	_obj.draw();
}

void Player::update(double dt, float radiusTunnel) {
	//Speed without friction on Z, but not on x and y
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ) { //left
		_angleSpeed += dt/8;
		if (_angleSpeed < -ANGLESPEED_MAX)
			_angleSpeed = -ANGLESPEED_MAX;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ) { //right	
		_angleSpeed -= dt/8;
		if (_angleSpeed > ANGLESPEED_MAX)
			_angleSpeed = ANGLESPEED_MAX;
	}
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ) { //boost
		_speed.z += dt/100.0 * 2;
	} else {
		_speed.z += dt/1000.0;
	}

	_angle += _angleSpeed;
	_obj.rotate(_angleSpeed, 0, 0, 1);

	_pos.x = cos(_angle) * (radiusTunnel - _radius);
	_pos.y = sin(_angle) * (radiusTunnel - _radius);
	_pos.z += _speed.z;
	_obj.setTranslation(_pos.x, _pos.y, _pos.z);

	//std::cout << _pos.x << " " << _pos.y << " " << _pos.z << 
	//	" Speed:" << _angleSpeed << " " << _speed.z << std::endl;;

	_angleSpeed *= FRICTION;
}

void Player::draw() {
	_obj.draw();
}

glm::vec3 Player::getPos() {
	return _pos;
}

float Player::getRadius() {
	return _radius;
}

float Player::getSpeed() {
	return _speed.z;
}

float Player::getAngle() {
	return _angle;
}
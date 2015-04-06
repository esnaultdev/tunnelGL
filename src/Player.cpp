#include "Player.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include <sstream>
#include "PrintText.hpp"

extern GLFWwindow* window;

Player::Player() {

}

Player::Player(glhf::Program prog) {
	//build the player object with prog
	_pos = glm::vec3(0, 0, 0);
	_speed = glm::vec3(0, 0, 0.01);
	_obj.setProgram(prog);
	_obj.load("vaisseau.obj");
	_obj.initVao();
	_obj.setTexture("vaisseau.tga", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, 10.0);
	_obj.scale(_radius * 2 / 0.65);
}

Player::~Player() {
	_obj.draw();
}

void Player::update(double dt, float radiusTunnel) {
	//Speed without friction on Z, but not on x and y
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ) { //left
		_angleSpeed += dt/8;
		if (_angleSpeed < -ANGLESPEED_MAX)
			_angleSpeed = -ANGLESPEED_MAX;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ) { //right	
		_angleSpeed -= dt/8;
		if (_angleSpeed > ANGLESPEED_MAX)
			_angleSpeed = ANGLESPEED_MAX;
	}
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ) { //boost
		_speed.z += dt/100.0 * 2;
		_score += dt * _speed.z * 2000;
	} else {
		_speed.z += dt/1000.0;
		_score += dt * _speed.z * 1000;
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

void Player::drawText() {
	std::ostringstream strs;
	strs << std::floor(_score);
	printText2D(strs.str().c_str(), 400, 30, 50);

	std::ostringstream strs2;
	strs2 << std::floor(getSpeed() * 1000) << "km/h";
	printText2D(strs2.str().c_str(), 30, 750, 40);
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

int Player::getScore() {
	return (int) _score;
}
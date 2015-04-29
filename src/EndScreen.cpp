#include <iostream>
#include <sstream>
#include <cmath>

#include "EndScreen.hpp"
#include "utils/textures.h"

#include "Engine.hpp"
#include "PrintText.hpp"

extern Engine* engine;
extern GLFWwindow* window;

EndScreen::EndScreen(glhf::Program prog, int time, int score, int speed, GameScreen *screen, Tunnel * tunnel, glm::vec3 posPlayer, double anglePlayer ){
	_prog = prog;
	_score = score;
	_time = time;
	_speed = speed;
	_screen = screen;
	_pos = posPlayer;
	_angle = anglePlayer;
	_tunnel = tunnel;

	std::cout << "Time: " << time << " " << " Score: " << score << " Speed: " << speed << std::endl;
}

EndScreen::~EndScreen(){
	cleanupText2D();
	delete _tunnel;
}

void EndScreen::init(){
	initText2D("font.DDS", 16);
	_skytube = SkyTube(_prog, _pos);
	_camera = Camera(_prog, _pos, _angle);
	_tunnel->setMuted(true);
}

void EndScreen::update(double dt) {
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS ) { //boost
		engine->setNextScreen(_screen);
	}

	glm::vec3 oldPos = _pos;
	_pos.x *= 0.9;
	_pos.y *= 0.9;
	_pos.z += (float)_speed / 1000;

	_tunnel->update(dt, _pos.z);
	_camera.translate(_pos - oldPos);
	_camera.rotate(dt/4);
	_skytube.setPos(glm::vec3(0, 0, _pos.z));
}

void EndScreen::draw(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	_tunnel->draw();
	_skytube.draw();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	printText2D(std::string("Press ENTER").c_str(), 165, 150, 60);
	printText2D(std::string("to try again!").c_str(), 130, 80, 60);

	std::ostringstream strs;
	strs << "Score: " << _score;
	printText2D(strs.str().c_str(), 50, 550, 40);

	std::ostringstream strs4;
	strs4 << "Speed: " << _speed;
	printText2D(strs4.str().c_str(), 50, 350, 40);

	std::ostringstream strs2;
	int sec = _time % 60;
	strs2 << "Time: " << _time/60 << ":";
	if (sec < 10)
		strs2 << "0" << sec;
	else
		strs2 << sec;
	printText2D(strs2.str().c_str(), 50, 450, 40);
}

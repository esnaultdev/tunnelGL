#include <iostream>
#include <sstream>
#include <cmath>
#include <glm/glm.hpp>
#include <irrKlang.h>
#include "MenuScreen.hpp"
#include "utils/textures.h"

#include "Engine.hpp"
#include "GameScreen.hpp"
#include "PrintText.hpp"
#include "Tunnel.hpp"
#include "Camera.hpp"

extern Engine* engine;
extern GLFWwindow* window;
extern irrklang::ISoundEngine* SoundEngine;

MenuScreen::MenuScreen(glhf::Program prog){
	_prog = prog;
}

MenuScreen::~MenuScreen(){
	cleanupText2D();
}

void MenuScreen::init(){
	initText2D("font.DDS", 16);
	
	_skytube = SkyTube(_prog, glm::vec3(0, 0, 0));
	_camera = Camera(_prog, glm::vec3(0, 0, 0), 0);	
	_tunnel = Tunnel(_prog);
	_tunnel.setMuted(true);
	_posZ = 0;

}

void MenuScreen::update(double dt) {
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS ) { //boost
		engine->setNextScreen(new GameScreen(_prog));
	}
	double diffZ = dt * 8;
	_posZ += diffZ;
	_tunnel.update(dt, _posZ);
	_camera.translate(glm::vec3(0, 0, diffZ));
	_camera.rotate(dt / 2);
	_skytube.setPos(glm::vec3(0, 0, _posZ));
}

void MenuScreen::draw(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	_tunnel.draw();
	_skytube.draw();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	std::ostringstream strs3;
	strs3 << "TUNNEL GL";
	printText2D(strs3.str().c_str(), 60, 650, 100);

	std::ostringstream strs;
	strs << "Press ENTER to start!";
	printText2D(strs.str().c_str(), 20, 100, 50);

	printText2D("Move: Q/D or LEFT/RIGHT", 50, 200, 30);

	std::ostringstream strs2;
	strs2 << "Boost: SPACE";
	printText2D(strs2.str().c_str(), 50, 250, 30);
}

#include <iostream>
#include <sstream>
#include <cmath>
#include "GameScreen.hpp"

#include "Box.hpp"
#include "Sphere.hpp"
#include "PrintText.hpp"
#include "Tunnel.hpp"

extern GLFWwindow* window;

GameScreen::GameScreen(glhf::Program prog){
	_prog = prog;
}

GameScreen::~GameScreen(){
	cleanupText2D();
}

void GameScreen::init(){
	Box *b = new Box(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0, 1, 1), glm::vec3(1, 0, 1));
	Sphere *s = new Sphere(glm::vec3(-0.5, -0.5, 0), 1);
	std::cout << b->collideWith(s) << std::endl;
	initText2D("../resources/test.DDS", 11);
	
	_player = Player(_prog);
	_tunnel = Tunnel(_prog, &_player);
	_camera = Camera(_prog, &_player);
	_skytube = SkyTube(_prog, glm::vec3(0, 0, 0));
	_lightdirnID = glGetUniformLocation(_prog.getId(), "lightdirn");
}

void GameScreen::update(double dt) {
	_time += dt;

	_player.update(dt, _tunnel.getRadius());

	glm::vec3 posPlayer = _player.getPos();

	if (_tunnel.update(dt))
		init();

	_camera.update(dt);
	_skytube.setPos(posPlayer);

	glm::vec2 light(-posPlayer.x, -posPlayer.y);
	light = glm::normalize(light);
	glUniform3f(_lightdirnID, light.x, light.y, 1);
}

void GameScreen::draw(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	_tunnel.draw();
	_player.draw();
	_skytube.draw();


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	_player.drawText();

	std::ostringstream strs3;
	int sec = ((int)_time) % 60;
	strs3 << std::floor(std::floor(_time/60)) << ":";
	if (sec < 10)
		strs3 << "0" << sec;
	else
		strs3 << sec;
	printText2D(strs3.str().c_str(), 800, 750, 50);
}
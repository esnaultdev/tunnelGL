#include <iostream>
#include <sstream>
#include <cmath>
#include "EndScreen.hpp"
#include "GameScreen.hpp"
#include "Engine.hpp"
#include "Box.hpp"
#include "Sphere.hpp"
#include "PrintText.hpp"
#include "Tunnel.hpp"

extern Engine* engine;
extern GLFWwindow* window;

GameScreen::GameScreen(glhf::Program prog){
	std::cout << "===== HEY =====" << std::endl;
	_prog = prog;
	SoundEngine = irrklang::createIrrKlangDevice();
	music = SoundEngine->play2D("../resources/Hexagonest_Stage.ogg", true, false, true);
	music->setVolume(0.4);
	music->setIsPaused(true);
}

GameScreen::~GameScreen(){
	cleanupText2D();
	music->stop();
	music->drop();
	SoundEngine->drop();
}

void GameScreen::init(){
	Box *b = new Box(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0, 1, 1), glm::vec3(1, 0, 1));
	Sphere *s = new Sphere(glm::vec3(-0.5, -0.5, 0), 1);
	std::cout << b->collideWith(s) << std::endl;
	initText2D("../resources/font.DDS", 16);
	
	_time = 0;
	_player = Player(_prog);
	_tunnel = Tunnel(_prog, &_player);
	_camera = Camera(_prog, &_player);
	_skytube = SkyTube(_prog, glm::vec3(0, 0, 0));
	_posShipID = glGetUniformLocation(_prog.getId(), "posShip");
	_lightAmbientID = glGetUniformLocation(_prog.getId(), "colorReal");
	
	music->setIsPaused(false);
	music->setPlayPosition(0);
}

void GameScreen::update(double dt) {
	_time += dt;

	_player.update(dt, _tunnel.getRadius());

	glm::vec3 posPlayer = _player.getPos();

	if (_tunnel.update(dt)) {
		engine->setNextScreen(new EndScreen(_prog, (int) _time, _player.getScore(), (int) (_player.getSpeed()*1000)));
	}

	_camera.update(dt);
	_skytube.setPos(posPlayer);

	float speed = (_player.getSpeed() - 0.07)*7;
	glm::vec3 ambient(1 - speed, 1 - speed, 1 - speed);
	glUniform3f(_lightAmbientID, ambient.x, ambient.y, ambient.z);
	glUniform3f(_posShipID, posPlayer.x, posPlayer.y, posPlayer.z);
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
	_tunnel.drawText();

	std::ostringstream strs3;
	int sec = ((int)_time) % 60;
	strs3 << std::floor(std::floor(_time/60)) << ":";
	if (sec < 10)
		strs3 << "0" << sec;
	else
		strs3 << sec;
	printText2D(strs3.str().c_str(), 800, 750, 50);
}

#include <iostream>
#include <sstream>
#include <cmath>
#include <irrKlang.h>
#include "EndScreen.hpp"
#include "GameScreen.hpp"
#include "Engine.hpp"
#include "PrintText.hpp"
#include "Tunnel.hpp"

extern Engine* engine;
extern GLFWwindow* window;
extern irrklang::ISoundEngine* SoundEngine;

GameScreen::GameScreen(glhf::Program prog){
	_prog = prog;
	_player = Player(_prog);
	_skytube = SkyTube(_prog, glm::vec3(0, 0, 0));

	music = SoundEngine->play2D("../resources/Hexagonest_Stage.ogg", true, false, true);
	music->setVolume(0.4);
	music->setIsPaused(true);
}

GameScreen::~GameScreen(){
	cleanupText2D();
	music->drop();
}

void GameScreen::init(){
	initText2D("../resources/font.DDS", 16);
	
	_time = 0;
	
	_player.reset();
	_tunnel = new Tunnel(_prog);
	_camera = PlayerCamera(_prog, &_player);
	_posShipID = glGetUniformLocation(_prog.getId(), "posShip");
	_lightAmbientID = glGetUniformLocation(_prog.getId(), "colorReal");
	
	music->setIsPaused(false);
	music->setPlayPosition(0);
}

void GameScreen::update(double dt) {
	_time += dt;

	_player.update(dt, _tunnel->getRadius());

	glm::vec3 posPlayer = _player.getPos();

	_tunnel->update(dt, _player.getPos().z);

	int isCrashed = _tunnel->isHole(_player.getAngle(), _player.getPos().z);

	if (isCrashed > 0) {
		irrklang::ISound *sfx;
		if (isCrashed == 1) {
			sfx = SoundEngine->play2D("../resources/fall.ogg", false, false, true);
			sfx->setVolume(0.8);
		}
		else {
			sfx = SoundEngine->play2D("../resources/crash.ogg", false, false, true);
			sfx->setVolume(0.3);
		}
		engine->setNextScreen(new EndScreen(_prog, (int) _time, _player.getScore(), (int) (_player.getSpeed()*1000), this, _tunnel, posPlayer, _player.getAngle()));
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
	
	_tunnel->draw();
	_player.draw();
	_skytube.draw();


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	_player.drawText();
	_tunnel->drawText();

	std::ostringstream strs3;
	int sec = ((int)_time) % 60;
	strs3 << std::floor(std::floor(_time/60)) << ":";
	if (sec < 10)
		strs3 << "0" << sec;
	else
		strs3 << sec;
	printText2D(strs3.str().c_str(), 800, 750, 50);
}

void GameScreen::onEnd() {
	music->setIsPaused(true);
}

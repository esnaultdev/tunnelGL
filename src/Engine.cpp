#include "MenuScreen.hpp"
#include "Engine.hpp"
#include <iostream>
#include <irrKlang.h>

irrklang::ISoundEngine * SoundEngine = irrklang::createIrrKlangDevice();

Engine::Engine(glhf::Program prog){

	_currentScreen = new MenuScreen(prog);
	_currentScreen->init();
	_prog = prog;
}

Engine::~Engine(){

}

void Engine::update(double currentTime){
	double dt = currentTime - _prevTime;
	_prevTime = currentTime;

	if (_nextScreen != 0) {
		_currentScreen->onEnd();
		_currentScreen = _nextScreen;
		_nextScreen = 0;
		_currentScreen->init();
	}

	_currentScreen->update(dt);
}

void Engine::draw(){
	_currentScreen->draw();
}

void Engine::setNextScreen(Screen *next){
	_nextScreen = next;
}

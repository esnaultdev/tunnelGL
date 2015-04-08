#include "Tunnel.hpp"
#include <irrKlang.h>
#include <cstdlib>
#include <sstream>
#include "PrintText.hpp"

extern irrklang::ISoundEngine* SoundEngine;

Tunnel::Tunnel() {
	srand(time(NULL));
}

Tunnel::Tunnel(glhf::Program prog) {
	_prog = prog;

	TunnelSection::loadMatricesFile();
	_tunnel[0] = TunnelSection(_prog);
	_tunnel[0].loadNew(0);
	_tunnel[0].init(0);

	for (int i = 1; i < NB_TUNNEL; ++i) {
		_tunnel[i] = TunnelSection(_prog);
		if (_tunnel[i - 1].hasNext()) {
			_tunnel[i].loadNext(_tunnel[i - 1].getNextId());
		} else {
			_tunnel[i].loadNew(1);
		}
		_tunnel[i].init(_tunnel[i - 1].getPosEndZ());
	}
}

Tunnel::~Tunnel() {

}

void Tunnel::update(double dt, double posz) {
	_newLevelTime += dt;

	if (posz >= _tunnel[0].getPosEndZ())
		nextTunnel();
	
}

int Tunnel::isHole(double angle, double posz){
	return _tunnel[0].isHole(angle, posz);
}

void Tunnel::nextTunnel() {
	_nbSectionEnded++;

	TunnelSection tunnel = TunnelSection(_prog);
	if (_tunnel[NB_TUNNEL - 1].hasNext()) {
		tunnel.loadNext(_tunnel[NB_TUNNEL - 1].getNextId());
	} else {
		int level = _nbSectionEnded / NB_SECTION_BEFORE_LEVEL + 1;

		if (level > _level){
			_newLevel = true;
			_newLevelTime = 0;	
			_level = level;
			if (!_muted) {
				irrklang::ISound *sfx = SoundEngine->play2D("../resources/levelup.ogg", false, false, true);
				sfx->setVolume(0.4);
			}
		}

		tunnel.loadNew(rand() % _level + 1);
	}
	tunnel.init(_tunnel[NB_TUNNEL - 1].getPosEndZ());

	for (int i = 1; i < NB_TUNNEL; ++i) {
		_tunnel[i-1] = _tunnel[i];
	}
	_tunnel[NB_TUNNEL - 1] = tunnel;
}

float Tunnel::getRadius() {
	return _tunnel[0].getRadius();
}

void Tunnel::draw() {
	for (int i = 0; i < NB_TUNNEL; ++i)
		_tunnel[i].draw();
}

void Tunnel::drawText() {
	if (_newLevel) {
		if (_newLevelTime > TIME_DISPLAY_NEW_LEVEL)
			_newLevel = false;

		if ( ((int) (_newLevelTime*10)) % 2 == 0) {
 			std::ostringstream strs;
			strs << "LEVEL : " << _level;
			printText2D(strs.str().c_str(), 300, 420, 40);
		}
	}
}

void Tunnel::setMuted(bool muted) {
	_muted = muted;
}

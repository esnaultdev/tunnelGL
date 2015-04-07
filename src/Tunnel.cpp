#include "Tunnel.hpp"
#include <cstdlib>
#include <sstream>
#include "PrintText.hpp"

Tunnel::Tunnel() {
	srand(time(NULL));
}

Tunnel::Tunnel(glhf::Program prog, Player *player, irrklang::ISoundEngine *SoundEngine) {
	_prog = prog;
	_player = player;
	_SoundEngine = SoundEngine;

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

int Tunnel::update(double dt) {
	_newLevelTime += dt;

	if (_player->getPos().z >= _tunnel[0].getPosEndZ())
		nextTunnel();

	return _tunnel[0].isHole(_player->getAngle(), _player->getPos().z);
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
			irrklang::ISound *sfx = _SoundEngine->play2D("../resources/levelup.ogg", false, false, true);
			sfx->setVolume(0.4);
		}

		tunnel.loadNew(rand() % _level + 1);
		//std::cout << _level << std::endl;
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
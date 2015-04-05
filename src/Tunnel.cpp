#include "Tunnel.hpp"

Tunnel::Tunnel() {

}

Tunnel::Tunnel(glhf::Program prog, Player *player) {
	_prog = prog;
	_player = player;

	_tunnel[0] = TunnelSection(_prog);
	_tunnel[0].init(0);

	for (int i = 1; i < NB_TUNNEL; ++i) {
		_tunnel[i] = TunnelSection(_prog);
		_tunnel[i].init(_tunnel[i-1].getPosEndZ());
	}
}

Tunnel::~Tunnel() {

}

bool Tunnel::update(double dt) {
	if (_player->getPos().z >= _tunnel[0].getPosEndZ())
		nextTunnel();

	return _tunnel[0].isHole(_player->getAngle(), _player->getPos().z);
}

void Tunnel::nextTunnel() {
	TunnelSection tunnel = TunnelSection(_prog);
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
#ifndef _TUNNEL
#define _TUNNEL

#define NB_TUNNEL 4

#include "Player.hpp"
#include "glhfAPI.hpp"
#include "TunnelSection.hpp"

class Tunnel {

public:
	Tunnel();
	Tunnel(glhf::Program prog, Player *player);
	~Tunnel();
	void nextTunnel();
	bool update(double dt);
	void draw();
	float getRadius();
private:
	glhf::Program _prog;
	Player *_player;
	TunnelSection _tunnel[NB_TUNNEL];
};

#endif
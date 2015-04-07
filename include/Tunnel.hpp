#ifndef _TUNNEL
#define _TUNNEL

#define NB_TUNNEL 4
#define NB_SECTION_BEFORE_LEVEL 10
#define TIME_DISPLAY_NEW_LEVEL 2.0f

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
	void drawText();
private:
	glhf::Program _prog;
	Player *_player;
	TunnelSection _tunnel[NB_TUNNEL];
	int _nbSectionEnded = 0;
	int _level = 0;
	bool _newLevel = false;
	float _newLevelTime = 0;
};

#endif
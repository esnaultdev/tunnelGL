#ifndef _GAMESCREEN
#define _GAMESCREEN

#include <vector>
#include <GL/glew.h> 

#include "glhfAPI.hpp"

#include "Screen.hpp"
#include "Tunnel.hpp"
#include "Player.hpp"
#include "Camera.hpp"

#define NB_TUNNEL 4

class GameScreen : public Screen {

public:
	GameScreen(glhf::Program prog);
	~GameScreen();
	void init();
	void update(double dt);
	void draw();
	void nextTunnel();

private:
	glhf::Program _prog;
	Tunnel _tunnel[NB_TUNNEL];
	int _time = 0;
	Player _player;
	Camera _camera;
	float _lightdirnID;
};

#endif
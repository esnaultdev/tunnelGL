#ifndef _GAMESCREEN
#define _GAMESCREEN

#include <vector>
#include <GL/glew.h> 

#include "glhfAPI.hpp"

#include "Screen.hpp"
#include "Tunnel.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "SkyTube.hpp"

class GameScreen : public Screen {

public:
	GameScreen(glhf::Program prog);
	~GameScreen();
	void init();
	void update(double dt);
	void draw();

private:
	glhf::Program _prog;
	GLuint _lightAmbientID;
	float _time = 0;
	Player _player;
	Camera _camera;
	Tunnel _tunnel;
	SkyTube _skytube;
	GLint _posShipID;
};

#endif
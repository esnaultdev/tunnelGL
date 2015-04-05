#ifndef _GAMESCREEN
#define _GAMESCREEN

#include <vector>
#include <GL/glew.h> 

#include "glhfAPI.hpp"

#include "Screen.hpp"
#include "Tunnel.hpp"
#include "Player.hpp"
#include "Camera.hpp"

class GameScreen : public Screen {

public:
	GameScreen(glhf::Program prog);
	~GameScreen();
	void init();
	void update(double dt);
	void draw();

private:
	glhf::Program _prog;

	float _time = 0;
	Player _player;
	Camera _camera;
	Tunnel _tunnel;
	GLint _lightdirnID;
};

#endif
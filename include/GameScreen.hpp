#ifndef _GAMESCREEN
#define _GAMESCREEN

#include <vector>
#include <GL/glew.h> 
#include "glhfAPI.hpp"

#include "Screen.hpp"
#include "Tunnel.hpp"
#include "Player.hpp"

class GameScreen : public Screen {

public:
	GameScreen(glhf::Program prog);
	~GameScreen();
	void init();
	void update(double dt);
	void draw();
	void makeTriangle();

private:
	glhf::GLObject _triangle;
	glhf::Program _prog;
	Tunnel _tunnel;
	int _time = 0;
	Player _player;
};

#endif
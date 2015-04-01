#ifndef _GAMESCREEN
#define _GAMESCREEN

#include <vector>
#include <GL/glew.h> 
#include "Screen.hpp"
#include "glhfAPI.hpp"

class GameScreen : public Screen {

public:
	GameScreen(glhf::Program prog);
	~GameScreen();
	void init();
	void update(double dt);
	void draw();
	void make_triangle();

private:
	glhf::GLObject _triangle;
	glhf::Program _prog;
	int _time = 0;
};

#endif
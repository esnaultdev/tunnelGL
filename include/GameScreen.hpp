#ifndef _GAMESCREEN
#define _GAMESCREEN

#include "Screen.hpp"

class GameScreen : public Screen {

public:
	GameScreen();
	~GameScreen();
	void init();
	void update(double dt);
	void draw();

private:

};

#endif
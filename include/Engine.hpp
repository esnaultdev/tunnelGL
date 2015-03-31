#ifndef _ENGINE
#define _ENGINE

#include "Screen.hpp"

class Engine {

public:
	Engine();
	~Engine();
	void update(double currentTime);
	void draw();
	void setNextScreen(Screen *next);

private:
	Screen *_currentScreen;
	Screen *_nextScreen = 0;
	double _prevTime = 0;
};

#endif
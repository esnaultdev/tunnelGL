#ifndef _ENDSCREEN
#define _ENDSCREEN

#include <GL/glew.h> 
#include "glhfAPI.hpp"

#include "GameScreen.hpp"
#include "Camera.hpp"
#include "Tunnel.hpp"
#include "SkyTube.hpp"

class EndScreen : public Screen {

public:
	EndScreen(glhf::Program prog, int time, int score, int speed, GameScreen *screen, Tunnel * tunnel, glm::vec3 posPlayer, double anglePlayer);
	~EndScreen();
	void init();
	void update(double dt);
	void draw();

private:
	glhf::Program _prog;
	int _time;
	int _speed;
	int _score;
	GameScreen *_screen;
	glm::vec3 _pos;
	double _angle;
	Tunnel * _tunnel;
	Camera _camera;
	SkyTube _skytube;
};

#endif
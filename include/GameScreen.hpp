#ifndef _GAMESCREEN
#define _GAMESCREEN

#include <vector>
#include <GL/glew.h> 
#include <irrKlang.h>

#include "glhfAPI.hpp"

#include "Screen.hpp"
#include "Tunnel.hpp"
#include "Player.hpp"
#include "PlayerCamera.hpp"
#include "SkyTube.hpp"

class GameScreen : public Screen {

public:
	GameScreen(glhf::Program prog);
	~GameScreen();
	void init();
	void update(double dt);
	void draw();
	void onEnd();

private:
	glhf::Program _prog;
	GLuint _lightAmbientID;
	float _time = 0;
	Player _player;
	PlayerCamera _camera;
	Tunnel _tunnel;
	SkyTube _skytube;
	irrklang::ISound *music;
	GLint _posShipID;
};

#endif

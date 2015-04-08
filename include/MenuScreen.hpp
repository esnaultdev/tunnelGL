#ifndef _MENUSCREEN
#define _MENUSCREEN

#include <GL/glew.h> 
#include "glhfAPI.hpp"

#include "Screen.hpp"
#include "Camera.hpp"
#include "Tunnel.hpp"
#include "SkyTube.hpp"

class MenuScreen : public Screen {

public:
	MenuScreen(glhf::Program prog);
	~MenuScreen();
	void init();
	void update(double dt);
	void draw();

private:
	glhf::Program _prog;
	SkyTube _skytube;
	Tunnel _tunnel;
	Camera _camera;
	double _posZ;
};

#endif

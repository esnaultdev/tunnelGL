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
	void makeResources();

private:
	glhf::Program _prog;
	GLuint _idTexture;
	GLuint _vao;
	GLuint _vertexID;
	GLuint _uvID;
	GLuint _idProgram;
	GLuint _idUniform;
	std::vector<glm::vec2> _position;
	SkyTube _skytube;
	Tunnel _tunnel;
	Camera _camera;
};

#endif

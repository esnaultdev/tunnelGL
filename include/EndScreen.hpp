#ifndef _ENDSCREEN
#define _ENDSCREEN

#include <GL/glew.h> 
#include "glhfAPI.hpp"

#include "Screen.hpp"

class EndScreen : public Screen {

public:
	EndScreen(glhf::Program prog, int time, int score, int speed);
	~EndScreen();
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
	int _time;
	int _speed;
	int _score;
};

#endif
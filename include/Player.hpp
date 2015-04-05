#ifndef _PLAYER
#define _PLAYER

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp> 
#include <cmath>
#include "glhfAPI.hpp"

#define FRICTION 0.95
#define ANGLESPEED_MAX M_PI/30

class Player {

public:
	Player();
	Player(glhf::Program prog);
	~Player();
	void update(double dt, float tunnelRadius);
	void draw();
	glm::vec3 getPos();
	float getRadius();
	float getSpeed();
	float getAngle();

private:
	glm::vec3 _pos;
	glm::vec3 _speed;
	float _angle = M_PI/2 * 3;
	float _angleSpeed = 0;
	float _radius = 0.1;
	glhf::GLObject _obj;
};

#endif
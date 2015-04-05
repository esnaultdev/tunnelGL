#ifndef _SKYTUBE
#define _SKYTUBE

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp> 
#include <cmath>
#include "glhfAPI.hpp"

#define SKYTUBE_NB_POINT_Z 18
#define SKYTUBE_NB_POLY 12
#define SKYTUBE_RADIUS 5

class SkyTube {

public:
	SkyTube();
	SkyTube(glhf::Program prog, glm::vec3 pos);
	~SkyTube();
	void draw();
	void setPos(glm::vec3 pos);

private:
	glhf::GLObject _obj;
	glhf::Program _prog;
};

#endif
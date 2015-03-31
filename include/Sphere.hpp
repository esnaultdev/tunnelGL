#ifndef _SPHERE
#define _SPHERE

#include <glm/glm.hpp>

class Sphere {

public:
	Sphere(glm::vec3 pos, int r);
	~Sphere();
	bool collideWith(glm::vec3 p);
	glm::vec3 getPos();

private:
	glm::vec3 _pos;
	int _r;
};

#endif
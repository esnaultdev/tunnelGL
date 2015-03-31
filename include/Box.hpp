#ifndef _BOX
#define _BOX

#include <glm/glm.hpp>
#include "Sphere.hpp"

class Box {

public:
	Box(glm::vec3 pos, glm::vec3 width, glm::vec3 height, glm::vec3 depth);
	~Box();
	bool collideWith(Sphere *s);

private:
	glm::vec3 _pos;
	glm::vec3 _width;
	glm::vec3 _height;
	glm::vec3 _depth;
};

#endif
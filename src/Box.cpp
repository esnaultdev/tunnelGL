#include "Box.hpp"

Box::Box(glm::vec3 pos, glm::vec3 width, glm::vec3 height, glm::vec3 depth) : 
	_pos(pos), _width(width), _height(height), _depth(depth) {

}

Box::~Box(){

}

bool Box::collideWith(Sphere *s){
	glm::vec3 x = glm::normalize(_width);
	glm::vec3 y = glm::normalize(_height);
	glm::vec3 z = glm::normalize(_depth);
	float width = glm::length(_width);
	float height = glm::length(_height);
	float depth = glm::length(_depth);

	glm::mat3x3 m = glm::inverse(glm::mat3x3(x, y, z));

	glm::vec3 sPos = s->getPos();
	sPos = m * sPos;

	glm::vec3 bPos = m * _pos;

	glm::vec3 pos(sPos);

	if (pos[0] > bPos[0] + width - 1) {
		pos[0] = bPos[0] + width - 1;
	} else if (pos[0] < bPos[0]) {
		pos[0] = bPos[0];
	}

	if (pos[1] > bPos[1] + height - 1) {
		pos[1] = bPos[1] + height - 1;
	} else if (pos[1] < bPos[1]) {
		pos[1] = bPos[1];
	}

	if (pos[2] > bPos[2] + depth - 1) {
		pos[2] = bPos[2] + depth - 1;
	} else if (pos[2] < bPos[2]) {
		pos[2] = bPos[2];
	}

	return s->collideWith(pos);
}

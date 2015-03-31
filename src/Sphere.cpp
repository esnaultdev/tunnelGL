#include "Sphere.hpp"

Sphere::Sphere(glm::vec3 pos, int r) : _pos(pos), _r(r) {

}

Sphere::~Sphere(){

}

/*
bool Sphere::collideWith(Box *b){
	b->collideWith(this);
}*/

bool Sphere::collideWith(glm::vec3 p){
	return glm::length((p - _pos)) <= _r;
}

glm::vec3 Sphere::getPos(){
	return _pos;
}

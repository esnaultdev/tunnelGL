#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "Tunnel.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

Tunnel::Tunnel() {

}

Tunnel::Tunnel(glhf::Program prog){
	_prog = prog;
}

void Tunnel::init(float posStartZ){
	_posStartZ = posStartZ;
	generateMatrix();
	makeSection();
}

Tunnel::~Tunnel(){
	
}

void Tunnel::draw(){
	_tunnelObj.draw();
}

void Tunnel::generateMatrix(){
	srand(time(NULL));

    for (int i = 0; i < TUNNEL_NB_POINT_Z; i++) {
    	for (int j = 0; j < TUNNEL_NB_POLY; j++) {
    		_matrix[i][j] = (rand()%100 +1 > 99) ? 0 : 1 ;
    	}
    }

}

void Tunnel::makeSection(){
	std::vector<unsigned int> indices; 	
	std::vector<float> position;
	std::vector<float> color;
	std::vector<float> normal;
	std::vector<float> uv;

    double angleStep = 2 * M_PI / TUNNEL_NB_POLY;
    double sideLength = _radius * 2 * glm::sin(M_PI / TUNNEL_NB_POLY);

    for(int i = 0; i < TUNNEL_NB_POINT_Z; i++) {
        for(int j = 0; j < TUNNEL_NB_POLY; j++) {
        	double theta = j * angleStep;

        	double x = glm::cos(theta) * _radius;
        	double y = glm::sin(theta) * _radius;
        	double z = sideLength * i + _posStartZ;
        	//std::cout << "(" << i << ", " << j << ") x : " << x << " y : " << y << " z : " << z << std::endl;

            position.push_back(x);
			position.push_back(y);
			position.push_back(z);

			color.push_back(cos(_posStartZ) + sin(_posStartZ));
			color.push_back(std::abs(cos(_posStartZ)));
			color.push_back(std::abs(sin(_posStartZ)));

			glm::vec3 pos(x, y, z);
			glm::vec3 center(0, 0, z);
			glm::vec3 normalVec(center-pos);
			normalVec = glm::normalize(normalVec);

			normal.push_back(normalVec.x);
			normal.push_back(normalVec.y);
			normal.push_back(normalVec.z);

			uv.push_back(0);
			uv.push_back(0);

			if (i != TUNNEL_NB_POINT_Z - 1 && _matrix[i][j] != 0) {
				// Outside
				indices.push_back(i * TUNNEL_NB_POLY + j);
				indices.push_back(i * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				indices.push_back((i+1) * TUNNEL_NB_POLY + j);

				indices.push_back((i+1) * TUNNEL_NB_POLY + j);
				indices.push_back(i * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				indices.push_back((i+1) * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));

				//Inside
				indices.push_back(i * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				indices.push_back(i * TUNNEL_NB_POLY + j);
				indices.push_back((i+1) * TUNNEL_NB_POLY + j);

				indices.push_back(i * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				indices.push_back((i+1) * TUNNEL_NB_POLY + j);
				indices.push_back((i+1) * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
			}
        }
    }

    _length = sideLength * (TUNNEL_NB_POINT_Z - 1);

	_tunnelObj = glhf::GLObject(_prog, position.size(), indices.size(), indices, position, color, normal, uv);
	_tunnelObj.initVao();
}

float Tunnel::getPosEndZ() {
	return _posStartZ + _length;
}

float Tunnel::getRadius() {
	return _radius;
}

bool Tunnel::isHole(float angle, float z) {
	int posZ = ((int) ((z - _posStartZ) / _length * TUNNEL_NB_POINT_Z)) % TUNNEL_NB_POINT_Z;
	int posAngle = (((int) std::abs(std::floor(angle / (M_PI * 2) * TUNNEL_NB_POLY)) % TUNNEL_NB_POLY));

	return ! _matrix[posZ][posAngle];
}
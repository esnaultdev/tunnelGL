#include <iostream>
#include <cstdlib>
#include <ctime>

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

    for (int i = 0; i < _length; i++) {
    	for (int j = 0; j < TUNNEL_NB_POLY; j++) {
    		_matrix[i][j] = (rand()%100 +1 >95) ? 0 : 1 ;
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

    for(int i = 0; i < _length; i++) {
        for(int j = 0; j < TUNNEL_NB_POLY; j++) {
        	double theta = j * angleStep;

        	double x = glm::cos(theta) * _radius;
        	double y = glm::sin(theta) * _radius;
        	double z = sideLength * i + _posStartZ;
        	//std::cout << "(" << i << ", " << j << ") x : " << x << " y : " << y << " z : " << z << std::endl;

            position.push_back(x);
			position.push_back(y);
			position.push_back(z);

			color.push_back(0);
			color.push_back(0.6);
			color.push_back(0.8);

			glm::vec3 pos(x, y, z);
			glm::vec3 center(0, 0, z);
			glm::vec3 normalVec(pos-center);
			normalVec = glm::normalize(normalVec);

			normal.push_back(normalVec.x);
			normal.push_back(normalVec.y);
			normal.push_back(normalVec.z);

			uv.push_back(0);
			uv.push_back(0);

			if (i != _length - 1 && _matrix[i][j] != 0) {
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

	_tunnelObj = glhf::GLObject(_prog, position.size(), indices.size(), indices, position, color, normal, uv);
	_tunnelObj.initVao();
}

float Tunnel::getPosEndZ() {
	return _posStartZ + _length;
}

float Tunnel::getRadius() {
	return _radius;
}
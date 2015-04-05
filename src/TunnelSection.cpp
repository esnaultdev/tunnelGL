#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "TunnelSection.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

TunnelSection::TunnelSection() {

}

TunnelSection::TunnelSection(glhf::Program prog){
	_prog = prog;
}

void TunnelSection::init(float posStartZ){
	_posStartZ = posStartZ;
	generateMatrix();
	makeSection();
}

TunnelSection::~TunnelSection(){
	
}

void TunnelSection::draw(){
	_tunnelObj.draw();
}

void TunnelSection::generateMatrix(){
	srand(time(NULL));

    for (int i = 0; i < TUNNEL_NB_POINT_Z; i++) {
    	for (int j = 0; j < TUNNEL_NB_POLY; j++) {
    		_matrix[i][j] = (rand()%100 +1 > 99) ? 0 : 1 ;
    	}
    }

}

void TunnelSection::makeSection(){
	std::vector<unsigned int> indices; 	
	std::vector<float> position;
	std::vector<float> color;
	std::vector<float> normal;
	std::vector<float> uv;

    double angleStep = 2 * M_PI / TUNNEL_NB_POLY;
    double sideLength = _radius * 2 * glm::sin(M_PI / TUNNEL_NB_POLY)*3;
    _length = sideLength * (TUNNEL_NB_POINT_Z - 1);

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

			double colorAlea = (_posStartZ + i * sideLength) / _length;
			color.push_back(cos(colorAlea) + sin(colorAlea));
			color.push_back(std::abs(cos(colorAlea)));
			color.push_back(std::abs(sin(colorAlea)));

			glm::vec3 pos(x, y, z);
			glm::vec3 center(0, 0, z);
			glm::vec3 normalVec(center-pos);
			normalVec = glm::normalize(normalVec);

			normal.push_back(normalVec.x);
			normal.push_back(normalVec.y);
			normal.push_back(normalVec.z);

			uv.push_back(i % 2);
			uv.push_back(j % 2);

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

	_tunnelObj = glhf::GLObject(_prog, position.size(), indices.size(), indices, position, color, normal, uv);
	_tunnelObj.initVao();
	_tunnelObj.setTexture("tunnelUnit.tga", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, 10.0);
}

float TunnelSection::getPosEndZ() {
	return _posStartZ + _length;
}

float TunnelSection::getRadius() {
	return _radius;
}

bool TunnelSection::isHole(float angle, float z) {
	int posZ = ((int) ((z - _posStartZ) / _length * TUNNEL_NB_POINT_Z)) % TUNNEL_NB_POINT_Z;
	int posAngle = (((int) std::abs(std::floor(angle / (M_PI * 2) * TUNNEL_NB_POLY)) % TUNNEL_NB_POLY));

	return ! _matrix[posZ][posAngle];
}
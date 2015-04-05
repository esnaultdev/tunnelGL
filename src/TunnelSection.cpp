#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "TunnelSection.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#define OBSTACLE 2
#define HOLE 1
#define SAFE 0

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
    for (int i = 0; i < TUNNEL_NB_POINT_Z-1; i++) {
    	for (int j = 0; j < TUNNEL_NB_POLY; j++) {
    		int random = rand()%100 +1;
    		if (random > 98) {
    			_matrix[i][j] = OBSTACLE; // obstacle
    		} else if (random > 96) {
    			_matrix[i][j] = HOLE; // hole
    		} else {
    			_matrix[i][j] = SAFE; // safe
    		}
    	}
    }

    for (int i = 0; i < TUNNEL_NB_POLY; ++i) {
    	_matrix[TUNNEL_NB_POINT_Z-1][i] = SAFE;
    }
}

void TunnelSection::makeSection(){
	std::vector<unsigned int> indices; 	
	std::vector<float> position;
	std::vector<float> color;
	std::vector<float> normal;
	std::vector<float> uv;

	std::vector<float> positionCube;
	std::vector<float> colorCube;
	std::vector<float> normalCube;
	std::vector<float> uvCube;

    double angleStep = 2 * M_PI / TUNNEL_NB_POLY;
    double sideLength = _radius * 2 * glm::sin(M_PI / TUNNEL_NB_POLY)*3;
    _length = sideLength * (TUNNEL_NB_POINT_Z - 1);


    int offset = -8;

    for(int i = 0; i < TUNNEL_NB_POINT_Z; ++i) {
        for(int j = 0; j < TUNNEL_NB_POLY; ++j) {
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

			if (i != TUNNEL_NB_POINT_Z - 1 && _matrix[i][j] == SAFE) {
				//Inside
				indices.push_back(i * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				indices.push_back(i * TUNNEL_NB_POLY + j);
				indices.push_back((i+1) * TUNNEL_NB_POLY + j);

				indices.push_back(i * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				indices.push_back((i+1) * TUNNEL_NB_POLY + j);
				indices.push_back((i+1) * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
			} else if (_matrix[i][j] == OBSTACLE && i != TUNNEL_NB_POINT_Z - 1) {
				double xx,yy,zz;
				
				for (int k = 0; k < 2; ++k) {
					theta = j * angleStep;
					xx = glm::cos(theta) * _radius/2;
					yy = glm::sin(theta) * _radius/2;
					zz = sideLength * (i+1) + _posStartZ;

					positionCube.push_back(xx);
					positionCube.push_back(yy);
					positionCube.push_back(z);

					colorCube.push_back(1);//cos(colorAlea) + sin(colorAlea));
					colorCube.push_back(1);//std::abs(cos(colorAlea)));
					colorCube.push_back(1);//std::abs(sin(colorAlea)));

					normalCube.push_back(0);
					normalCube.push_back(0);
					normalCube.push_back(-1);

					uvCube.push_back((i+1+k) % 2);
					uvCube.push_back((j+k) % 2);

					offset++;

					positionCube.push_back(xx);
					positionCube.push_back(yy);
					positionCube.push_back(zz);

					colorCube.push_back(1);//cos(colorAlea) + sin(colorAlea));
					colorCube.push_back(1);//std::abs(cos(colorAlea)));
					colorCube.push_back(1);//std::abs(sin(colorAlea)));

					normalCube.push_back(0);
					normalCube.push_back(0);
					normalCube.push_back(-1);

					uvCube.push_back((i+k) % 2);
					uvCube.push_back((j+k) % 2);

					offset++;

					theta = (j+1) * angleStep;
					xx = glm::cos(theta) * _radius/2;
					yy = glm::sin(theta) * _radius/2;

					positionCube.push_back(xx);
					positionCube.push_back(yy);
					positionCube.push_back(z);

					colorCube.push_back(1);//cos(colorAlea) + sin(colorAlea));
					colorCube.push_back(1);//std::abs(cos(colorAlea)));
					colorCube.push_back(1);//std::abs(sin(colorAlea)));

					normalCube.push_back(0);
					normalCube.push_back(0);
					normalCube.push_back(-1);

					uvCube.push_back((i+1+k) % 2);
					uvCube.push_back((j+1+k) % 2);

					offset ++;

					positionCube.push_back(xx);
					positionCube.push_back(yy);
					positionCube.push_back(zz);

					colorCube.push_back(1);//cos(colorAlea) + sin(colorAlea));
					colorCube.push_back(1);//std::abs(cos(colorAlea)));
					colorCube.push_back(1);//std::abs(sin(colorAlea)));

					normalCube.push_back(0);
					normalCube.push_back(0);
					normalCube.push_back(-1);

					uvCube.push_back((i+k) % 2);
					uvCube.push_back((j+1+k) % 2);

					offset++;
				}

				//front
				indices.push_back(i * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				indices.push_back(i * TUNNEL_NB_POLY + j);
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset);

				indices.push_back(i * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset);
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 2);

				//up
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset);
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 1);
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 2);

				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 1);
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 3);
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 2);

				//back
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 1);
				indices.push_back((i+1) * TUNNEL_NB_POLY + j);
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 3);

				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 3);
				indices.push_back((i+1) * TUNNEL_NB_POLY + j);
				indices.push_back((i+1) * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				
				//left
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 7);
				indices.push_back(i * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 6);

				indices.push_back((i+1) * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				indices.push_back(i * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 7);
				
				//right
				indices.push_back(i * TUNNEL_NB_POLY + j);
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 5);
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 4);
				
				indices.push_back(TUNNEL_NB_POINT_Z * TUNNEL_NB_POLY + offset + 5);
				indices.push_back(i * TUNNEL_NB_POLY + j);
				indices.push_back((i+1) * TUNNEL_NB_POLY + j);
			}
        }
    }

    position.insert(position.end(), positionCube.begin(), positionCube.end());
    normal.insert(normal.end(), normalCube.begin(), normalCube.end());
    color.insert(color.end(), colorCube.begin(), colorCube.end());
    uv.insert(uv.end(), uvCube.begin(), uvCube.end());

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
	int posAngle = (((int) std::floor(angle / (M_PI * 2) * TUNNEL_NB_POLY)) % TUNNEL_NB_POLY);

	if (posAngle < 0)
		posAngle += TUNNEL_NB_POLY;

	return _matrix[posZ][posAngle] != SAFE;
}
#include "SkyTube.hpp"

SkyTube::SkyTube() {

}

SkyTube::SkyTube(glhf::Program prog, glm::vec3 pos){
	_prog = prog;

	std::vector<unsigned int> indices; 	
	std::vector<float> position;
	std::vector<float> color;
	std::vector<float> normal;
	std::vector<float> uv;

    double angleStep = 2 * M_PI / SKYTUBE_NB_POLY;
    double sideLength = SKYTUBE_RADIUS * 2 * glm::sin(M_PI / SKYTUBE_NB_POLY)*3;

    for(int i = 0; i < SKYTUBE_NB_POINT_Z; i++) {
        for(int j = 0; j < SKYTUBE_NB_POLY; j++) {
        	double theta = j * angleStep;

        	double x = glm::cos(theta) * SKYTUBE_RADIUS;
        	double y = glm::sin(theta) * SKYTUBE_RADIUS;
        	double z = sideLength * i;
        	//std::cout << "(" << i << ", " << j << ") x : " << x << " y : " << y << " z : " << z << std::endl;

            position.push_back(x);
			position.push_back(y);
			position.push_back(z);

			color.push_back(1);
			color.push_back(1);
			color.push_back(1);

			glm::vec3 pos(x, y, z);
			glm::vec3 center(0, 0, z);
			glm::vec3 normalVec(center-pos);
			normalVec = glm::normalize(normalVec);

			normal.push_back(normalVec.x);
			normal.push_back(normalVec.y);
			normal.push_back(normalVec.z);

			uv.push_back(i % 2);
			uv.push_back(j % 2);

			if (i != SKYTUBE_NB_POINT_Z - 1) {
				//Inside
				indices.push_back(i * SKYTUBE_NB_POLY + ((j+1) % SKYTUBE_NB_POLY));
				indices.push_back(i * SKYTUBE_NB_POLY + j);
				indices.push_back((i+1) * SKYTUBE_NB_POLY + j);

				indices.push_back(i * SKYTUBE_NB_POLY + ((j+1) % SKYTUBE_NB_POLY));
				indices.push_back((i+1) * SKYTUBE_NB_POLY + j);
				indices.push_back((i+1) * SKYTUBE_NB_POLY + ((j+1) % SKYTUBE_NB_POLY));
			}
        }
    }

	_obj = glhf::GLObject(_prog, position.size(), indices.size(), indices, position, color, normal, uv);
	_obj.initVao();
	_obj.setTexture("tunnelUnit.tga", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, 10.0);
	_obj.setTranslation(pos[0], pos[1], pos[2]);
}

SkyTube::~SkyTube(){
	
}

void SkyTube::draw(){
	_obj.draw();
}

void SkyTube::setPos(glm::vec3 pos){
	_obj.setTranslation(pos[0], pos[1], pos[2]);
}

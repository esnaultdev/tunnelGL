#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Tunnel.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

Tunnel::Tunnel(){

}

Tunnel::Tunnel(glhf::Program prog){
	_prog = prog;
}

void Tunnel::init(){
	generate_matrix();
	make_section();
}

Tunnel::~Tunnel(){
	
}

void Tunnel::draw(){
	_tunnelObj.draw();
}

void Tunnel::generate_matrix(){
	srand(time(NULL));

    for (int i = 0; i < TUNNEL_LENGTH; i++) {
    	for (int j = 0; j < TUNNEL_NB_POLY; j++) {
    		_matrix[i][j] = (rand()%100 +1 >95) ? 0 : 1 ;
    	}
    }

}

void Tunnel::make_section(void){
	std::vector<unsigned int> indices; 	
	std::vector<float> position;
	std::vector<float> color;
	std::vector<float> normal;
	std::vector<float> uv;

    double angleStep = 2 * M_PI / TUNNEL_NB_POLY;
    double sideLength = TUNNEL_RADIUS * 2 * glm::sin(M_PI / TUNNEL_NB_POLY);

    for(int i = 0; i < TUNNEL_LENGTH; i++) {
        for(int j = 0; j < TUNNEL_NB_POLY; j++) {
        	double theta = j * angleStep;

        	double x = glm::cos(theta) * TUNNEL_RADIUS * (1 - (float)i/TUNNEL_LENGTH);
        	double y = glm::sin(theta) * TUNNEL_RADIUS * (1 - (float)i/TUNNEL_LENGTH);
        	double z = sideLength * i;
        	//std::cout << "(" << i << ", " << j << ") x : " << x << " y : " << y << " z : " << z << " factor : " <<  (1 - (float)i/TUNNEL_LENGTH) << std::endl;

            position.push_back(x);
			position.push_back(y);
			position.push_back(z);

			color.push_back(0);
			color.push_back(0.6);
			color.push_back(0.8);

			normal.push_back(1);
			normal.push_back(1);
			normal.push_back(1);

			uv.push_back(0);
			uv.push_back(0);

			if (i != TUNNEL_LENGTH - 1 && _matrix[i][j] != 0) {
				indices.push_back(i * TUNNEL_NB_POLY + j);
				indices.push_back(i * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				indices.push_back((i+1) * TUNNEL_NB_POLY + j);

				indices.push_back((i+1) * TUNNEL_NB_POLY + j);
				indices.push_back(i * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
				indices.push_back((i+1) * TUNNEL_NB_POLY + ((j+1) % TUNNEL_NB_POLY));
			}
        }
    }

	_tunnelObj = glhf::GLObject(_prog, position.size(), indices.size(), indices, position, color, normal, uv);
	_tunnelObj.initVao();
}
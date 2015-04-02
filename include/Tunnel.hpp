#ifndef _TUNNEL
#define _TUNNEL

#include <vector>
#include "glhfAPI.hpp"

#define TUNNEL_LENGTH 20
#define TUNNEL_NB_POLY 12
#define TUNNEL_RADIUS 0.5

class Tunnel {

public:
	Tunnel();
	Tunnel(glhf::Program prog);
	~Tunnel();
	void init(float posStartZ);
	void draw();
	float getPosEndZ();
	float getRadius();

private:
	void makeSection();
	void generateMatrix();

	glhf::GLObject _tunnelObj;
	glhf::Program _prog;
	int _matrix[TUNNEL_LENGTH][TUNNEL_NB_POLY];
	float _radius = TUNNEL_RADIUS;
	float _length = TUNNEL_LENGTH;
	float _posStartZ;
};

#endif
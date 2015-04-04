#ifndef _TUNNEL_SECTION
#define _TUNNEL_SECTION

#include <vector>
#include "glhfAPI.hpp"

#define TUNNEL_NB_POINT_Z 20
#define TUNNEL_NB_POLY 12
#define TUNNEL_RADIUS 0.5

class TunnelSection {

public:
	TunnelSection();
	TunnelSection(glhf::Program prog);
	~TunnelSection();
	void init(float posStartZ);
	void draw();
	float getPosEndZ();
	float getRadius();
	bool isHole(float angle, float z);

private:
	void makeSection();
	void generateMatrix();

	glhf::GLObject _tunnelObj;
	glhf::Program _prog;
	int _matrix[TUNNEL_NB_POINT_Z][TUNNEL_NB_POLY];
	float _radius = TUNNEL_RADIUS;
	float _length;
	float _posStartZ;
};

#endif
#ifndef _TUNNEL_SECTION
#define _TUNNEL_SECTION

#include <map>
#include "glhfAPI.hpp"

#define TUNNEL_NB_POINT_Z 25
#define TUNNEL_NB_POLY 12
#define TUNNEL_RADIUS 0.5
#define MATRICES_FILE "matrices.txt"

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

	void loadNew(const int difficulty);
	void loadNext(const int next);
	bool hasNext();
	int getNextId();
	static void loadMatricesFile();

private:
	void makeSection();

	glhf::GLObject _tunnelObj;
	glhf::Program _prog;
	int _matrixId;
	float _radius = TUNNEL_RADIUS;
	float _length;
	float _posStartZ;

	struct SectionMatrix {
		int id;
		int difficulty;
		int next;
		int matrix[TUNNEL_NB_POINT_Z][TUNNEL_NB_POLY];
	};
	static std::map<int, SectionMatrix> _matrices; // All the matrices loaded
	static std::multimap<int, int> _difficultyIds; // Ids of matrices sorted by difficulty
	static int _maxDifficulty;
};

#endif
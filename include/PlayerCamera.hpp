#ifndef _PLAYERCAMERA
#define _PLAYERCAMERA

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include "Player.hpp"
#include "glhfAPI.hpp"

#define DURATION_BOOST_ANIM 2.0f

class PlayerCamera {

public:
	PlayerCamera();
	PlayerCamera(glhf::Program prog, Player *player);
	~PlayerCamera();
	void update(double dt);
	void translate(double z);

private:
	glhf::Program _prog;
	GLuint _cameraID;
	glm::mat4 _projection;
	Player *_player;
	glm::vec3 _offsetCamera;
	glm::vec3 _offsetCameraBoost = glm::vec3(0, 0, 0);
	glm::vec3 _offsetCameraNormal = glm::vec3(0, -1, -1);
	double _timeMove = 0;

	void move(double dt);
};

#endif

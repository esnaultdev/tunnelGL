#ifndef _CAMERA
#define _CAMERA

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

class Camera {

public:
	Camera();
	Camera(glhf::Program prog, Player *player);
	~Camera();
	void update(double dt);
	void move(double dt);
private:
	glhf::Program _prog;
	GLuint _cameraID;
	glm::mat4 _projection;
	Player *_player;
	glm::vec3 _offsetCamera;
	glm::vec3 _offsetCameraBoost = glm::vec3(0, 0.1, -0);
	glm::vec3 _offsetCameraNormal = glm::vec3(0, -0.1, -1);
	double _timeMove = 0;
};

#endif
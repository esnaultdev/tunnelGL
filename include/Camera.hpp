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

class Camera {

public:
	Camera();
	Camera(glhf::Program prog, Player *player);
	~Camera();
	void update(double dt);

private:
	glhf::Program _prog;
	GLuint _cameraID;
	glm::mat4 _projection;
	Player *_player;
};

#endif
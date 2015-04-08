#ifndef _CAMERA
#define _CAMERA

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include "glhfAPI.hpp"

class Camera {

public:
	Camera();
	Camera(glhf::Program prog, glm::vec3 pos, double angle);
	~Camera();
	void translate(double z);
	void rotate(double z);

private:
	glhf::Program _prog;

	GLuint _cameraID;
	glm::mat4 _projection;
	double _timeMove = 0;
	glm::vec3 _pos;
	double _angle;

	void updateUniform();
};

#endif

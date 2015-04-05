#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include "utils.h"

/*
 * OpenGL Error checking
 */
void checkGLerror()
{
    GLenum errCode;
    if ((errCode = glGetError()) != GL_NO_ERROR)
        std::cerr<<"OpenGL Error: "<<gluErrorString(errCode)<<std::endl;
}

void displayMat4(const glm::mat4& m) {
	std::cout << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << std::endl 
			  << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << std::endl 
			  << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << std::endl 
			  << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << std::endl;
}



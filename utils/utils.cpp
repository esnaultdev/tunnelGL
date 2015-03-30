#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>

/*
 * OpenGL Error checking
 */
void checkGLerror()
{
    GLenum errCode;
    if ((errCode = glGetError()) != GL_NO_ERROR)
        std::cerr<<"OpenGL Error: "<<gluErrorString(errCode)<<std::endl;
}



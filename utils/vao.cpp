#include <iostream>
#include <GL/glew.h>
#include "utils/utils.h"
#include "utils/vao.h"

GLuint make_buffer( GLenum target, const void *buffer_data, GLsizei buffer_size)
{
    /* Utility function to generate a buffer (for VBO and IBO) *//*{{{*/
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
    return buffer;/*}}}*/
} 

GLuint make_vao()
{ 
    /* Utility function to create a VAO *//*{{{*/
    GLuint vao;
    glGenVertexArrays(1, &vao);
    return vao;/*}}}*/
}


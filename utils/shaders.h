#ifndef __SHADERS_H__
#define __SHADERS_H__ 
#include <string>
GLuint make_program(const std::string& vertexname, const std::string& fragmentname);
void link_program(GLuint program);
#endif // __SHADERS_H__

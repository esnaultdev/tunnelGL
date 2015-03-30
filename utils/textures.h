#ifndef __TEXTURES_H__
#define __TEXTURES_H__ 
GLuint loadTGATexture(const std::string& imagepath, GLenum wrap_s=GL_REPEAT, GLenum wrap_t=GL_REPEAT, GLenum mag_filter=GL_LINEAR, GLenum min_filter=GL_LINEAR_MIPMAP_LINEAR, bool anisotropy=false);
GLuint loadTGACubemap( std::string* faceNames, GLenum* faceTypes);
char *read_tga(const char *filename, int& width, int& height);
#endif // __TEXTURES_H__


#ifndef _PRINTTEXT
#define _PRINTTEXT

void initText2D(const char * texturePath, int nbCharW);
void printText2D(const char * text, int x, int y, int size);
void cleanupText2D();
void getTextProgId(GLuint *idShader, GLuint *idTexture);

#endif
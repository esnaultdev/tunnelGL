#include <iostream>
#include <GL/glew.h>
#include "utils/utils.h"
#include "utils/shaders.h"

static void *file_contents(const char *filename, GLint *length);

static void show_info_log(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)/*{{{*/
{
    GLint log_length;
    char *log;
    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = new char[log_length];
    glGet__InfoLog(object, log_length, NULL, log);
    std::cerr<<log;
    delete[] log;
}/*}}}*/

static GLuint make_shader(GLenum type, const std::string& filename)/*{{{*/
{
    /* Utility function to create a shader object from a file*/
    GLint length;
    GLchar *source = (GLchar*)file_contents(filename.c_str(), &length);
    GLuint shader;
    GLint shader_ok;
    if (!source)
        return 0;
    //
    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&source, &length);
    free(source);
    glCompileShader(shader);
    //
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        std::cerr<<"Failed to compile "<<filename<<":"<<std::endl;
        show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
    } else {
		std::cout << filename << " compiled" << std::endl;
	}
    return shader;
}/*}}}*/

static GLuint make_program(GLuint vertex_shader, GLuint fragment_shader)/*{{{*/
{
    /* Utility function to create a program object from two files*/ 
    GLint program_ok;
    GLuint program = glCreateProgram();
    //
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    //
    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        std::cerr<<"Failed to link shader program:"<<std::endl;
        show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    } else {
		std::cout << "Vertex and fragment shaders linked" << std::endl;	
	}
    return program;
}/*}}}*/


GLuint make_program(const std::string& vertexname, const std::string& fragmentname)
{
    GLuint vertex_shader = make_shader(GL_VERTEX_SHADER, vertexname);
    if (vertex_shader == 0)
        return 0;

    GLuint fragment_shader = make_shader(GL_FRAGMENT_SHADER, fragmentname);
    if (fragment_shader == 0)
        return 0;

    GLuint program = make_program(vertex_shader, fragment_shader);
    if (program == 0)
        return 0;    
    return program;
}

void link_program(GLuint program)/*{{{*/
{
    /* Utility function to create a program object from two files*/ 
    GLint program_ok;
    //
    glLinkProgram(program);
    //
    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        std::cerr<<"Failed to link shader program:"<<std::endl;
        show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
    }
}/*}}}*/
/*
 * Boring, non-OpenGL-related utility functions
 */

static void *file_contents(const char *filename, GLint *length)
{
    FILE *f = fopen(filename, "r");
    void *buffer;

    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    *length = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = malloc(*length+1);
    *length = fread(buffer, 1, *length, f);
    fclose(f);
    ((char*)buffer)[*length] = '\0';

    return buffer;
}



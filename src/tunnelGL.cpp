/* Include headers*/
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include "glhfAPI.hpp"
#include "tunnelGL.hpp"
#include "Engine.hpp"

GLFWwindow* window;
glhf::Program prog;
unsigned int timeUniformID;
Engine* engine;


int main(void) {
    init();
    engine = new Engine(prog);
    mainLoop();
    shutDown(0);
}

void mainLoop(void) {
	window = glfwGetCurrentContext();

    while(!glfwWindowShouldClose(window)){
        double current_time = glfwGetTime();
        glUseProgram(prog.getId());
        glUniform1f(timeUniformID, current_time);

        engine->update(current_time);

        draw();

		glfwSwapBuffers(window);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ) { //boost
			shutDown(0);
			break;
		}

        glfwPollEvents();
    }
}

void init(void) { 
    
	initGLFW();
	initGLEW();
	prog = glhf::Program("simple.v.glsl", "simple.f.glsl");
    timeUniformID = glGetUniformLocation( prog.getId(), "time");

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "We made it! Now to the end of the tunnel..." << std::endl;
}

void initGLFW() {
	
	if (!glfwInit()) shutDown(1);
                                                
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const int window_width = 1024, window_height = 800;
	GLFWwindow* window = glfwCreateWindow(window_width, window_height,
			"TunnelGL", NULL, NULL);
	if (!window) {
		std::cerr << "Could not open a window" << std::endl;
		shutDown(1);
	}

	glfwMakeContextCurrent(window);
	glfwSetTime(0);
}

void initGLEW(){
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();
	if (GlewInitResult != GLEW_OK) {
		std::cerr << "ERROR: " << glewGetErrorString(GlewInitResult) << std::endl;
		shutDown(1);
	}

}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	engine->draw();	

	glUseProgram(0);
}

void shutDown(int return_code) {
    glfwTerminate();
    exit(return_code);
}

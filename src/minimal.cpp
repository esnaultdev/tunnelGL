/* Include headers*/
#include <stdlib.h>
#include <iostream>
#include <stack>
#include <vector>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
// matrix and vectors
// to declare before including glm.hpp, to use the swizzle operators
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
//loading stuffs
#include "utils/utils.h"
#include "utils/shaders.h"
#include "glhfAPI.hpp"
#include "minimal.hpp"
#include "Engine.hpp"

GLFWwindow* window;
glhf::Program prog;
unsigned int timeUniformID;
Engine* engine;

//-------------------- MAIN 
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

        /* Send all the drawing commands and swap the buffers*/
        draw();
        // swap back and front buffers

		glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

//-------------------- Inits
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
	// Create OpenGL 4.4 Core Profile and Forward Compatible Context                                                   
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//           // Create a window of given size
	const int window_width = 1024, window_height = 800;
	GLFWwindow* window = glfwCreateWindow(window_width, window_height,
			"TunnelGL", NULL, NULL);
	if (!window) {
		std::cerr << "Could not open a window" << std::endl;
		shutDown(1);
	}

	// Make the context of the specified window current 
	glfwMakeContextCurrent(window);
	glfwSetTime(0);
	
	/* GLFW callbak definitions */
    glfwSetFramebufferSizeCallback(window, &resize);
}

void initGLEW(){
	/* GLEW Initialization */
	// Do not forget to use glewExperimental so that glewInit works with core
	// profile: http://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
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



//-------------------- GLFW Callbacks

void shutDown(int return_code) {
    glfwTerminate();
    exit(return_code);
}

void resize(GLFWwindow* window, int width, int height) {        
	std::cout << "resize" << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode,  int action, int modifiers) {
	std::cout << "appui touche" << std::endl;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers) {
	std::cout << "clic souris" << std::endl;
}

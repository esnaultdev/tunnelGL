#include <iostream>
#include <sstream>
#include <cmath>
#include <glm/glm.hpp>
#include "EndScreen.hpp"
#include "utils/textures.h"

#include "Engine.hpp"
#include "GameScreen.hpp"
#include "Box.hpp"
#include "Sphere.hpp"
#include "PrintText.hpp"
#include "Tunnel.hpp"

extern Engine* engine;
extern GLFWwindow* window;

EndScreen::EndScreen(glhf::Program prog, int time, int score, int speed){
	_prog = prog;
	_score = score;
	_time = time;
	_speed = speed;
}

EndScreen::~EndScreen(){
	cleanupText2D();
}

void EndScreen::init(){
	initText2D("../resources/font.DDS", 16);
	_idTexture = loadTGATexture("../resources/mainScreen.tga");
	getTextProgId(&_idProgram, &_idUniform);
	makeResources();
}

void EndScreen::update(double dt) {
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS ) { //boost
		engine->setNextScreen(new GameScreen(_prog));
	}
}

void EndScreen::draw(){
	glUseProgram(_idProgram);
	glBindVertexArray(_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _idTexture);
	glUniform1i(_idUniform, 0);

	glDrawArrays(GL_TRIANGLES, 0, _position.size());

	glBindVertexArray(0);
	glUseProgram(0);

	std::ostringstream strs3;
	strs3 << "Press ENTER to try again!";
	printText2D(strs3.str().c_str(), 50, 750, 30);

	std::ostringstream strs;
	strs << "Score: " << _score;
	printText2D(strs.str().c_str(), 50, 550, 30);

	std::ostringstream strs4;
	strs4 << "Speed: " << _speed;
	printText2D(strs4.str().c_str(), 50, 350, 30);

	std::ostringstream strs2;
	int sec = _time % 60;
	strs2 << "Time: " << _time/60 << ":";
	if (sec < 10)
		strs2 << "0" << sec;
	else
		strs2 << sec;
	printText2D(strs2.str().c_str(), 50, 450, 30);
}

void EndScreen::makeResources() {
	std::vector<glm::vec2> uv;

	_position.push_back(glm::vec2(0, 0));
	_position.push_back(glm::vec2(0, 800));
	_position.push_back(glm::vec2(1024, 0));

	_position.push_back(glm::vec2(1024, 0));
	_position.push_back(glm::vec2(0, 800));
	_position.push_back(glm::vec2(1024, 800));

	uv.push_back(glm::vec2(0, 0));
	uv.push_back(glm::vec2(0, 1));
	uv.push_back(glm::vec2(1, 0));

	uv.push_back(glm::vec2(1, 0));
	uv.push_back(glm::vec2(0, 1));
	uv.push_back(glm::vec2(1, 1));

	glUseProgram(_idProgram);
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glGenBuffers(1, &_vertexID);
	glGenBuffers(1, &_uvID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexID);
	glBufferData(GL_ARRAY_BUFFER, _position.size() * sizeof(glm::vec2), &_position[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, _uvID);
	glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _uvID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
	glBindVertexArray(0);
	glUseProgram(0);
}
#include <iostream>
#include <sstream>
#include <cmath>
#include "GameScreen.hpp"

#include "Box.hpp"
#include "Sphere.hpp"
#include "PrintText.hpp"
#include "Tunnel.hpp"

extern GLFWwindow* window;

GameScreen::GameScreen(glhf::Program prog){
	_prog = prog;
}

GameScreen::~GameScreen(){
	cleanupText2D();
}

void GameScreen::init(){
	Box *b = new Box(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0, 1, 1), glm::vec3(1, 0, 1));
	Sphere *s = new Sphere(glm::vec3(-0.5, -0.5, 0), 1);
	std::cout << b->collideWith(s) << std::endl;
	initText2D("../resources/test.DDS", 11);
	_tunnel[0] = Tunnel(_prog);
	_tunnel[0].init(0);

	for (int i = 1; i < NB_TUNNEL; ++i) {
		_tunnel[i] = Tunnel(_prog);
		_tunnel[i].init(_tunnel[i-1].getPosEndZ());
	}

	_player = Player(_prog);

	//Camera
	_projection = glm::perspective(45.0f, 1024.0f/800.0f, 0.1f, 100.0f);
	_cameraID = glGetUniformLocation(_prog.getId(), "viewProjection");
	_lightdirnID = glGetUniformLocation(_prog.getId(), "lightdirn");
}

void GameScreen::update(double dt) {
	_time += dt;
	glm::vec3 offsetCamera(0, 0, -0);

	_player.update(dt, _tunnel[0].getRadius());

	if (_player.getPos().z >= _tunnel[0].getPosEndZ())
		nextTunnel();

	if (_tunnel[0].isHole(_player.getAngle(), _player.getPos().z))
		init();

	glm::vec3 posPlayer = _player.getPos();

	glm::mat4 view = glm::lookAt(
		posPlayer + offsetCamera,
		posPlayer + glm::vec3(0,0,10),
		glm::normalize(glm::vec3(-posPlayer.x,-posPlayer.y,0)));
	_camera = _projection * view;
	glUniformMatrix4fv(_cameraID, 1, GL_FALSE, &_camera[0][0]);

	glm::vec2 light(-posPlayer.x, -posPlayer.y);
	light = glm::normalize(light);
	glUniform3f(_lightdirnID, light.x, light.y, 1);
}

void GameScreen::nextTunnel() {
	Tunnel tunnel = Tunnel(_prog);
	tunnel.init(_tunnel[NB_TUNNEL - 1].getPosEndZ());

	for (int i = 1; i < NB_TUNNEL; ++i) {
		_tunnel[i-1] = _tunnel[i];
	}

	_tunnel[NB_TUNNEL - 1] = tunnel;
}

void GameScreen::draw(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	for (int i = 0; i < NB_TUNNEL; ++i)
		_tunnel[i].draw();

	_player.draw();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	std::ostringstream strs;
	strs << std::floor(_player.getSpeed() * 1000) << "km_h";
	std::string str = strs.str();
	printText2D(strs.str().c_str(), 400, 30, 40);
}
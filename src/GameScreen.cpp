#include <iostream>
#include <sstream>
#include "GameScreen.hpp"

#include "Box.hpp"
#include "Sphere.hpp"
#include "PrintText.hpp"
#include "Tunnel.hpp"

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
	makeTriangle();
	_tunnel[0] = Tunnel(_prog);
	_tunnel[0].init(0);

	for (int i = 1; i < NB_TUNNEL; ++i) {
		_tunnel[i] = Tunnel(_prog);
		_tunnel[i].init(_tunnel[i-1].getPosEndZ());
	}

	_player = Player(_prog);
}

void GameScreen::update(double dt) {
	_time += dt * 100;

	_player.update(dt, _tunnel[0].getRadius());

	if (_player.getPos().z >= _tunnel[NB_TUNNEL - 1].getPosEndZ())
		nextTunnel();
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
	//_triangle.draw();
	
	for (int i = 0; i < NB_TUNNEL; ++i)
		_tunnel[i].draw();

	_player.draw();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	std::ostringstream strs;
	strs << _time;
	std::string str = strs.str();
	printText2D(strs.str().c_str(), 400, 30, 40);
}

void GameScreen::makeTriangle(void){
	std::vector<unsigned int> indices; 	
	std::vector<float> position;
	std::vector<float> color;
	std::vector<float> normal;
	std::vector<float> uv;

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);
	
	position.push_back(-0.2);
	position.push_back(-0.2);
	position.push_back(0.0);

	position.push_back(0.2);
	position.push_back(0.2);
	position.push_back(0.0);

	position.push_back(0.2);
	position.push_back(-0.2);
	position.push_back(0.0);

	color.push_back(1);
	color.push_back(0);
	color.push_back(0);
	color.push_back(1);
	color.push_back(1);
	color.push_back(0);
	color.push_back(1);
	color.push_back(1);
	color.push_back(1);

	for (int i = 0; i < 3; i++) {
		normal.push_back(1);
		normal.push_back(1);
		normal.push_back(1);
		uv.push_back(0);
		uv.push_back(0);
	}

	_triangle = glhf::GLObject(_prog, 3, 3, indices, position, color, normal, uv); 
	_triangle.initVao();
}
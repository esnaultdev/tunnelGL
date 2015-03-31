#include <iostream>
#include "GameScreen.hpp"

#include "Box.hpp"
#include "Sphere.hpp"

GameScreen::GameScreen(glhf::Program prog){
	_prog = prog;
}

GameScreen::~GameScreen(){

}

void GameScreen::init(){
	Box *b = new Box(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0, 1, 1), glm::vec3(1, 0, 1));
	Sphere *s = new Sphere(glm::vec3(-0.5, -0.5, 0), 1);
	std::cout << b->collideWith(s) << std::endl;
	make_triangle();

}

void GameScreen::update(double dt){
	//std::cout << dt << std::endl;
}

void GameScreen::draw(){
	_triangle.draw();
}

void GameScreen::make_triangle(void){
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
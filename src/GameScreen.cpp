#include <iostream>
#include "GameScreen.hpp"

#include "Box.hpp"
#include "Sphere.hpp"

GameScreen::GameScreen(){

}

GameScreen::~GameScreen(){

}

void GameScreen::init(){
	Box *b = new Box(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0, 1, 1), glm::vec3(1, 0, 1));
	Sphere *s = new Sphere(glm::vec3(-0.5, -0.5, 0), 1);
	std::cout << b->collideWith(s) << std::endl;
	std::cout << "init" << std::endl;
}

void GameScreen::update(double dt){
	//std::cout << dt << std::endl;
}

void GameScreen::draw(){
	//std::cout << "drawing" << std::endl;
}
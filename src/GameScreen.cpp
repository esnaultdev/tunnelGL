#include <iostream>
#include "GameScreen.hpp"


GameScreen::GameScreen(){

}

GameScreen::~GameScreen(){

}

void GameScreen::init(){
	std::cout << "init" << std::endl;
}

void GameScreen::update(double dt){
	std::cout << dt << std::endl;
}

void GameScreen::draw(){
	std::cout << "drawing" << std::endl;
}
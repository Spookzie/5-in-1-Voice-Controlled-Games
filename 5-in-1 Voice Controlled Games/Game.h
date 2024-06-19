#pragma once

#include "MainMenu.h"


class Game
{
private:
	//	VARIABLES	//
	//Game Setup
	sf::RenderWindow window;
	sf::Event ev;

	//Main menu
	MainMenu mainMenu;

	//	FUNCTIONS	//
	//Initializers
	void Init_Window();

	//Game setup
	void PollEvents();

	//Main Menu
	void LaunchGame(int game_id);

public:
	//Constructor & Destructor
	Game();
	~Game();

	//Game setup
	const sf::RenderWindow& GetWindow() const {	return this->window; };

	void Update();
	void Render();
};


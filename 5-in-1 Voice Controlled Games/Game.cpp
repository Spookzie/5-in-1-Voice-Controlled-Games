#include "stdafx.h"
#include "Game.h"


void Game::Start()
{
	//	Main menu	//
	if (!this->mmTexture.loadFromFile("Textures/MainMenu/BG.jpg"))
		std::cout << "ERROR::Game.cpp::Start():: Failed to load the main menu background texture." << std::endl;
	
	if (!this->mmButtonTexture.loadFromFile("Textures/MainMenu/Button.png"))
		std::cout << "ERROR::Game.cpp::Start():: Failed to load the main menu button texture." << std::endl;

	this->mmBG.setTexture(this->mmTexture);
	this->mmButton.setTexture(this->mmButtonTexture);
	this->mmButton.setScale(1.5f, 1.f);
}

void Game::Init_Window()
{
	this->window.create(sf::VideoMode(880, 495), "5-in-1 Voice Controlled games");
	this->window.setFramerateLimit(60);
	this->window.setVerticalSyncEnabled(false);
}


void Game::PollEvents()
{
	while (this->window.pollEvent(this->ev))
	{
		if (this->ev.type == sf::Event::Closed)
			this->window.close();
	}
}


//Constructor
Game::Game()
{
	this->Start();
	this->Init_Window();
	this->PollEvents();
}

//Destructor
Game::~Game()
{
}


void Game::Update()
{
	this->PollEvents();
}


void Game::Render()
{
	this->window.clear();

	//	DRAW HERE	//
	this->window.draw(this->mmBG);
	this->window.draw(this->mmButton);

	this->window.display();
}

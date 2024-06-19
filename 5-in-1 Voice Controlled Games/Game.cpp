#include "stdafx.h"
#include "Game.h"


void Game::Start()
{
}

void Game::Init_Window()
{
	this->window.create(sf::VideoMode(1280, 720), "5-in-1 Voice Controlled games");
	this->window.setFramerateLimit(60);
	this->window.setVerticalSyncEnabled(false);
}


void Game::PollEvents()
{
	while (this->window.pollEvent(this->ev))
	{
		if ((this->ev.type == sf::Event::Closed) || (mainMenu.GetButtonPressed() == 5))
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

	mainMenu.Update(this->window);
}


void Game::Render()
{
	this->window.clear();

	//	DRAW HERE	//
	mainMenu.Render(this->window);

	this->window.display();
}

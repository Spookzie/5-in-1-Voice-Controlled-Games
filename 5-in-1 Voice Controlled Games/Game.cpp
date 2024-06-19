#include "stdafx.h"
#include "Game.h"


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
		if (this->ev.type == sf::Event::Closed)
			this->window.close();
	}
}


void Game::LaunchGame(int game_id)
{
	std::string command;
    switch (game_id)
    {
    case 0:
        command = "DoodleJump\\x64\\Debug\\DoodleJump.exe";
        break;
    case 1:
        command = "\"path/to/arkanoid_executable.exe\"";
        break;
    case 2:
        command = "\"path/to/snake_executable.exe\"";
        break;
    case 3:
        command = "\"path/to/car_racing_executable.exe\"";
        break;
    case 4:
        command = "\"path/to/xonix_executable.exe\"";
        break;
    case 5:
        this->window.close();
        return;
    default:
        return;
    }

    this->window.close();
    std::system(command.c_str());
    this->Init_Window();
}


//Constructor
Game::Game()
{
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

    //Game launching functionality
    if (mainMenu.buttonID != 50)
    {
        this->LaunchGame(mainMenu.buttonID);
        mainMenu.buttonID = 50;
    }
}


void Game::Render()
{
	this->window.clear();

	//	DRAW HERE	//
	mainMenu.Render(this->window);

	this->window.display();
}

#include "stdafx.h"
#include "MainMenu.h"


void MainMenu::Start()
{
	this->buttonID = 50;
}

void MainMenu::Init_Resources()
{
	//	Background	//
	if (!this->bgTexture.loadFromFile("Textures/MainMenu/BG1.jpg"))
		std::cout << "ERROR::MainMenu.cpp::Init_Resources():: Failed to load the main menu background texture." << std::endl;

	this->bgSprite.setTexture(this->bgTexture);

	
	//	Button	//
	if (!this->buttonTexture.loadFromFile("Textures/MainMenu/Button2.png"))
		std::cout << "ERROR::MainMenu.cpp::Init_Resources():: Failed to load the main menu button texture." << std::endl;

	this->buttonSprite.setTexture(this->buttonTexture);
	this->buttonSprite.setScale(2.5f, 1.25f);
	for (int i = 0; i < 6; i++)
	{
		this->buttons.push_back(this->buttonSprite);
		this->buttons[i].setPosition(900.f, 40.f + i * 80.f);
	}


	//	Font	//
	if (!this->font.loadFromFile("Fonts/Ojuju-Bold.ttf"))
		std::cout << "ERROR::MainMenu.cpp::Init_Resources():: Failed to load the font." << std::endl;

	//Doodle Jump (Button 1 [0])
	this->doodleJump.setFont(this->font);
	this->doodleJump.setCharacterSize(32);
	this->doodleJump.setFillColor(sf::Color::Black);
	this->doodleJump.setString("Doodle Jump");
	this->doodleJump.setPosition(965.f, 50.f);

	//Arkanoid (Button 2 [1])
	this->arkanoid.setFont(this->font);
	this->arkanoid.setCharacterSize(32);
	this->arkanoid.setFillColor(sf::Color::Black);
	this->arkanoid.setString("Arkanoid");
	this->arkanoid.setPosition(1000.f, 130.f);
	
	//Snake (Button 3 [2])
	this->snake.setFont(this->font);
	this->snake.setCharacterSize(32);
	this->snake.setFillColor(sf::Color::Black);
	this->snake.setString("Snake");
	this->snake.setPosition(1015.f, 210.f);
	
	//Car racing (Button 4 [3])
	this->carRacing.setFont(this->font);
	this->carRacing.setCharacterSize(32);
	this->carRacing.setFillColor(sf::Color::Black);
	this->carRacing.setString("Car Racing");
	this->carRacing.setPosition(980.f, 290.f);
	
	//Xonix (Button 5 [4])
	this->xonix.setFont(this->font);
	this->xonix.setCharacterSize(32);
	this->xonix.setFillColor(sf::Color::Black);
	this->xonix.setString("Xonix");
	this->xonix.setPosition(1015.f, 370.f);
	
	//Quit (Button 6 [5])
	this->quit.setFont(this->font);
	this->quit.setCharacterSize(32);
	this->quit.setFillColor(sf::Color::Black);
	this->quit.setString("Quit");
	this->quit.setPosition(1025.f, 450.f);

}


MainMenu::MainMenu()
{
	this->Start();
	this->Init_Resources();
}


void MainMenu::IsMouseHovering(sf::RenderWindow& window)
{
	this->buttonID = 50;

	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	for (int i = 0; i < this->buttons.size(); i++)
	{
		if (this->buttons[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				this->buttonID = i;
		}
	}
}


void MainMenu::Update(sf::RenderWindow& window)
{
	this->IsMouseHovering(window);
	//this->ApplicationSwitching(window);
	std::cout << this->buttonID << std::endl;
}


void MainMenu::Render(sf::RenderTarget& target)
{
	//Background
	target.draw(this->bgSprite);

	//Buttons
	for (int i=0; i<buttons.size(); i++)
		target.draw(this->buttons[i]);

	//Button Texts
	target.draw(this->doodleJump);
	target.draw(this->arkanoid);
	target.draw(this->snake);
	target.draw(this->carRacing);
	target.draw(this->xonix);
	target.draw(this->quit);
}
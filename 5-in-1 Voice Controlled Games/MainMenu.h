#pragma once


class MainMenu
{
private:
	//	VARIABLES	//
	//Textures & sprites
	sf::Texture bgTexture, buttonTexture;
	sf::Sprite bgSprite, buttonSprite;
	std::vector<sf::Sprite> buttons;

	//Font
	sf::Font font;
	sf::Text doodleJump, arkanoid, snake, asteroidShooter, xonix, quit;

	//	FUNCTIONS	//
	//Initializers
	void Start();
	void Init_Resources();

	//Interaction
	void IsMouseHovering(sf::RenderWindow& window);


public:
	//	VARIABLES	//
	//Interaction
	int buttonID;

	//	FUNCTIONS	//
	//Constructor
	MainMenu();

	void Update(sf::RenderWindow& window);
	void Render(sf::RenderTarget& target);
};
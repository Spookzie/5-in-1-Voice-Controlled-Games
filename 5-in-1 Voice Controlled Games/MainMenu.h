#pragma once


class MainMenu
{
private:
	//	VARIABLES	//
	//Textures & sprites
	sf::Texture bgTexture;
	sf::Texture buttonTexture;
	sf::Sprite bgSprite;
	sf::Sprite buttonSprite;
	std::vector<sf::Sprite> buttons;

	//Font
	sf::Font font;
	sf::Text doodleJump;
	sf::Text arkanoid;
	sf::Text snake;
	sf::Text carRacing;
	sf::Text xonix;
	sf::Text quit;

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
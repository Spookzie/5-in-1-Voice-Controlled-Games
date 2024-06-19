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

	//Interaction
	int buttonID;
	/*
	Doodle Jump = 0
	Arkanoid	= 1
	Snake		= 2
	Car Racing	= 3
	Xonix		= 4
	Quit		= 5
	*/

	//	FUNCTIONS	//
	//Initializers
	void Start();
	void Init_Resources();

	//Interaction
	void IsMouseHovering(sf::RenderWindow& window);

public:
	//Constructor
	MainMenu();

	const int GetButtonPressed() const { return this->buttonID; };

	void Update(sf::RenderWindow& window);
	void Render(sf::RenderTarget& target);
};
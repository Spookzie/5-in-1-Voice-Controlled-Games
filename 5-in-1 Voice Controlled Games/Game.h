#pragma once


class Game
{
private:
	//	VARIABLES	//
	//Game Setup
	sf::RenderWindow window;
	sf::Event ev;

	//Main Menu
	sf::Texture mmTexture;
	sf::Texture mmButtonTexture;
	sf::Sprite mmBG;
	sf::Sprite mmButton;

	//	FUNCTIONS	//
	//Initializers
	void Start();
	void Init_Window();

	//Game setup
	void PollEvents();

public:
	//Constructor & Destructor
	Game();
	~Game();

	//Game setup
	const sf::RenderWindow& GetWindow() const {	return this->window; };

	void Update();
	void Render();
};


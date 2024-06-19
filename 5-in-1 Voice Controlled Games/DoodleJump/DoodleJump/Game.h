#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>


struct PlatformCoordinates { int x, y; };


class Game
{
private:
	//	VARIABLES	//
	//Game Setup
	sf::RenderWindow window;
	sf::Event ev;
	bool hasEnded;

	int points;

	//Resources
	sf::Texture bgTexture, platformTexture, doodleTexture, oopsTexture;
	sf::Sprite bg, platform, doodle, oops;
	sf::Font font;
	sf::Text pointsText, gameOverText;
	sf::Music music;
	sf::SoundBuffer bounceBuffer;
	sf::Sound bounceSound;

	// Player
	int doodleX, doodleY, doodleH;
	float doodleDY;

	// Platforms
	std::vector<PlatformCoordinates> platforms;

	//	FUNCTIONS	//
	//Initializers
	void Init_Window();
	void Init_Resources();
	void Start();

	//Game setup
	void PollEvents();

	//Game Control
	void PlayerMovement();
	void ScreenScrolling();
	void PlatformBouncing();
	void UpdatePointsText();

public:
	//Constructor & Destructor
	Game();

	//Game setup
	const sf::RenderWindow& GetWindow() const { return this->window; };


	void Update();
	void Render();
};
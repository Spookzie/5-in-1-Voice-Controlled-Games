#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <WinSock2.h>


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
	bool isMovingLeft, isMovingRight;

	// Platforms
	std::vector<PlatformCoordinates> platforms;

	//Socket Communication
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	std::thread socketThread;

	//	FUNCTIONS	//
	//Initializers
	void Init_Window();
	void Init_Resources();
	void Init_Socket();
	void Start();

	//Game setup
	void PollEvents();

	//Game Control
	void PlayerMovement(const std::string& command = "");
	void ScreenScrolling();
	void PlatformBouncing();
	void UpdatePointsText();

	//Socket Communication
	void SocketListener();

public:
	//Constructor & Destructor
	Game();
	virtual ~Game();

	//Game setup
	const sf::RenderWindow& GetWindow() const { return this->window; };


	void Update();
	void Render();
};
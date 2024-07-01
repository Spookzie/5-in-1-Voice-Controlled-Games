#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <string>
#include <sstream>
#include <SFML/Audio.hpp>
#include <thread>
#include <winsock2.h>

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"


class Game
{
private:
	//	VARIABLES	//
	//Game Control
	sf::RenderWindow* window;
	bool hasEnded;

	//GUI
	sf::Font font;
	sf::Text pointsText;
	float points;
	sf::Text endGameText;

	//World
	sf::Texture bgTexture;
	sf::Sprite bg;
	sf::Music music;
	sf::SoundBuffer shootBuffer, explosionBuffer;
	sf::Sound shootSound, explosionSound;

	//Player
	Player* player;
	sf::RectangleShape playerHealthBar;
	sf::RectangleShape playerHealthBarBG;
	bool isMovingLeft, isMovingRight;

	//Bullet
	std::map<std::string, sf::Texture*> texturesMap;
	std::vector<Bullet*> bulletsVector;
	float spawnTimer;
	float spawnTImerMax;

	//Enemies
	std::vector<Enemy*> enemiesVector;

	//Socket Communication
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	std::thread socketThread;

	//	FUNCTIONS	//
	//Initializers
	void Init_Window();
	void Init_TexturesMap();
	void Init_GUI();
	void Init_World();
	void Init_Player();
	void Init_Enemies();
	void Init_Socket();

	//Socket Communication
	void SocketListener();

public:
	//Constructor & Destructor
	Game();
	virtual ~Game();

	//	Game Loop	//
	//Getter
	const bool IsRunning() const;

	//Update Function
	void UpdatePollEvents();
	void UpdateInput(const std::string& command = "");
	void UpdateGUI();
	void LimitPlayerMovement();
	void UpdateBullets();
	void UpdateEnemies();
	void UpdatePlayer();
	void Update();

	//Render Function
	void RenderGUI();
	void RenderWorld();
	void Render();
};
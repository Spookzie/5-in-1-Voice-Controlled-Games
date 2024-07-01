#include "Game.h"


//Window Initializer Function
void Game::Init_Window()
{
	this->window = new sf::RenderWindow(sf::VideoMode(700, 900), "Asteroid Shooter");
	this->window->setFramerateLimit(60);
	this->window->setVerticalSyncEnabled(false);
}


//Texture Map Initializer Function
void Game::Init_TexturesMap()
{
	this->texturesMap["Bullet"] = new sf::Texture();
	this->texturesMap["Bullet"]->loadFromFile("Textures/AsteroidShooter/Bullet.png");
}


//GUI Initializer Function
void Game::Init_GUI()
{
	//	Points	//
	if (!this->font.loadFromFile("Fonts/Ojuju-SemiBold.ttf"))
		std::cout << "ERROR::Game.cpp::Init_GUI():: Failed to load the font." << std::endl;

	this->pointsText.setFont(this->font);
	this->pointsText.setCharacterSize(32);
	this->pointsText.setFillColor(sf::Color::White);

	this->points = 0;

	//	Health Bar	//
	this->playerHealthBar.setSize(sf::Vector2f(300.f, 30.f));
	this->playerHealthBar.setFillColor(sf::Color(255, 0, 0, 190));
	this->playerHealthBar.setPosition(395.f, 4.f);

	this->playerHealthBarBG = this->playerHealthBar;
	this->playerHealthBarBG.setOutlineThickness(2.f);
	this->playerHealthBarBG.setFillColor(sf::Color::White);

	//	End Game Text	//
	this->endGameText.setFont(this->font);
	this->endGameText.setFillColor(sf::Color::Red);
	this->endGameText.setCharacterSize(108);
	this->endGameText.setPosition(sf::Vector2f(75.f, 350.f));
	this->endGameText.setString("Game Over!");
}


//World Attributes Initializer Function
void Game::Init_World()
{
	this->hasEnded = false;

	//	Background	//
	if (!this->bgTexture.loadFromFile("Textures/AsteroidShooter/BG.jpg"))
		std::cout << "ERROR::Game.cpp::Init_World():: Failed to load the background texture." << std::endl;

	this->bg.setTexture(this->bgTexture);

	//	Music	//
	if (!this->music.openFromFile("SFX/AsteroidShooter/Tank.wav"))
		std::cout << "ERROR::Game.cpp::Init_World():: Failed to load the music." << std::endl;

	this->music.setVolume(2.f);
	this->music.setLoop(true);
	this->music.play();

	//	SFX	//
	//Shoot
	if(!this->shootBuffer.loadFromFile("SFX/AsteroidShooter/LaserShoot.wav"))
		std::cout << "ERROR::Game.cpp::Init_World():: Failed to load the shoot sound." << std::endl;

	this->shootSound.setBuffer(this->shootBuffer);
	this->shootSound.setVolume(1.f);

	//Explosion
	if (!this->explosionBuffer.loadFromFile("SFX/AsteroidShooter/Retro.wav"))
		std::cout << "ERROR::Game.cpp::Init_World():: Failed to load the explosion sound." << std::endl;

	this->explosionSound.setBuffer(this->explosionBuffer);
	this->explosionSound.setVolume(10.f);
}


//Player Initializer Function
void Game::Init_Player()
{
	this->player = new Player();

	this->player->SetPos(315.f, this->window->getSize().y);

	this->isMovingLeft = this->isMovingRight = false;
}


//Enemies Initializaer Function
void Game::Init_Enemies()
{
	this->spawnTImerMax = 30.f;
	this->spawnTimer = this->spawnTImerMax;
}


void Game::Init_Socket()
{
	WSADATA wsaData;

	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
		exit(1);
	}

	// Create a socket for listening for incoming connection requests.
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
		WSACleanup();
		exit(1);
	}

	// Bind the socket.
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(65432);
	if (bind(ListenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// Listen on the socket.
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "Error at listen(): " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// Accept a client socket.
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// Start socket listener thread.
	socketThread = std::thread(&Game::SocketListener, this);
}


void Game::SocketListener()
{
	char recvbuf[512];
	int iResult;

	while (true) {
		iResult = recv(ClientSocket, recvbuf, 512, 0);
		if (iResult > 0) {
			std::string command(recvbuf, iResult);
			std::cout << "Received command: " << command << std::endl;
			UpdateInput(command); // Process the received command
		}
		else if (iResult == 0) {
			std::cout << "Connection closed" << std::endl;
			break;
		}
		else {
			std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
			break;
		}
	}

	closesocket(ClientSocket);
}


//Constructor
Game::Game()
{
	this->Init_Window();
	this->Init_TexturesMap();
	this->Init_GUI();
	this->Init_World();
	this->Init_Player();
	this->Init_Enemies();
	this->Init_Socket();
}


//Destructor
Game::~Game()
{
	delete this->window;
	delete this->player;

	//Deleting Textures
	for (auto& i : this->texturesMap)
		delete i.second;

	//Deleting Bullets
	for (auto* i : this->bulletsVector)
		delete i;

	//Deleting Enemies
	for (auto* i : this->enemiesVector)
		delete i;

	//Close socket
	closesocket(ClientSocket);
	closesocket(ListenSocket);
	WSACleanup();

	//Stop socket thread
	if (socketThread.joinable())
		socketThread.join();
}


//Check if game application is open or game has not ended
const bool Game::IsRunning() const
{
	return this->window->isOpen();
}


//Event Polling Function
void Game::UpdatePollEvents()
{
	sf::Event e;
	while (this->window->pollEvent(e))
	{
		if (e.Event::type == sf::Event::Closed)
			this->window->close();
	}
}


//Input Updating Function
void Game::UpdateInput(const std::string& command)
{
	//Enabling & disabling flags on different inputs
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) || command == "left")
	{
		this->isMovingLeft = true;
		this->isMovingRight = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || command == "right")
	{
		this->isMovingRight = true;
		this->isMovingLeft = false;
	}

	//Continuous movement based on flags	
	if (this->isMovingLeft)
		this->player->Move(-1.f, 0.f);
	else if (this->isMovingRight)
		this->player->Move(1.f, 0.f);

	//Shooting
	if (this->player->CanAttack())
	{
		this->bulletsVector.push_back(new Bullet(this->texturesMap["Bullet"],		//Texture
			this->player->GetPos().x + 15.f, this->player->GetPos().y - 25.f,		//Spawn Position
			0.f, -1.f, 5.f));														//Direction & Speed

		this->shootSound.play();
	}
}


//GUI Update Function
void Game::UpdateGUI()
{
	//	Points	//
	std::stringstream ssPoints;
	ssPoints << "Points: " << this->points;
	this->pointsText.setString(ssPoints.str());

	//	Health Bar	//
	float hpPercent = static_cast<float>(this->player->GetHP()) / this->player->GetHPMax();
	this->playerHealthBar.setSize(sf::Vector2f(300.f * hpPercent, this->playerHealthBar.getSize().y));
}


//Limiting Player movements so he doesn't move out of the screen
void Game::LimitPlayerMovement()
{
	//Left/Right Collision
	if (this->player->GetBounds().left <= 0.f)
		this->player->SetPos(0.f, this->player->GetBounds().top);

	else if ((this->player->GetBounds().left + this->player->GetBounds().width) >= this->window->getSize().x)
		this->player->SetPos(this->window->getSize().x - this->player->GetBounds().width, this->player->GetBounds().top);

	//Top/Bottom Collision
	if (this->player->GetBounds().top <= 0.f)
		this->player->SetPos(this->player->GetBounds().left, 0.f);

	else if ((this->player->GetBounds().top + this->player->GetBounds().height) >= this->window->getSize().y)
		this->player->SetPos(this->player->GetBounds().left, this->window->getSize().y - this->player->GetBounds().height);
}


//Bullets Updating Function
void Game::UpdateBullets()
{
	unsigned bulletCount = 0;

	for (auto* bullet : this->bulletsVector)
	{
		bullet->Update();

		//Bullet Culling (top of screen)
		if ((bullet->GetBounds().top + bullet->GetBounds().height) < 0.f)
		{
			delete bulletsVector.at(bulletCount);	//This deletes the bullet
			this->bulletsVector.erase(this->bulletsVector.begin() + bulletCount);	//This removes the bullet from the vector
			--bulletCount;
		}

		++bulletCount;
	}
}


//Enemies Update Function
void Game::UpdateEnemies()
{
	//Spawning Enemies
	this->spawnTimer += 0.5f;

	if (this->spawnTimer >= this->spawnTImerMax)
	{
		this->enemiesVector.push_back(new Enemy(rand() % this->window->getSize().x - 20.f, -100.f));
		this->spawnTimer = 0.f;
	}

	//Killing Enemies on different conditions
	for (int i = 0; i < this->enemiesVector.size(); ++i)
	{
		this->enemiesVector[i]->Update();

		//When enemies get shot
		for (size_t k = 0; k < this->bulletsVector.size(); k++)
		{
			if (this->bulletsVector[k]->GetBounds().intersects(this->enemiesVector[i]->GetBounds()))
			{
				this->points += this->enemiesVector[i]->GetPoints();

				this->enemiesVector[i]->Explode();
				this->explosionSound.play();

				delete this->bulletsVector[k];
				this->bulletsVector.erase(this->bulletsVector.begin() + k);

			}
		}

		//When enemies run out of screen
		if (this->enemiesVector[i]->GetBounds().top > this->window->getSize().y)
		{
			this->points -= this->enemiesVector[i]->GetPoints();

			this->player->TakeDamage(this->enemiesVector[i]->GetDamage());

			delete this->enemiesVector[i];
			this->enemiesVector.erase(this->enemiesVector.begin() + i);
		}

		//Delete enemies after explosion animation has been played
		if (enemiesVector[i]->isDead())
		{
			delete this->enemiesVector[i];
			this->enemiesVector.erase(this->enemiesVector.begin() + i);
		}
	}
}


//Update Player
void Game::UpdatePlayer()
{
	if (this->player->GetHP() <= 0)
		this->hasEnded = true;
}


//Update Function
void Game::Update()
{
	this->UpdatePollEvents();

	if (!this->hasEnded)
	{
		this->UpdatePlayer();

		this->UpdateInput();

		this->player->Update();

		this->LimitPlayerMovement();

		this->UpdateBullets();

		this->UpdateEnemies();

		this->UpdateGUI();
	}
}


//GUI Rendering Function
void Game::RenderGUI()
{
	this->window->draw(this->pointsText);
	this->window->draw(this->playerHealthBarBG);
	this->window->draw(this->playerHealthBar);
}


//World Attributes Rendering Function
void Game::RenderWorld()
{
	this->window->draw(this->bg);
}


//Render Function
void Game::Render()
{
	//Clearing previous frame
	this->window->clear();

	//	Draw Rendering stuff here	//
	this->RenderWorld();

	this->player->Render(*this->window);

	for (auto* bullet : this->bulletsVector)
		bullet->Render(*this->window);

	for (auto* enemy : this->enemiesVector)
		enemy->Render(*this->window);

	this->RenderGUI();

	if (this->hasEnded)
		this->window->draw(this->endGameText);

	//Displaying what we drew
	this->window->display();
}
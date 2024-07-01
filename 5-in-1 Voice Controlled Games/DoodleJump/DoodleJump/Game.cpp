#include "Game.h"


void Game::Init_Window()
{
	this->window.create(sf::VideoMode(600, 800), "Doodle Jump");
	this->window.setFramerateLimit(60);
	this->window.setVerticalSyncEnabled(false);
}


void Game::Init_Resources()
{
	//	VFX	//
	//Background
	if (!this->bgTexture.loadFromFile("Textures/DoodleJump/BG.png"))
		std::cout << "ERROR::Game.cpp::Init_Resources(): Failed to load background texture" << std::endl;
	
	this->bg.setTexture(this->bgTexture);
	this->bg.setScale(1.5f, 1.5f);
	
	//Platform
	if (!this->platformTexture.loadFromFile("Textures/DoodleJump/Platform.png"))
		std::cout << "ERROR::Game.cpp::Init_Resources(): Failed to load platform texture" << std::endl;
	
	this->platform.setTexture(this->platformTexture);
	this->platform.setScale(1.25f, 1.25f);

	//Doodle
	if (!this->doodleTexture.loadFromFile("Textures/DoodleJump/Doodle.png"))
		std::cout << "ERROR::Game.cpp::Init_Resources(): Failed to load doodle texture" << std::endl;
	
	this->doodle.setTexture(this->doodleTexture);

	//UI text
	if(!this->font.loadFromFile("Fonts/ReenieBeanie-Regular.ttf"))
		std::cout << "ERROR::Game.cpp::Init_Resources(): Failed to load font" << std::endl;

	this->pointsText.setFont(this->font);
	this->pointsText.setFillColor(sf::Color::Black);
	this->pointsText.setCharacterSize(48);

	this->gameOverText.setFont(this->font);
	this->gameOverText.setFillColor(sf::Color::Red);
	this->gameOverText.setCharacterSize(196);
	this->gameOverText.setString("Game\nOver!");
	this->gameOverText.setPosition(125.f, 150.f);

	//Oops
	if (!this->oopsTexture.loadFromFile("Textures/DoodleJump/Oops.png"))
		std::cout << "ERROR::Game.cpp::Init_Resources(): Failed to load oops texture" << std::endl;

	this->oops.setTexture(this->oopsTexture);
	this->oops.setScale(1.5f, 1.5f);


	//	SFX	//
	//Music
	if (!this->music.openFromFile("SFX/DoodleJump/HotAirBalloonFlight.wav"))
		std::cout << "ERROR::Game.cpp::Init_Resources(): Failed to load music" << std::endl;

	this->music.setVolume(2.f);
	this->music.setLoop(true);
	this->music.play();

	//Boing
	if (!this->bounceBuffer.loadFromFile("SFX/DoodleJump/Springboard.wav"))
		std::cout << "ERROR::Game.cpp::Init_Resources(): Failed to load bounceSound sound effect" << std::endl;

	this->bounceSound.setBuffer(this->bounceBuffer);
	this->bounceSound.setPitch(3.f);
	this->bounceSound.setVolume(40.f);
}


void Game::Init_Socket()
{
	WSADATA wsaData;

	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
		exit(1);
	}

	// Create a socket for listening for incoming connection requests.
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET)
	{
		std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
		WSACleanup();
		exit(1);
	}

	// Bind the socket.
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(65432);
	if (bind(ListenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// Listen on the socket.
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "Error at listen(): " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// Accept a client socket.
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// Start socket listener thread.
	socketThread = std::thread(&Game::SocketListener, this);
}


void Game::Start()
{
	//Initialize Player
	this->doodleX = 300;
	this->doodleY = 200;
	this->doodleH = 200;
	this->doodleDY = 0;
	this->isMovingLeft = this->isMovingRight = false;

	// Initialize platforms
	this->platforms.resize(13);
	for (auto& platform : this->platforms)
	{
		platform.x = rand() % 600;
		platform.y = rand() % 800;
	}

	//Initialize points
	this->points = 0;

	//Game Setup
	this->hasEnded = false;
}


void Game::PollEvents()
{
	while (this->window.pollEvent(this->ev))
	{
		if (this->ev.type == sf::Event::Closed)
			this->window.close();
	}
}


//Player input & gravity
void Game::PlayerMovement(const std::string& command)
{
	//	Input	//
	//Enabling & disabling flags on different inputs
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) || command == "right")
	{
		this->isMovingLeft = false;
		this->isMovingRight = true;
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) || command == "left")
	{
		this->isMovingLeft = true;
		this->isMovingRight = false;
	}

	//Continuous movement based on flags	
	if (this->isMovingRight)
		this->doodleX += 5;
	else if (this->isMovingLeft)
		this->doodleX -= 5;

	//	Gravity	//
	this->doodleDY += 0.1f;
	this->doodleY += this->doodleDY;

	// Check for collision with the bottom of the screen
	if (doodleY > this->window.getSize().y)
	{
		this->oops.setPosition(this->doodleX, this->doodle.getGlobalBounds().top - this->oops.getGlobalBounds().height);
		this->hasEnded = true;
	}

	// Check if player is out of screen (horizontally)
	if ((this->doodle.getGlobalBounds().left + this->doodle.getGlobalBounds().width) < 0)
		this->doodleX = this->window.getSize().x;

	if (this->doodle.getGlobalBounds().left > this->window.getSize().x)
		this->doodleX = -this->doodle.getGlobalBounds().width;

	// Set doodle position
	doodle.setPosition(doodleX, doodleY);
}


//Scroll platforms and player if jumping up
void Game::ScreenScrolling()
{
	if (this->doodleY < this->doodleH)
	{
		this->doodleY = this->doodleH;
		for (auto& platform : this->platforms)
		{
			platform.y -= static_cast<int>(this->doodleDY);
			if (platform.y > this->window.getSize().y)
			{
				platform.y = 0;
				platform.x = rand() % 600;
			}
		}
	}
}


//Check for collision with platforms
void Game::PlatformBouncing()
{
	for (const auto& platform : platforms)
	{
		if ((doodleX + 50 > platform.x) && (doodleX + 20 < platform.x + 68) &&
			(doodleY + 70 > platform.y) && (doodleY + 70 < platform.y + 14) &&
			(doodleDY > 0))
		{
			doodleDY = -11;
			this->points += 10;
			this->bounceSound.play();
		}
	}
}


void Game::UpdatePointsText()
{
	std::stringstream ssPoints;
	ssPoints << "Points: " << this->points;
	this->pointsText.setString(ssPoints.str());
}


void Game::SocketListener()
{
	char recvbuf[512];
	int iResult;

	while (true)
	{
		iResult = recv(ClientSocket, recvbuf, 512, 0);
		if (iResult > 0)
		{
			std::string command(recvbuf, iResult);
			std::cout << "Received command: " << command << std::endl;
			this->PlayerMovement(command); // Process the received command
		}
		else if (iResult == 0)
		{
			std::cout << "Connection closed" << std::endl;
			break;
		}
		else
		{
			std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
			break;
		}
	}

	closesocket(ClientSocket);
}


Game::Game()
{
	this->Init_Window();
	this->Init_Resources();
	this->Init_Socket();
	this->Start();
}


Game::~Game()
{
	//Close socket
	closesocket(ClientSocket);
	closesocket(ListenSocket);
	WSACleanup();

	//Stop socket thread
	if (socketThread.joinable())
		socketThread.join();
}


void Game::Update()
{
	this->PollEvents();

	if(!this->hasEnded)
	{
		this->PlayerMovement();

		this->ScreenScrolling();
		
		this->PlatformBouncing();

		this->UpdatePointsText();
	}
}


void Game::Render()
{
	this->window.clear();

	//Drawing background
	this->window.draw(this->bg);
	
	//Drawing platforms
	for (const auto& platform : platforms)
	{
		this->platform.setPosition(platform.x, platform.y);
		this->window.draw(this->platform);
	}

	//Drawing Points
	this->window.draw(this->pointsText);

	//Drawing doodle
	this->window.draw(this->doodle);

	//Drawing game over text
	if (this->hasEnded)
	{
		this->window.draw(this->oops);		//Drawing Oops where doodle fell
		this->window.draw(this->gameOverText);
		this->music.stop();
	}

	this->window.display();
}
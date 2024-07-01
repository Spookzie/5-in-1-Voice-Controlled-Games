#include "game.h"


void Game::Init_Window()
{
    this->window.create(sf::VideoMode(1250, 862), "Arkanoid");
    this->window.setFramerateLimit(60);
    this->window.setVerticalSyncEnabled(false);
}


void Game::Init_Resources()
{
    //  VFX //
    //Background
    if (!this->bgTexture.loadFromFile("Textures/Arkanoid/BG.jpg"))
        std::cout << "ERROR::Game.cpp::Init_Resources():: Failed to load background texture." << std::endl;
    
    this->bgSprite.setTexture(this->bgTexture);
    this->bgSprite.setScale(1.25f, 1.25f);

    //Ball
    if (!this->ballTexture.loadFromFile("Textures/Arkanoid/Ball.png"))
        std::cout << "ERROR::Game.cpp::Init_Resources():: Failed to load ball texture." << std::endl;

    this->ballSprite.setTexture(this->ballTexture);
    this->ballSprite.setScale(1.25f, 1.25f);
    this->ballSprite.setColor(sf::Color::Yellow);

    //Paddle
    if (!this->paddleTexture.loadFromFile("Textures/Arkanoid/Paddle.png"))
        std::cout << "ERROR::Game.cpp::Init_Resources():: Failed to load paddle texture." << std::endl;
    
    this->paddleSprite.setTexture(this->paddleTexture);
    this->paddleSprite.setScale(0.7f, 0.6f);
    this->paddleSprite.setPosition(550.f, 800.f);

    //Blocks
    if (!this->blockTexture.loadFromFile("Textures/Arkanoid/Block01.png"))
        std::cout << "ERROR::Game.cpp::Init_Resources():: Failed to load block texture." << std::endl;

    for (int i = 1; i <= 17; i++)
    {
        for (int j = 1; j <= 12; j++)   
        {
            sf::Sprite block(this->blockTexture);
            block.setPosition(i * 67, j * 30);
            block.setScale(1.3f, 1.2f);
            this->blocks.push_back(block);
        }
    }

    //UI Text
    if(!this->font.loadFromFile("Fonts/Oxanium-Light.ttf"))
        std::cout << "ERROR::Game.cpp::Init_Resources():: Failed to load font." << std::endl;

    this->endText.setFont(this->font);
    this->endText.setCharacterSize(132);
    this->endText.setFillColor(sf::Color::Red);
    this->endText.setPosition(300.f, 300.f);


    //  SFX //
    //Music
    if(!this->music.openFromFile("SFX/Arkanoid/Sprites.wav"))
        std::cout << "ERROR::Game.cpp::Init_Resources():: Failed to load music." << std::endl;

    this->music.setVolume(2.f);
    this->music.setLoop(true);
    this->music.play();

    //Block break
    if (!this->breakBuffer.loadFromFile("SFX/Arkanoid/DistantRevolver.wav"))
        std::cout << "ERROR::Game.cpp::Init_Resources():: Failed to load block breaking sound effect." << std::endl;

    this->breakSound.setBuffer(this->breakBuffer);
    this->breakSound.setVolume(4.f);
    this->breakSound.setPitch(4.f);
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
    this->ballDX = rand() % 13 - 6;
    this->ballDY = 4.f;
    this->ballX = 620.f;
    this->ballY = 700.f;
    this->paddleDX = 10.f;
    this->blockCount = this->blocks.size();
    this->hasEnded = false;
    this->isMovingLeft = this->isMovingRight = false;
}


void Game::PollEvents()
{
    while (this->window.pollEvent(this->ev))
    {
        if (this->ev.type == sf::Event::Closed)
            this->window.close();
    }
}


void Game::CheckCollisions()
{
    //  Ball collision with blocks    //
    for (auto& block : this->blocks)
    {
        if (sf::FloatRect(this->ballX + 3, this->ballY + 3, 6, 6).intersects(block.getGlobalBounds()))
        {
            block.setPosition(-100, 0);
            this->ballDY = -this->ballDY;
            this->blockCount--;
            this->breakSound.play();
        }
    }
    for (auto& block : this->blocks)
    {
        if (sf::FloatRect(this->ballX + 3, this->ballY + 3, 6, 6).intersects(block.getGlobalBounds()))
        {
            block.setPosition(-100, 0);
            this->ballDX = -this->ballDX;
            this->blockCount--;
            this->breakSound.play();
        }
    }


    //  Ball collision with paddle    //
    if (sf::FloatRect(this->ballX + 3, this->ballY + 3, 6, 6).intersects(this->paddleSprite.getGlobalBounds()))
        this->ballDY = rand() % 3 + 4;


    //  Ball collision with screen   //
    //Side 
    if (this->ballX < 0 || this->ballSprite.getGlobalBounds().left + this->ballSprite.getGlobalBounds().width > this->window.getSize().x)
        this->ballDX = -this->ballDX;

    //Top & Bottom
    if (this->ballY < 0)
        this->ballDY = -this->ballDY;
    else if (this->ballY > this->window.getSize().y)
        this->ballDY = -this->ballDY;
        //this->hasEnded = true;


    //  Paddle collision with screen    //
    if (this->paddleSprite.getGlobalBounds().left < 0.f)
        this->paddleSprite.setPosition(0.f, this->paddleSprite.getGlobalBounds().top);
    else if ((this->paddleSprite.getGlobalBounds().left + this->paddleSprite.getGlobalBounds().width) > this->window.getSize().x)
    {
        this->paddleSprite.setPosition(this->window.getSize().x - this->paddleSprite.getGlobalBounds().width,
            this->paddleSprite.getGlobalBounds().top);
    }
}


void Game::UpdateBall()
{
    this->ballX -= this->ballDX;
    this->ballY -= this->ballDY;

    this->ballSprite.setPosition(this->ballX, this->ballY);
}


void Game::UpdatePaddle(const std::string& command)
{
    //Handling flags on inputs
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) || command == "right")
    {
        this->isMovingLeft = false;
        this->isMovingRight = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || command == "left")
    {
        this->isMovingLeft = true;
        this->isMovingRight = false;
    }

    //Continuous movement based on flags
    if(this->isMovingRight)
        this->paddleSprite.move(this->paddleDX, 0.f);
    else if(this->isMovingLeft)
        this->paddleSprite.move(-this->paddleDX, 0.f);

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
            this->UpdatePaddle(command); // Process the received command
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


// Constructor
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
    
    if (!this->hasEnded)
    {
        this->UpdateBall();
        this->UpdatePaddle();
        this->CheckCollisions();
    }

    if (this->blockCount <= 0)
        this->hasEnded = true;
}


void Game::Render()
{
    this->window.clear();

    //  DRAW HERE   //
    this->window.draw(this->bgSprite);
    this->window.draw(this->ballSprite);
    this->window.draw(this->paddleSprite);

    for (const auto& block : this->blocks)
        this->window.draw(block);

    if (this->hasEnded)
    {
        if (this->blockCount > 0)
            this->endText.setString("Game Over!");
        else
            this->endText.setString("You Win!");

        this->window.draw(this->endText);
        this->music.stop();
    }

    this->window.display();
}

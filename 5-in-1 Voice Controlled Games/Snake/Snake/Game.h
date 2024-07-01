#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <WinSock2.h>


class Game
{
private:
    //Grid
    static const int colCount = 30, rowCount = 20;
    static const int size = 32;
    static const int w = size * colCount;
    static const int h = size * rowCount;

    //Game setup
    sf::RenderWindow window;
    sf::Event ev;
    sf::Clock clock;
    float timer, delay;
    bool hasEnded;

    //Resources
    sf::Texture groundTexture, snakeTexture;
    sf::Sprite ground, snake;
    sf::Font font;
    sf::Text endText;
    sf::Music music;
    sf::SoundBuffer eatBuffer;
    sf::Sound eatSound;

    //Snake and Apple
    struct Snake { int x, y; } s[100];
    struct Apple { int x, y; } a;
    int dir;
    int snakeLength, maxSnakeLength;
    bool isMovingUp, isMovingRight, isMovingDown, isMovingLeft;

    //Socket Communication
    SOCKET ListenSocket;
    SOCKET ClientSocket;
    std::thread socketThread;

    //  FUNCTIONS   //
    //Initializers
    void InitWindow();
    void InitResources();
    void Init_Socket();
    void Start();
    
    //Game Setup
    void PollEvents();
    
    //Game Control
    void Movement(const std::string& command = "");
    int Tick();
    void UpdateEndText();

    //Socket Communication
    void SocketListener();

public:
    // Constructor & Destructor
    Game();
    virtual ~Game();

    // Game setup
    const sf::RenderWindow& GetWindow() const { return this->window; };

    void Update();
    void Render();
};

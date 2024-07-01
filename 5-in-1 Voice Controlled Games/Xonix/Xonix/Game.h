#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <WinSock2.h>


class Game
{
private:
    // Constants
    static const int rowCount = 30, colCount = 45;
    static const int tileSize = 18;

    // Game setup
    sf::RenderWindow window;
    sf::Event ev;
    sf::Clock clock;
    float timer, delay;
    bool isGameOver;

    // Resources
    sf::Texture tileTexture, gameOverTexture, enemyTexture;
    sf::Sprite tile, gameOverImg, enemy;

    // Grid and Enemy
    int grid[rowCount][colCount] = { 0 };

    struct Enemy
    {
        int x, y, dx, dy;
        Enemy() //Constructor
        {
            //Enemy start position
            x = y = 300;

            //Enemy speed
            dx = 4 - rand() % 15;
            dy = 4 - rand() % 15;
        }

        void Move(int grid[rowCount][colCount])
        {
            x += dx;

            //Collision with walls
            if (grid[y / tileSize][x / tileSize] == 1)
            {
                dx = -dx;   
                x += dx;    
            }
            y += dy;
            if (grid[y / tileSize][x / tileSize] == 1)
            {
                dy = -dy;
                y += dy;
            }
        }
    };

    std::vector<Enemy> enemies;

    // Player position
    int playerX, playerY, dx, dy;
    bool isMovingUp, isMovingRight, isMovingDown, isMovingLeft;
    
    //Socket Communication
    SOCKET ListenSocket;
    SOCKET ClientSocket;
    std::thread socketThread;

    //  FUNCTIONS   //
    //Initializers
    void Init_Window();
    void Init_Resources();
    void Init_Socket();
    void Start();

    //Game Setup
    void PollEvents();
    
    //Game Control
    void Movement(const std::string& command = "");
    void Capture(int y, int x);
    void Tick();

    //Socket Communication
    void SocketListener();

public:
    // Constructor & Destructor
    Game();
    virtual ~Game();

    // Game setup
    const sf::RenderWindow& GetWindow() const { return this->window; }

    void Update();
    void Render();
};

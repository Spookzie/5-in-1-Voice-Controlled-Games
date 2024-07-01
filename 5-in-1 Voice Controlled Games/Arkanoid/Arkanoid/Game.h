#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <thread>
#include <WinSock2.h>


class Game
{
private:
    // VARIABLES //
    //Game setup
    sf::RenderWindow window;
    sf::Event ev;
    bool hasEnded;

    //Resources
    sf::Texture bgTexture, ballTexture, paddleTexture, blockTexture;
    sf::Sprite bgSprite, ballSprite, paddleSprite;
    std::vector<sf::Sprite> blocks;
    sf::Font font;
    sf::Text endText;
    sf::Music music;
    sf::SoundBuffer breakBuffer;
    sf::Sound breakSound;

    //Ball, Paddle & blocks
    float ballDX, ballDY, ballX, ballY;
    float paddleDX;
    bool isMovingLeft, isMovingRight;
    int blockCount;

    //Socket Communication
    SOCKET ListenSocket;
    SOCKET ClientSocket;
    std::thread socketThread;

    // FUNCTIONS //
    //Initializers
    void Init_Window();
    void Init_Resources();
    void Init_Socket();
    void Start();

    //Game Setup
    void PollEvents();
    
    //Game Control
    void CheckCollisions();
    void UpdateBall();
    void UpdatePaddle(const std::string& command = "");

    //Socket Communication
    void SocketListener();

public:
    // Constructor & Destructor
    Game();
    virtual ~Game();

    const sf::RenderWindow& GetWindow() const { return this->window; }

    void Update();
    void Render();
};

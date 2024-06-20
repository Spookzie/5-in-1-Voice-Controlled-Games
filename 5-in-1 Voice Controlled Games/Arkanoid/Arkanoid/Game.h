#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>


class Game
{
private:
    // VARIABLES //
    //Game setup
    sf::RenderWindow window;
    sf::Event ev;
    bool hasEnded;

    // Resources
    sf::Texture bgTexture, ballTexture, paddleTexture, blockTexture;
    sf::Sprite bgSprite, ballSprite, paddleSprite;
    std::vector<sf::Sprite> blocks;
    sf::Font font;
    sf::Text endText;
    sf::Music music;
    sf::SoundBuffer breakBuffer;
    sf::Sound breakSound;

    // Ball and paddle 
    float ballDX, ballDY, ballX, ballY;
    float paddleDX;
    int blockCount;

    // FUNCTIONS //
    //Initializers
    void Init_Window();
    void Init_Resources();
    void Start();

    //Game Setup
    void PollEvents();
    
    //Game Control
    void CheckCollisions();
    void UpdateBall();
    void UpdatePaddle();

public:
    // Constructor & Destructor
    Game();

    const sf::RenderWindow& GetWindow() const { return this->window; }

    void Update();
    void Render();
};

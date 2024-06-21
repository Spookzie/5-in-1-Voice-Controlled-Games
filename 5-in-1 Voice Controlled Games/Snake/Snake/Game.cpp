#include "Game.h"

void Game::InitWindow()
{
    this->window.create(sf::VideoMode(w, h), "Snake Game!");
    this->window.setFramerateLimit(60); //Even with tick logic implemented, setting a frame rate limit is better for renderering
}


void Game::InitResources()
{
    //  VFX //
    //Ground
    if (!this->groundTexture.loadFromFile("Textures/Snake/White.png"))
        std::cout << "ERROR::Game.cpp::InitResources: Failed to load white texture" << std::endl;

    this->ground.setTexture(this->groundTexture);
    this->ground.setScale(2.f, 2.f);

    //Snake
    if (!this->snakeTexture.loadFromFile("Textures/Snake/Red.png"))
        std::cout << "ERROR::Game.cpp::InitResources: Failed to load red texture" << std::endl;

    this->snake.setTexture(this->snakeTexture);
    this->snake.setScale(2.f, 2.f);

    //UI Text
    if (!this->font.loadFromFile("Fonts/PlaywriteNGModern-Regular.ttf"))
        std::cout << "ERROR::Game.cpp::InitResources: Failed to load font" << std::endl;

    this->endText.setFont(this->font);
    this->endText.setCharacterSize(64);
    this->endText.setOutlineColor(sf::Color::Black);
    this->endText.setOutlineThickness(3.f);
    this->endText.setFillColor(sf::Color::Red);
    this->endText.setPosition(50.f, 150.f);

    //  SFX //
    //Music
    if (!this->music.openFromFile("SFX/Snake/WondrousWaters.wav"))
        std::cout << "ERROR::Game.cpp::InitResources: Failed to music" << std::endl;

    this->music.setVolume(5.f);
    this->music.setLoop(true);
    this->music.play();

    //Bite
    if (!this->eatBuffer.loadFromFile("SFX/Snake/AppleBite.wav"))
        std::cout << "ERROR::Game.cpp::InitResources: Failed to bite sound" << std::endl;

    this->eatSound.setBuffer(this->eatBuffer);
    this->eatSound.setVolume(15.f);
}


void Game::Start()
{
    this->maxSnakeLength = this->snakeLength;
    this->hasEnded = false;

    //Setting fruit location
    a.x = 10;   a.y = 10;
}


void Game::PollEvents()
{
    while (this->window.pollEvent(this->ev))
    {
        if (this->ev.type == sf::Event::Closed)
            this->window.close();
    }   
}


void Game::Movement()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        this->dir = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        this->dir = 2;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        this->dir = 3;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        this->dir = 0;
}


int Game::Tick()
{
    //Move the snake body
    //Moves each segment of the snake body forward, starts the loop from tail(index num) to head (index 1)
    for (int i = this->snakeLength; i > 0; i--)
    {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

    //Move the snake head
    //Different dir values correspond to 4 different directions, 0=down, 1=left, 2=right, 3=up
    if (dir == 0) s[0].y += 1;
    if (dir == 1) s[0].x -= 1;
    if (dir == 2) s[0].x += 1;
    if (dir == 3) s[0].y -= 1;

    //Eating apple
    if (s[0].x == a.x && s[0].y == a.y) //Check to see if head is at the location of apple
    {
        this->snakeLength++;
        this->maxSnakeLength = this->snakeLength;
        this->eatSound.play();

        //Setting apple at a new random location
        a.x = rand() % this->colCount;
        a.y = rand() % this->rowCount;
    }

    //Updating snake location if it goes off grid (screen)
    //Goes off to right
    if (s[0].x >= this->colCount) 
        s[0].x = 0;
    //Goes off to left
    if (s[0].x < 0) 
        s[0].x = this->colCount - 1;
    //Goes off to bottom
    if (s[0].y >= this->rowCount) 
        s[0].y = 0;
    //Goes off to top
    if (s[0].y < 0)
        s[0].y = this->rowCount - 1;

    //Self Collision
    for (int i = 1; i < snakeLength; i++)
    {
        if (s[0].x == s[i].x && s[0].y == s[i].y)
        {
            this->hasEnded = true;
            this->snakeLength = 0;
            this->music.stop();
            return this->maxSnakeLength;
        }
    }
}


void Game::UpdateEndText()
{
    std::stringstream ssEndText;
    ssEndText << "        Game Over!\nYour snake was " << this->maxSnakeLength << "m long";
    this->endText.setString(ssEndText.str());
}


Game::Game() : dir(0), snakeLength(4), timer(0), delay(0.1f)
{
    this->InitWindow();
    this->InitResources();
    this->Start();
}


void Game::Update()
{
    float elapsedTime = this->clock.getElapsedTime().asSeconds();
    this->clock.restart();
    this->timer += elapsedTime;

    this->PollEvents();
    if (!this->hasEnded)
    {
        this->Movement();

        if (this->timer > this->delay)
        {
            this->timer = 0;
            this->Tick();
        }
    }
}


void Game::Render()
{
    this->window.clear();

    //Drawing ground grid
    for (int i = 0; i < colCount; i++)
    {
        for (int j = 0; j < rowCount; j++)
        {
            this->ground.setPosition(i * size, j * size);
            this->window.draw(this->ground);
        }
    }

    //Drawing snake
    for (int i = 0; i < snakeLength; i++)
    {
        this->snake.setPosition(s[i].x * size, s[i].y * size);
        this->window.draw(this->snake);
    }

    //Drawing fruit
    this->snake.setPosition(a.x * size, a.y * size);
    this->window.draw(this->snake);

    //Drawing end text
    if (this->hasEnded)
    {
        this->UpdateEndText();
        this->window.draw(this->endText);
    }

    this->window.display();
}
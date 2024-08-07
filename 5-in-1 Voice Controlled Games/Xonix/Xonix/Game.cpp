#include "Game.h"


void Game::Init_Window()
{
    this->window.create(sf::VideoMode(colCount * tileSize, rowCount * tileSize), "Xonix Game!");
    this->window.setFramerateLimit(60);
    this->window.setVerticalSyncEnabled(false);
}


void Game::Init_Resources()
{
    //  Graphics    //
    //Tile
    if (!this->tileTexture.loadFromFile("Textures/Xonix/Tiles.png"))
        std::cout << "ERROR::Game.cpp::Init_Resources: Failed to load tile texture" << std::endl;
    
    this->tile.setTexture(this->tileTexture);

    //Game Over
    if (!this->gameOverTexture.loadFromFile("Textures/Xonix/GameOver.png"))
        std::cout << "ERROR::Game.cpp::Init_Resources: Failed to load game over texture" << std::endl;
    
    this->gameOverImg.setTexture(this->gameOverTexture);
    this->gameOverImg.setPosition(100, 100);

    //Enemy
    if (!this->enemyTexture.loadFromFile("Textures/Xonix/Enemy.png"))
        std::cout << "ERROR::Game.cpp::Init_Resources: Failed to load enemy texture" << std::endl;

    this->enemy.setTexture(this->enemyTexture);
    this->enemy.setOrigin(20, 20);
}


Game::Game() : timer(0), delay(0.07f), isGameOver(false), playerX(0), playerY(0), dx(0), dy(0)
{
    this->Init_Window();
    this->Init_Resources();

    for (int i = 0; i < this->rowCount; i++)
    {
        for (int j = 0; j < this->colCount; j++)
        {
            if (i == 0 || j == 0 ||
                i == this->rowCount - 1 ||
                j == this->colCount - 1)
            {
                grid[i][j] = 1;
            }
        }
    }

    for (int i = 0; i < 4; i++)
        this->enemies.emplace_back();
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
    //Horizontal
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))     { this->dx = -1; this->dy = 0; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))    { this->dx = 1; this->dy = 0; }

    //Vertical
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))       { this->dx = 0; this->dy = -1; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))     { this->dx = 0; this->dy = 1; }
}


void Game::Capture(int y, int x)
{
    /* 
    * This function is used to capture, it first fills the current cell, then recursively checks and fills the neighbouring cells
        Indexing:
           -1 = Temporary marker indicating the cells that have been visited and are part of the connected region being marked.
            0 = empty
            1 = capturing
            2 = moving
    */

    //Filling
    if (this->grid[y][x] == 0)
        this->grid[y][x] = -1;
    
    //Checking cells up & down
    if (this->grid[y - 1][x] == 0) 
        this->Capture(y - 1, x);
    if (this->grid[y + 1][x] == 0)
        this->Capture(y + 1, x);
    
    //Checking cells left & right
    if (this->grid[y][x - 1] == 0) 
        this->Capture(y, x - 1);
    if (this->grid[y][x + 1] == 0) 
        this->Capture(y, x + 1);
}


void Game::Tick()
{
    this->playerX += this->dx;
    this->playerY += this->dy;

    //  Player & screen collision   //
    //Horizontal
    if (this->playerX < 0)
        this->playerX = 0;
    if (this->playerX > this->colCount - 1)
        this->playerX = this->colCount - 1;

    //Vertical
    if (this->playerY < 0)
        this->playerY = 0;
    if (this->playerY > this->rowCount - 1)
        this->playerY = this->rowCount - 1;
    
    //If moved back while capturing
    if (this->grid[playerY][playerX] == 2)
        this->isGameOver = true;

    //Moving trail
    if (this->grid[playerY][playerX] == 0)
        this->grid[playerY][playerX] = 2;
    
    //Enemy movement
    for (auto& enemy : this->enemies)
        enemy.Move(grid);

    //Checking if player is back in already captured area
    if (this->grid[playerY][playerX] == 1)
    {
        this->dx = this->dy = 0;    //Setting speed to 0

        //We are passing the enemies' locations to the Capture cuz when we draw a line, computer doesn't understand which of the 2 divided areas have we captured
        //So, by passing the enemies location, we check the area which has no enemies and capture that
        for (auto& enemy : this->enemies)
            this->Capture(enemy.y / this->tileSize, enemy.x / this->tileSize);

        //Explain from here
        for (int i = 0; i < this->rowCount; i++)
        {
            for (int j = 0; j < this->colCount; j++)
            {
                if (this->grid[i][j] == -1)
                    this->grid[i][j] = 0;
                else 
                    this->grid[i][j] = 1;
            }
        }
    }

    for (auto& enemy : this->enemies)
    {
        if (grid[enemy.y / this->tileSize][enemy.x / this->tileSize] == 2)
            this->isGameOver = true;
    }
}





void Game::Update()
{
    float elapsedTime = this->clock.getElapsedTime().asSeconds();
    this->clock.restart();
    this->timer += elapsedTime;

    this->PollEvents();
    if (!this->isGameOver)
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

    for (int i = 0; i < rowCount; i++)
        for (int j = 0; j < colCount; j++)
        {
            if (grid[i][j] == 0) continue;
            if (grid[i][j] == 1) this->tile.setTextureRect(sf::IntRect(0, 0, tileSize, tileSize));
            if (grid[i][j] == 2) this->tile.setTextureRect(sf::IntRect(54, 0, tileSize, tileSize));
            this->tile.setPosition(j * tileSize, i * tileSize);
            window.draw(this->tile);
        }

    this->tile.setTextureRect(sf::IntRect(36, 0, tileSize, tileSize));
    this->tile.setPosition(playerX * tileSize, playerY * tileSize);
    window.draw(this->tile);

    this->enemy.rotate(10);
    for (auto& enemy : enemies)
    {
        this->enemy.setPosition(enemy.x, enemy.y);
        window.draw(this->enemy);
    }

    if (isGameOver)
        window.draw(this->gameOverImg);

    window.display();
}
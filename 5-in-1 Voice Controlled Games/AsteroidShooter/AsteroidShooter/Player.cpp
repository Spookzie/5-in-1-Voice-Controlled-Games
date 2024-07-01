#include "Player.h"


//Texture Initializer Function
void Player::Init_Texture()
{
	if (!this->texture.loadFromFile("Textures/AsteroidShooter/Ship.png"))
		std::cout << "ERROR::Player.cpp::Init_Texture():: Failed to load the texture." << std::endl;
}


//Sprite Initializer Function
void Player::Init_Sprite()
{
	this->sprite.setTexture(this->texture);
	this->sprite.scale(0.1f, 0.1f);
}

//Variable Initializer Funciton
void Player::Init_Variables()
{
	this->moveSpeed = 8.f;
	this->attackCooldownMax = 15.f;
	this->attackCooldown = this->attackCooldownMax;
	this->hpMax = 75;
	this->hp = this->hpMax;
}


//Constructor
Player::Player()
{
	this->Init_Variables();
	this->Init_Texture();
	this->Init_Sprite();
}

//Destructor
Player::~Player()
{

}


//Get player position
const sf::Vector2f Player::GetPos() const
{
	return this->sprite.getPosition();
}


//Get Player Bounds
const sf::FloatRect Player::GetBounds() const
{
	return this->sprite.getGlobalBounds();
}


//Get current player HP
const int& Player::GetHP() const
{
	return this->hp;
}


//Get player max HP
const int& Player::GetHPMax() const
{
	return this->hpMax;
}


//Set Player location
void Player::SetPos(const float pos_x, const float pos_y)
{
	this->sprite.setPosition(pos_x, pos_y);
}


//Set Player HP
void Player::SetHP(const int hp)
{
	this->hp = hp;
}


//Player Movement
void Player::Move(const float dir_x, const float dir_y)
{
	this->sprite.move(this->moveSpeed * dir_x, this->moveSpeed * dir_y);
}


//Player Attacking
void Player::UpdateAttack()
{
	if (this->attackCooldown < this->attackCooldownMax)
		this->attackCooldown += 1.f;
}


//Check to see if player can attack 
const bool Player::CanAttack()
{
	if (this->attackCooldown >= this->attackCooldownMax)
	{
		this->attackCooldown = 0.f;
		return true;
	}

	return false;
}


//Apply damage to player
void Player::TakeDamage(const int damage)
{
	this->hp -= damage;
	if (this->hp < 0)
		this->hp = 0;
}


//Update Function
void Player::Update()
{
	this->UpdateAttack();
}

//Render Function
void Player::Render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
#include "Bullet.h"


//Default Constructor
Bullet::Bullet()
{

}

//Advanced Constructor
Bullet::Bullet(sf::Texture* texture, float pos_x, float pos_y, float dir_x, float dir_y, float move_speed)
{
	this->sprite.setTexture(*texture);

	this->sprite.setPosition(pos_x, pos_y);
	this->direction.x = dir_x;
	this->direction.y = dir_y;
	this->moveSpeed = move_speed;

	this->sprite.scale(0.06f, 0.06f);
}

//Destructor
Bullet::~Bullet()
{

}


//Getting Bullet Bounds
const sf::FloatRect Bullet::GetBounds()
{
	return this->sprite.getGlobalBounds();
}

//Update Function
void Bullet::Update()
{
	this->sprite.move(this->moveSpeed * this->direction);
}


//Render Function
void Bullet::Render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
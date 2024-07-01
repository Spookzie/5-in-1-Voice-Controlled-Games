#include "Enemy.h"


//Variable Initializer
void Enemy::Init_Variables()
{
	this->pointCount = rand() % 5 + 3;
	this->speed = 4.f;
	this->damage = this->pointCount;
	this->points = this->pointCount;

	this->exploding = false;
}


//Enemy Shape Initializer
void Enemy::Init_Shape()
{
	this->shape.setRadius(rand() % 21 + 15);
	this->shape.setPointCount(this->pointCount);
	this->shape.setFillColor(sf::Color(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1, 255));
}


//Constructor
Enemy::Enemy(float pos_x, float pos_y)
{
	this->shape.setPosition(pos_x, pos_y);

	this->Init_Variables();
	this->Init_Shape();
}


//Destructor
Enemy::~Enemy()
{

}


//Get enemy bounds
const sf::FloatRect Enemy::GetBounds()
{
	return this->shape.getGlobalBounds();
}


//Get how many points player gets from killing
const int& Enemy::GetPoints()
{
	return this->points;
}


//Get the damage value that the enemy deals to player
const int& Enemy::GetDamage()
{
	return this->damage;
}


//Start exploding the enemy
void Enemy::Explode()
{
	if (!this->exploding)
	{
		this->explosion = std::make_unique<Explosion>(this->shape.getPosition().x - this->shape.getRadius(),
			this->shape.getPosition().y - this->shape.getRadius());
		this->exploding = true;
	}
}


//Update Function
void Enemy::Update()
{
	if (this->exploding)
		this->explosion->Update();
	else
		this->shape.move(0.f, this->speed);
}


//Render Function
void Enemy::Render(sf::RenderTarget& target)
{
	if (this->exploding)
		this->explosion->Render(target);
	else
		target.draw(this->shape);
}


//Return true if enemy is exploding
bool Enemy::isExploding() const
{
	return this->exploding;
}


//Return true if enemy is dead i.e. explosion has been finished
bool Enemy::isDead() const
{
	return this->exploding && this->explosion->isFinished();
}
#pragma once

#include <SFML/Graphics.hpp>

#include "Explosion.h"


class Enemy
{
private:
	//	VARIABLES	//
	//Enemy UI
	sf::CircleShape shape;

	//Enemy Attributes
	unsigned pointCount;
	float speed;
	int damage;
	int points;

	//Explosion Animation
	bool exploding;
	std::unique_ptr<Explosion> explosion;


	//	FUNCTIONS	//
	//Initializers
	void Init_Variables();
	void Init_Shape();


public:
	//Constructor & Destructor
	Enemy(float pos_x, float pos_y);
	virtual ~Enemy();

	//Getters
	const sf::FloatRect GetBounds();
	const int& GetPoints();
	const int& GetDamage();

	//Setters
	void Explode();

	//Game Loop
	void Update();
	void Render(sf::RenderTarget& target);
	bool isExploding() const;
	bool isDead() const;
};
#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>


class Bullet
{
private:
	//	VARIABLES	//
	//Bullet UI
	sf::Sprite sprite;

	//Bullet Control
	sf::Vector2f direction;
	float moveSpeed;

public:
	//Constructor & Destructor
	Bullet();
	Bullet(sf::Texture* texture, float pos_x, float pos_y, float dir_x, float dir_y, float move_speed);
	virtual ~Bullet();

	//Getters
	const sf::FloatRect GetBounds();

	//Game Loop
	void Update();
	void Render(sf::RenderTarget& target);
};
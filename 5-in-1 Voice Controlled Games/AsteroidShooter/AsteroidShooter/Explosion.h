#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>


class Explosion
{
private:
	//	VARIABLES	//
	sf::Sprite sprite;
	std::vector<sf::Texture> textures;

	size_t currentFrame;

public:
	//Constructor
	Explosion(float pos_x, float pos_y);

	//Getter
	bool isFinished() const;

	void Update();
	void Render(sf::RenderTarget& target);
};
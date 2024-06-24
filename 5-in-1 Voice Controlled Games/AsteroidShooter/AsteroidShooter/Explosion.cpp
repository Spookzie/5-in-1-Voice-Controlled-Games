#include "Explosion.h"


//Constructor
Explosion::Explosion(float pos_x, float pos_y)
{
	this->currentFrame = 0;

	//Loading the textures
	for (int i = 0; i < 7; i++)
	{
		sf::Texture texture;
		if (!texture.loadFromFile("Textures/AsteroidShooter/Explosion" + std::to_string(i) + ".png"))
			std::cout << "ERROR::Explosion.cpp::Explosion():: Failed to load explosion texture no.: " << i << std::endl;

		textures.push_back(texture);
	}

	sprite.setTexture(textures[currentFrame]);
	sprite.setPosition(pos_x, pos_y);
	sprite.setScale(0.5f, 0.5f);
}


//Update Function
void Explosion::Update()
{
	currentFrame++;

	if (currentFrame < textures.size())
		sprite.setTexture(textures[currentFrame]);
}


//Returns true if the animation is complete i.e. all the textures have been displayed.
bool Explosion::isFinished() const
{
	return currentFrame >= textures.size();
}


//Render Function
void Explosion::Render(sf::RenderTarget& target)
{
	if (currentFrame < textures.size())
		target.draw(this->sprite);
}
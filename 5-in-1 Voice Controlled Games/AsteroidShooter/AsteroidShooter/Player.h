#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>


class Player
{
private:
	//	VARIABLES	//
	//Player UI
	sf::Sprite sprite;
	sf::Texture texture;

	//Player Attributes
	float moveSpeed;
	float attackCooldown;
	float attackCooldownMax;
	int hp;
	int hpMax;


	//	FUNCTIONS	//
	//Initializers
	void Init_Texture();
	void Init_Sprite();
	void Init_Variables();

public:
	//Constructor & Destructor
	Player();
	virtual ~Player();

	//Getters
	const sf::Vector2f GetPos() const;
	const sf::FloatRect GetBounds() const;
	const int& GetHP() const;
	const int& GetHPMax() const;

	//Setters
	void SetPos(const float pos_x, const float pos_y);
	void SetHP(const int hp);

	//Player Control
	void Move(const float dir_x, const float dir_y);
	void UpdateAttack();
	const bool CanAttack();
	void TakeDamage(const int damage);

	//Game Loop
	void Update();
	void Render(sf::RenderTarget& target);
};
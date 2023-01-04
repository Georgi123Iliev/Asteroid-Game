#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<iostream>
#include<vector>
#include"Projectile.h"
#include<chrono>
#define CWHOLE(c) c.cbegin(),c.cend()
#define WHOLE(c) c.begin(),c.end()

enum rotation
{
	left = -3,
	right = 3
};


class Spaceship
{
	
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Image idleImg,runningImg;
	sf::ConvexShape hitBox;
	std::vector<sf::Vector2f> points;
	std::vector<Projectile> projectiles;
	std::chrono::time_point<std::chrono::system_clock> lastShot;
		
	float distance{6};
	float decliningDistance{ 0 };

	const float reloadTime = 250.f;
	
	struct isOutOfBounds 
	{
		[[nodiscard]] bool operator()(const Projectile& pX, unsigned int xBound, unsigned int yBound) const noexcept;
	};

	[[nodiscard]] bool reloaded() const noexcept;
	
public:
	Spaceship();
	void Move();
	void updateProjectiles();
	void DisposeProjectiles(unsigned int xBound, unsigned int yBound);
	void finishMovement();
	
	void Rotate(rotation rot);
	[[nodiscard]] bool isOutOfBounds(unsigned int xBound, unsigned int yBound) const noexcept;
	void readjust(float xBound, float yBound);
	void explode();
	
	[[nodiscard]] const sf::Sprite& Drawable() const noexcept;
	[[nodiscard]] const std::vector<sf::Vector2f>& GetPoints() const noexcept;
	[[nodiscard]] const std::vector<sf::CircleShape> ProjDrawables() const noexcept;
	void Shoot();
	[[nodiscard]] bool hasHitTarget(const std::pair<sf::Vector2f, float>& asteroidParams);
};


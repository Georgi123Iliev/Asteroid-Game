#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<iostream>
#include<chrono>

class imageResources
{
private:
	sf::Image img;
	sf::Texture texture;
	bool textureSet=false;


	imageResources()
	{

		if (!textureSet)
		{
			
			if (!img.loadFromFile("Textures\\asteroid.png"))
			{
				// error...
			}

			img.createMaskFromColor(sf::Color::White);

			texture.loadFromImage(img);
		}
		textureSet = true;
	}
public:
	static imageResources& Instance()
	{
		static imageResources singleton;
		return singleton;
	}
	
	const auto& GetTexture() const
	{
		return texture;
	}
};








class Asteroid
{
	
	
	sf::Sprite sprite;
	sf::CircleShape hitBox;

	sf::Vector2f direction;

	std::chrono::time_point<std::chrono::system_clock> lastBounceX;
	std::chrono::time_point<std::chrono::system_clock> lastBounceY;

public:
	Asteroid(const sf::Vector2f pos, const sf::Vector2f& offset);
	void Move();
	//(x - center_x)² + (y - center_y)² < radius²
	[[nodiscard]] bool ContainsPoint(const sf::Vector2f& point) const noexcept;

	[[nodiscard]] const sf::Sprite& Drawable() const noexcept;
	
	[[nodiscard ]]  bool isOutOfBoundsX(const sf::Vector2f& bounds) const noexcept;
	
	[[nodiscard]]  bool isOutOfBoundsY(const sf::Vector2f& bounds) const noexcept;
	[[nodiscard]] std::pair<sf::Vector2f, float> params() const;
	void BounceX() noexcept;
	void BounceY() noexcept;
	
	[[nodiscard]] bool operator == (const Asteroid& other) const noexcept;
};


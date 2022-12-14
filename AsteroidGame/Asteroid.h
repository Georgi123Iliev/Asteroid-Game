#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<iostream>


class imageResources
{
private:
	sf::Image img;
	sf::Texture texture;
	bool textureSet=false;
	
public:
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
	const auto& GetTexture() const
	{
		return texture;
	}
};
static const imageResources imgResource;







class Asteroid
{
	
	
	sf::Sprite sprite;
   
	sf::CircleShape hitBox;
	sf::Vector2f direction;

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
	
	[[nodiscard]] inline bool operator == (const Asteroid& other) const noexcept;
};


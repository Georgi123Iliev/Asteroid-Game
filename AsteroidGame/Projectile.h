#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Network.hpp>
template <typename T1, typename T2>
const sf::Vector2f toV2(T1 x, T2 y)
{
	return sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
}
class Projectile
{
	typedef float angle;
	sf::CircleShape shape;
	const float speed = 10.f;
	 angle direction;

	 constexpr auto toRadians(angle dir) const
	 {
		 return dir / 57.f;
	 }
public:
	Projectile(angle direction,const sf::Vector2f& startingPoint) 
	{
		shape.setPosition(startingPoint);
		shape.setFillColor(sf::Color::Red);
		shape.setRadius(4);
		this->direction = direction;
	}
	void Move()
	{
		
	shape.move(	toV2( sin(toRadians(direction)) * speed,
		 cos(toRadians(direction)) * speed * -1));
	}
	bool isOutOfBounds(unsigned int xBound, unsigned int yBound) const
	{
		const auto& pos = shape.getPosition();
		const auto& radius = shape.getRadius();
		return pos.x - radius<0	||pos.y - radius<0|| pos.y + radius>yBound|| pos.x + radius>xBound;

	}
	const sf::CircleShape& Drawable() const
	{
		return shape;
	}
	
	void operator= (const Projectile& other)
	{
		shape = other.shape;
		direction = other.direction;

	}
	bool collides(const sf::Vector2f& center, const float& radius) const
	{
	//	std::cout << "x:" << center.x << "y:" << center.x << "r:" << radius << "\n";
		const auto& asteroidCenter = shape.getPosition();
		auto distance = sqrt(pow(center.x - asteroidCenter.x, 2) + pow(center.y - asteroidCenter.y, 2));
		return distance <= radius + shape.getRadius();
		
		
	}
};


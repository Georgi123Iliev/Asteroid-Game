#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Network.hpp>
template <typename T1, typename T2>
sf::Vector2f toV2(T1 x, T2 y)
{
	return sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
}
class Projectile
{
	using angle = float;
	sf::CircleShape shape;
	mutable float speed = 10.f;
	 angle direction;

	 [[nodiscard]] constexpr auto toRadians(angle dir) const noexcept;
public:
	Projectile(angle direction, const sf::Vector2f& startingPoint);
	void Move();
	[[nodiscard]] bool isOutOfBounds(unsigned int xBound, unsigned int yBound) const noexcept;
	const sf::CircleShape& Drawable() const noexcept;
	
	[[nodiscard]] bool collides(const sf::Vector2f& center, const float& radius) const noexcept;
};


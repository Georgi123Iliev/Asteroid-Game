#include "Asteroid.h"

Asteroid::Asteroid(const sf::Vector2f pos, const sf::Vector2f& offset)
	:direction(offset)
{

	sprite.setTexture(imgResource.GetTexture());
	sprite.setPosition(pos);
	sprite.setOrigin({ 15,13 });


	hitBox.setPosition(sprite.getPosition());

	hitBox.setOrigin({ 15,13 });


	hitBox.setRadius(14);

}

void Asteroid::Move()
{
	sprite.move(direction);
	hitBox.move(direction);
}

[[nodiscard]] bool Asteroid::ContainsPoint(const sf::Vector2f& point) const noexcept
{
	sf::Vector2f centre = { (hitBox.getPosition().x + hitBox.getOrigin().x),(hitBox.getPosition().y + hitBox.getOrigin().y) };
	return pow(point.x - centre.x, 2) + pow(point.y - centre.y, 2) <= pow(hitBox.getRadius(), 2);
}

[[nodiscard]] const sf::Sprite& Asteroid::Drawable() const noexcept
{
	return sprite;
}

bool Asteroid::isOutOfBoundsX(const sf::Vector2f& bounds) const noexcept
{
	auto radius = hitBox.getRadius();
	const auto& pos = hitBox.getPosition();
	return pos.x - radius<0
		|| pos.x + radius>bounds.x;
}

bool Asteroid::isOutOfBoundsY(const sf::Vector2f& bounds) const noexcept
{
	auto radius = hitBox.getRadius();
	const auto& pos = hitBox.getPosition();
	return pos.y - radius<0
		|| pos.y + radius>bounds.y;
}

[[nodiscard]] std::pair<sf::Vector2f, float> Asteroid::params() const
{
	return std::make_pair(hitBox.getPosition(), hitBox.getRadius());
}

void Asteroid::BounceX() noexcept
{
	auto currentTime = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastBounceX).count() >= 3)
	{
		direction.x *= -1;
		lastBounceX = currentTime;
	}
	

}

void Asteroid::BounceY() noexcept
{
	auto currentTime = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastBounceY).count() >= 3000)
	{
		direction.y *= -1;
		lastBounceY = currentTime;
	}
	
}

inline bool Asteroid::operator==(const Asteroid& other) const noexcept
{
	return this == &other;
}

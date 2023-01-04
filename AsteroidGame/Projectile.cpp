#include "Projectile.h"

constexpr auto Projectile::toRadians(angle dir) const noexcept
{
	return dir / 57.f;
}

Projectile::Projectile(angle direction, const sf::Vector2f& startingPoint)
{
	shape.setPosition(startingPoint);
	shape.setFillColor(sf::Color::Red);
	shape.setRadius(4);
	this->direction = direction;
}

void Projectile::Move()
{

	shape.move(toV2(sin(toRadians(direction)) * speed,
		cos(toRadians(direction)) * speed * -1));
}

bool Projectile::isOutOfBounds(unsigned int xBound, unsigned int yBound) const noexcept
{
	const auto& pos = shape.getPosition();
	const auto& radius = shape.getRadius();
	return pos.x - radius<0 || pos.y - radius<0 || pos.y + radius>yBound || pos.x + radius>xBound;

}

const sf::CircleShape& Projectile::Drawable() const noexcept
{
	return shape;
}

void Projectile::operator=(const Projectile& other) noexcept
{
	shape = other.shape;
	direction = other.direction;

}

bool Projectile::collides(const sf::Vector2f& center, const float& radius) const noexcept
{
	//	std::cout << "x:" << center.x << "y:" << center.x << "r:" << radius << "\n";
	const auto& asteroidCenter = shape.getPosition();
	auto distance = sqrt(pow(center.x - asteroidCenter.x, 2) + pow(center.y - asteroidCenter.y, 2));
	return distance <= radius + shape.getRadius();


}

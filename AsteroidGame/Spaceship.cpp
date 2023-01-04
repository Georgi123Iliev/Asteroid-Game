#include "Spaceship.h"

bool Spaceship::reloaded() const noexcept
{
	return fabs(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastShot).count()) >= reloadTime;
}

Spaceship::Spaceship()
	:lastShot(std::chrono::system_clock::now())
{

	if (!idleImg.loadFromFile("Textures\\ship_idle.png"))
	{
		// error...
	}
	if (!runningImg.loadFromFile("Textures\\ship.png"))
	{
		// error...
	}

	idleImg.createMaskFromColor(sf::Color::Black);

	texture.loadFromImage(idleImg);
	sprite.setTexture(texture);

	sprite.setOrigin({ 57,45 });

	sprite.setPosition(300, 240);
	hitBox.setPosition(300, 240);
	hitBox.setPointCount(9);

	hitBox.setOrigin(hitBox.getPosition());

	hitBox.setPoint(0, sprite.getPosition() + sf::Vector2f{ 57,10 } - sf::Vector2f{ 57,42 });
	hitBox.setPoint(1, sprite.getPosition() + sf::Vector2f{ 38,33 } - sf::Vector2f{ 57,42 });
	hitBox.setPoint(2, sprite.getPosition() + sf::Vector2f{ 18,44 } - sf::Vector2f{ 57,42 });
	hitBox.setPoint(3, sprite.getPosition() + sf::Vector2f{ 17,71 } - sf::Vector2f{ 57,42 });
	hitBox.setPoint(4, sprite.getPosition() + sf::Vector2f{ 42,59 } - sf::Vector2f{ 57,42 });
	hitBox.setPoint(5, sprite.getPosition() + sf::Vector2f{ 76,59 } - sf::Vector2f{ 57,42 });
	hitBox.setPoint(6, sprite.getPosition() + sf::Vector2f{ 99,72 } - sf::Vector2f{ 57,42 });
	hitBox.setPoint(7, sprite.getPosition() + sf::Vector2f{ 98,43 } - sf::Vector2f{ 57,42 });
	hitBox.setPoint(8, sprite.getPosition() + sf::Vector2f{ 72,28 } - sf::Vector2f{ 57,42 });



	for (size_t i = 0; i < 9; i++)
	{
		points.push_back(hitBox.getPoint(i));
	}
	points.push_back(hitBox.getOrigin());

}

void Spaceship::Move()
{


	if (distance == 0)
		return;
	auto angle = sprite.getRotation();
	auto toRadians = angle / 57.f;
	float xOffset = sin(toRadians) * distance;
	float yOffset = cos(toRadians) * distance * -1;
	texture.loadFromImage(runningImg);
	decliningDistance = distance;
	sprite.move({ xOffset,yOffset });
	hitBox.move({ xOffset,yOffset });

	for (size_t i = 0; i < 10; i++)
	{
		points[i] = points[i] + sf::Vector2f{ xOffset, yOffset };
	}
}

void Spaceship::updateProjectiles()
{
	std::for_each(WHOLE(projectiles), [](auto& proj) {proj.Move(); });
}

void Spaceship::DisposeProjectiles(unsigned int xBound, unsigned int yBound)
{
	projectiles.erase(std::remove_if(WHOLE(projectiles), [xBound, yBound](Projectile& proj) {return proj.isOutOfBounds(xBound, yBound); }), projectiles.end());


}

void Spaceship::finishMovement()
{
	if (decliningDistance == 0)
		return;


	auto angle = sprite.getRotation();

	auto toRadians = angle / 57.f;
	float xOffset = sin(toRadians) * decliningDistance;
	float yOffset = cos(toRadians) * decliningDistance * -1;

	decliningDistance -= .25f;
	texture.loadFromImage(idleImg);
	sprite.move({ xOffset,yOffset });
	hitBox.move({ xOffset,yOffset });


	for (size_t i = 0; i < 10; i++)
	{
		points[i] = points[i] + sf::Vector2f{ xOffset, yOffset };
	}
}

void Spaceship::Rotate(rotation rot)
{
	if (distance == 0)
		return;
	sprite.setRotation(sprite.getRotation() + rot);
	hitBox.setRotation(hitBox.getRotation() + rot);


	const auto& origin = points[9];

	for (size_t i = 0; i < 9; i++)
	{


		float d = static_cast<float>(sqrt(pow((origin.x - points[i].x), 2) + pow((origin.y - points[i].y), 2)));

		auto toRadians = rot / 57.f;
		sf::Transform transform;
		transform.rotate(static_cast<float>(rot), origin);

		points[i] = transform.transformPoint(points[i]);



	}



}

bool Spaceship::isOutOfBounds(unsigned int xBound, unsigned int yBound) const noexcept
{
	const auto& pos = sprite.getPosition();
	return pos.x<0 || pos.x>xBound
		|| pos.y<0 || pos.y>yBound;


}

void Spaceship::readjust(float xBound, float yBound)
{
	for (size_t i = 0; i < 10; i++)
	{
		points[i] -= hitBox.getPosition();
	}
	sprite.setPosition((sf::Vector2f{ xBound,yBound }) - sprite.getPosition());
	hitBox.setPosition((sf::Vector2f{ xBound,yBound }) - hitBox.getPosition());

	for (size_t i = 0; i < 10; i++)
	{
		points[i] += hitBox.getPosition();
	}

}

void Spaceship::explode()
{
	idleImg.loadFromFile("Textures\\explosion.png");
	runningImg.loadFromFile("Textures\\explosion.png");
	texture.loadFromImage(idleImg);
	distance = 0;
	decliningDistance = 0;

}

const sf::Sprite& Spaceship::Drawable() const noexcept
{
	return sprite;
}

const std::vector<sf::Vector2f>& Spaceship::GetPoints() const noexcept
{
	return points;
}

const std::vector<sf::CircleShape> Spaceship::ProjDrawables() const noexcept
{
	std::vector<sf::CircleShape> drawables;
	std::for_each(CWHOLE(projectiles), [&drawables](const auto& proj) {drawables.push_back(proj.Drawable()); });
	return drawables;
}

void Spaceship::Shoot()
{
	if (reloaded())
	{
		projectiles.push_back(Projectile(hitBox.getRotation(), points[0])); //points[0] is the front of the ship

		lastShot = std::chrono::system_clock::now();
	}
}

bool Spaceship::hasHitTarget(const std::pair<sf::Vector2f, float>& asteroidParams)
{
	auto hit = false;

	auto it = std::remove_if(WHOLE(projectiles), [&asteroidParams, &hit, this](const auto& proj)
		{	if (proj.collides(asteroidParams.first, asteroidParams.second))
	{
		hit = true;
		return true;
	}
	return false;
		});
	projectiles.erase(it, projectiles.end());
	return hit;
}

bool Spaceship::isOutOfBounds::operator()(const Projectile& pX, unsigned int xBound, unsigned int yBound) const noexcept
{
	return pX.isOutOfBounds(xBound, yBound);
}

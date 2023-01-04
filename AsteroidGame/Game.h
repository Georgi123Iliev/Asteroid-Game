#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<SFML/Window/Keyboard.hpp>
#include"Spaceship.h"
#include<iostream>
#include<algorithm>
#include<vector>
#include<time.h>
#include<chrono>
#include"Asteroid.h"
#define CWHOLE(c) c.cbegin(),c.cend()
#define WHOLE(c) c.begin(),c.end()






class Game
{
	sf::RenderWindow window;
	Spaceship spaceship;
	std::vector<Asteroid> asteroids;
	bool gameOver{ false };
	bool WPressed{ false };
	bool APressed{ false }, DPressed{ false };
	std::chrono::steady_clock::time_point lastAsteroidSpawned;
	 constexpr unsigned int maxAsteroids() const
	{
		return 150;
	}
	 constexpr unsigned int winHeight() const
	{
		return 720;
	}

    constexpr unsigned int winWidht() const
	{
		return 1080;
	}
	void SpawnAsteroid()
	{
		int rnd = rand() % 5 + 1;
		auto dir = sf::Vector2f(6.f - rnd, 0.f + rnd);

		auto randX = rand() % 1080 - 14;
		auto randY = rand() % 720 - 14;

		auto config = rand() % 4;
		if (config == 0)
			asteroids.push_back(Asteroid(toV2(14, randY),dir));
		else if (config == 1)
			asteroids.push_back(Asteroid(toV2(randX, 14.f), dir));
		else if (config == 2)
			asteroids.push_back(Asteroid(toV2(winWidht() - 14, randY), dir));
		else if (config == 3)
			asteroids.push_back(Asteroid(toV2(randX, winHeight() - 14), dir));

		lastAsteroidSpawned = std::chrono::steady_clock::now();
		
	}
	void Render()
	{
		window.clear();

		window.draw(spaceship.Drawable());
		const auto& projs{ spaceship.ProjDrawables() };
		std::for_each(CWHOLE(projs), [this](const auto& proj) {window.draw(proj); });
		std::for_each(CWHOLE(asteroids), [this](const auto& asteroid) {window.draw(asteroid.Drawable());  });

		window.display();
	}
	void CheckForMovements()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			WPressed = true;
		else
			WPressed = false;

		if (!DPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			APressed = true;
		else
			APressed = false;
		if (!APressed && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			DPressed = true;
		else
			DPressed = false;
	}
	void MakeMovements()
	{
		if (spaceship.isOutOfBounds(winWidht(), winHeight()))
			spaceship.readjust(static_cast<float>(winWidht()), static_cast<float>(winHeight()));

		if (WPressed)
			spaceship.Move();
		else
			spaceship.finishMovement();

		if (APressed)
			spaceship.Rotate(left);

		if (DPressed)
			spaceship.Rotate(right);
		std::for_each(WHOLE(asteroids), [this](auto& asteroid) {asteroid.Move(); });
		spaceship.DisposeProjectiles(winWidht(),winHeight());
		spaceship.updateProjectiles();
		

	}
	void GameOver()
	{
		gameOver = true;

		spaceship.explode();
	}

	void CheckForGameOver()
	{
		const auto& points = spaceship.GetPoints();
		std::for_each(CWHOLE(points),
			[this](const auto& point)
			{

				std::for_each(WHOLE(asteroids), [this, &point](const auto& asteroid)
					{if (asteroid.ContainsPoint(point))
					GameOver();
					});


			});
	}

public:
	Game()
		:window(sf::VideoMode(winWidht(), winHeight()), "Asteroids")
	{
		srand(static_cast<unsigned int>(time(nullptr)));
		lastAsteroidSpawned = std::chrono::steady_clock::now();
		window.setFramerateLimit(48);
	}
	void Run()
	{
		while (window.isOpen())
		{
			auto it = std::remove_if(WHOLE(asteroids), [this](const auto& asteroid) {return spaceship.hasHitTarget(asteroid.params()); });
			if (it != asteroids.end())
			{
				asteroids.erase(it, asteroids.end());
				lastAsteroidSpawned=std::chrono::steady_clock::now();
			}
			if (!gameOver)
			{
				CheckForGameOver();
				CheckForMovements();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
					spaceship.Shoot();

			}

			std::for_each(WHOLE(asteroids), [this](auto& asteroid)
			{
					if (asteroid.isOutOfBoundsX(toV2(winWidht(), winHeight())))
					{
						asteroid.BounceX();
						
					}
					if (asteroid.isOutOfBoundsY(toV2(winWidht(), winHeight())))
					{
						asteroid.BounceY();
						
					}
					
			});
			
				
			

			if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - lastAsteroidSpawned).count() >= 1
				&& asteroids.size() < maxAsteroids())
				SpawnAsteroid();

			MakeMovements();



			sf::Event event;
			while (window.pollEvent(event))
			{


				if (event.type == sf::Event::Closed)
					window.close();
			}
			Render();
		}
	}
};


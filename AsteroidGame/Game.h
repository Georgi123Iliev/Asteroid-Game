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

	[[nodiscard]] constexpr unsigned int maxAsteroids() const noexcept;
	[[nodiscard]] constexpr unsigned int winHeight() const noexcept;
	[[nodiscard]] constexpr unsigned int winWidht() const noexcept;

	void SpawnAsteroid();
	void Render();
	void CheckForMovements();
	void MakeMovements();
	void GameOver();
	void CheckForGameOver();

public:
	Game();
	void Run();
};


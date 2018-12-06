#pragma once
#include "GameObject.h"
#include "TileMapper.h"
#include <SFML/Graphics.hpp>
#include <vector>


class Roboto
{
	sf::Vector2f dir;
	bool dead = false;

	bool yeeted = false;
	sf::Vector2f yeetDir;
	float yeetSpeed = 100.f, sinceYeet = 0.f, yeetDuration = 1.0f;

public:
	sf::RectangleShape rs;
	float MOVEMENT_SPEED = 50.f;
	bool hacked = false;

	Roboto(sf::Vector2f pos, sf::Texture* t) {
		rs.setTexture(t);
		rs.setPosition(pos);
		rs.setSize(sf::Vector2f(64.f, 64.f));
	}

	~Roboto() {};

	void resolveCollisions(const TileMapper& tm, float frameTime)
	{
		for (auto a : tm.level) {
			if (a.collides && rs.getGlobalBounds().intersects(a.rs.getGlobalBounds())) {
				sf::Vector2f wat = (rs.getPosition() + rs.getSize() * 0.5f) - (a.rs.getPosition() + a.rs.getSize() * 0.5f);
				float l = std::max(sqrt(wat.x * wat.x + wat.y * wat.y), 0.00001f);
				wat = wat / l;

				if (yeeted)
					dead = true;

				rs.move( wat * MOVEMENT_SPEED * frameTime);
			}
		}
	}



	void update(const TileMapper& tm, const sf::Vector2f playerPos, float frameTime)
	{
		if (yeeted) 
		{
			rs.move(yeetDir * frameTime * yeetSpeed);
			sinceYeet += frameTime;
			resolveCollisions(tm, frameTime);

			if (sinceYeet >= yeetDuration)
				yeeted = false;
			return;
		}


		if (hacked)
			return;	

		dir = playerPos - rs.getPosition();
		dir = dir / sqrt(dir.x * dir.x + dir.y * dir.y);
		rs.move(dir * MOVEMENT_SPEED * frameTime);
		resolveCollisions(tm, frameTime);
	}

	

	void draw(sf::RenderWindow& window) {
		window.draw(rs);
	}



	void exterminate() {
		dead = true;
	}



	bool isDead() {
		return dead;
	}

	

	void yeet(sf::Vector2f yeetDirection) {
		yeeted = true;
		yeetDir = yeetDirection;
		sinceYeet = 0.f;
	}
};



class Overlord {

	sf::Texture t;
	int numEnemies;

public:

	std::vector<Roboto> robotos;

	Overlord(int numBots = 20) {
		t.loadFromFile("../Assets/machine.png");
	}

	~Overlord() {}



	void fill(const std::vector<sf::Vector2f>& enemyMap)
	{
		for (auto enemyPos : enemyMap)
			robotos.push_back(Roboto(enemyPos, &t));

		numEnemies = enemyMap.size();
	}



	void update(const TileMapper& tm, const sf::Vector2f playerPos, float frameTime)
	{
		//strange bug... it will keep going when (i < -1) maybe because size is unsigned int so comparing to -1 makes it go to a very big number?
		if (robotos.size() > 0) {
			for (int i = 0; i < robotos.size() - 1; i++) {
				for (int j = i + 1; j < robotos.size(); j++) {
					if (robotos[i].rs.getGlobalBounds().intersects(robotos[j].rs.getGlobalBounds())) {
						sf::Vector2f displacement = robotos[j].rs.getPosition() - robotos[i].rs.getPosition();	//i to j pos vector
						displacement = displacement / sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
						robotos[i].rs.move(-displacement * frameTime * robotos[i].MOVEMENT_SPEED * 0.1f);
						robotos[j].rs.move(displacement * frameTime * robotos[j].MOVEMENT_SPEED * 0.1f);
					}
				}
			}
		}

		for (Roboto& r : robotos) 
			r.update(tm, playerPos, frameTime);



		robotos.erase(std::remove_if(robotos.begin(), robotos.end(), [](Roboto& r) { return r.isDead(); }), robotos.end());
	}

	void draw(sf::RenderWindow& window)
	{
		for (auto r : robotos)
			r.draw(window);
	}
};

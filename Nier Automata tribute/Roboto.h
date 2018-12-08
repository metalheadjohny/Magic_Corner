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
	int OG_INDEX;

	Roboto(sf::Vector2f pos, sf::Texture* t, int og_index) {
		rs.setTexture(t);
		rs.setPosition(pos);
		rs.setSize(sf::Vector2f(64.f, 64.f));
		OG_INDEX = og_index;
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



	void resuscitate() {
		dead = false;
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
	std::vector<sf::Int32> bodyCount, deltaBodyCount, serverBodyCount;

	Overlord(int numBots = 20) {
		t.loadFromFile("../Assets/machine.png");
	}

	~Overlord() {}



	void fill(const std::vector<sf::Vector2f>& enemyMap)
	{
		for (int i = 0; i < enemyMap.size(); i++)
			robotos.push_back(Roboto(enemyMap[i], &t, i));

		numEnemies = enemyMap.size();
	}



	std::vector<int> update2b(const TileMapper& tm, const sf::Vector2f playerPos, float frameTime)
	{
		//strange bug... it will keep going when vector is empty
		//likely because size is unsigned int? cheap fix...
		if (robotos.size() > 0) {
			for (int i = 0; i < robotos.size() - 1; i++)
			{
				for (int j = i + 1; j < robotos.size(); j++) 
				{
					if (robotos[i].rs.getGlobalBounds().intersects(robotos[j].rs.getGlobalBounds())) 
					{
						sf::Vector2f displacement = robotos[j].rs.getPosition() - robotos[i].rs.getPosition();	//i to j pos vector
						displacement = displacement / sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
						robotos[i].rs.move(-displacement * frameTime * robotos[i].MOVEMENT_SPEED * 0.05f);
						robotos[j].rs.move(displacement * frameTime * robotos[j].MOVEMENT_SPEED * 0.05f);
					}
				}
			}
		}

		for (Roboto& r : robotos)
			r.update(tm, playerPos, frameTime);
			
		deltaBodyCount.clear();
		//add them to the list of dead robots
		for (int i = 0; i < robotos.size(); i++) {
			if (robotos[i].isDead()) {
				//if(std::find(bodyCount.begin(), bodyCount.end(), robotos[i].OG_INDEX) == bodyCount.end()) won't happen... I think
				deltaBodyCount.push_back(robotos[i].OG_INDEX);	//add it to the intermediate list that will be appended to the main list
			}
		}

		if (deltaBodyCount.size() != 0) {
			bodyCount.insert(bodyCount.end(), deltaBodyCount.begin(), deltaBodyCount.end());	//append the intermediate list to the main list
			std::sort(bodyCount.begin(), bodyCount.end());	//sort the main list every time it is changed
		}
		
		//erase dead enemies, which will shift the indices of some of the enemies but they keep their original index (OG_INDEX) value, used for sync
		robotos.erase(std::remove_if(robotos.begin(), robotos.end(), [](Roboto& r) { return r.isDead(); }), robotos.end());

		//return the intermediate list, if this is not size 0 it will trigger an update to 9s
		return deltaBodyCount;
	}



	void update9s(const TileMapper& tm, const sf::Vector2f playerPos, float frameTime) {

		//based on the updates from the server, synchronize which robots are alive
		for (Roboto& r : robotos) {
			if (std::find(serverBodyCount.begin(), serverBodyCount.end(), r.OG_INDEX) != serverBodyCount.end())
				r.exterminate();
			else
				r.resuscitate();
		}

		robotos.erase(std::remove_if(robotos.begin(), robotos.end(), [](Roboto& r) { return r.isDead(); }), robotos.end());

		//movement code is the same on the client to try and synchronize without exchanging too much data at once, however mistakes do happen!
		//because the movement of robots is towards the player, and lag can cause the player to be in slightly different places per frame,
		//even if it interpolates to the correct position over time the robots will not track exactly the same way
		if (robotos.size() > 0) {
			for (int i = 0; i < robotos.size() - 1; i++)
			{
				for (int j = i + 1; j < robotos.size(); j++)
				{
					if (robotos[i].rs.getGlobalBounds().intersects(robotos[j].rs.getGlobalBounds()))
					{
						sf::Vector2f displacement = robotos[j].rs.getPosition() - robotos[i].rs.getPosition();	//i to j pos vector
						displacement = displacement / sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
						robotos[i].rs.move(-displacement * frameTime * robotos[i].MOVEMENT_SPEED * 0.05f);
						robotos[j].rs.move(displacement * frameTime * robotos[j].MOVEMENT_SPEED * 0.05f);
					}
				}
			}
		}

		//this update differs for 9s, as the 9s (client) side has no control over killing robots, it must be notified about their deaths from the server!
		//keeping important data like this (because killing them all is a victory condition) on one side only makes it more reliable, and regardless 
		//only the "server" (first amongst equals in this case, the main player) will check for victory conditions
		for (Roboto& r : robotos) {
			
			r.update(tm, playerPos, frameTime);
			//update can result in robots dying if they were pushed into walls
			//we set the state of all robots back to alive to make sure only those that died on the server are truly considered dead
			r.resuscitate();
			//this is way easier than changing the update code and has very low overhead as it only sets a boolean
		}
	
	}



	void draw(sf::RenderWindow& window)
	{
		for (auto r : robotos) {
			if(!r.isDead())
				r.draw(window);
		}
			
	}



	std::vector<sf::Int32> getBodyCount() {
		return bodyCount;
	}



	void synchronizeDestruction(std::vector<sf::Int32> indices) {
		for (int i : indices) 
		{
			if (std::find(serverBodyCount.begin(), serverBodyCount.end(), i) == serverBodyCount.end())
				serverBodyCount.push_back(i);
		}
		
	}
};

#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include "TileMapper.h"
#include "Relay.h"

class Game2B{

	const float UPDATE_INTERVAL = 0.1f, ww = 1920.f, wh = 1080.f;
	const std::string BASEPATH = "../Assets/";
	float late = 0.f;
	

	InputManager iMan;
	Relay* relay;
	uint64_t lastTimestamp = 0;

	sf::RenderWindow& renderWindow;
	sf::View view;
	TileMapper tileMapper;

	sf::Event e;

	void resolveCollisions(float frameTime) 
	{
		for (auto a : tileMapper.level) {
			if (a.collides && player.s2b.sprite.getGlobalBounds().intersects(a.rs.getGlobalBounds())) {
				player.posMin -= player.velocity * frameTime * player.s2b.speed;
			}
		}
	}

public:

	Player player;
	bool first = true;

	Game2B(sf::RenderWindow& w);
	~Game2B();

	void init() {

		iMan.attachObserver2b(player);

		//set up the level
		tileMapper.loadFromFile(BASEPATH + "LevelTileMap.txt", "refinery.png");
		

		view = sf::View(sf::Vector2f(0, 0), sf::Vector2f(ww, wh));
		renderWindow.setView(view);
	}



	void update(float frameTime, sf::RenderWindow& w) {

		iMan.processInput(w, e);

		player.Update(frameTime);
		relay->accumulate(player.stageUpdates2b());

		view.setCenter(player.posMin);
		renderWindow.setView(view);
		resolveCollisions(frameTime);
		draw(renderWindow);

		late += frameTime;
		if (late >= UPDATE_INTERVAL) {

			if (player.outdated) {
				relay->relay();
			}


			/*//load updates
			p2b.stagepdates2b();
			pos2b.Load(updatePacket);

			//send data
			tcpi.send(updatePacket);
			updatePacket.clear();

			g2b.update9s();
			g2b.updateObservers();
			*/
			
			late -= UPDATE_INTERVAL;	//reset the interval
		}
	}



	void receiveUpdates(Msg9S msg) {
		if (lastTimestamp < msg.ms) {
			lastTimestamp = msg.ms;
		}
	}



	void draw(sf::RenderWindow& w) {

		w.draw(tileMapper.background);
		for (auto a : tileMapper.level)
			w.draw(a.rs);

		player.draw(&w);
	}



	void receiveUpdates() {

	}


	void updateObservers() {

	}

	void attachRelay(Relay& relayRef) {
		relay = &relayRef;
	}
};


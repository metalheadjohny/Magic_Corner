#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include "TileMapper.h"
#include "Relay.h"


class Game9S{

	const float UPDATE_INTERVAL = 0.1f, ww = 1920.f * 0.5f, wh = 1080.f * 0.5f;
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

	Game9S(sf::RenderWindow& w);
	~Game9S();



	void init() {

		iMan.attachObserver9s(player);

		//set up the level
		tileMapper.loadFromFile(BASEPATH + "LevelTileMap.txt", "refinery.png");

		//player.posMin = sf::Vector2f(97.f, 97.f) + sf::Vector2f(distanceTo2b, 0);	//set position to 2b's position + offset * angle

		view = sf::View(sf::Vector2f(0, 0), sf::Vector2f(ww, wh));
		renderWindow.setView(view);
	}



	void update(float frameTime, sf::RenderWindow& w)
	{
		iMan.processInput(w, e);

		player.Update(frameTime);
		resolveCollisions(frameTime);

		view.setCenter(player.posMin);
		renderWindow.setView(view);

		//receive updates
		Msg2B msg;
		if (relay->checkFor2BUpdates(msg))
		{
			receiveUpdates(msg);
		}
		else
		{
			relay->divinate();	//based on the current state, keep going as is by prediction and interpolate...
		}

		//send updates
		late += frameTime;
		if (late >= UPDATE_INTERVAL) {


			if (player.stateChanged9s) {
				relay->accumulate9s(player.stageUpdates9s());
				relay->relay();
				player.stateChanged9s = false;
			}

			//reset the interval
			late -= UPDATE_INTERVAL;
		}

		
		draw();
	}



	void receiveUpdates(Msg2B msg) {

		//check if the timestamp is more recent than the last one
		if (lastTimestamp < msg.ms) {
			lastTimestamp = msg.ms;
			player.posMin = sf::Vector2f(msg.x, msg.y);
			player.s2b.current = static_cast<Event2B>(msg.state);
		}
	}



	void draw() {
		
		//this stays the same either way
		renderWindow.draw(tileMapper.background);
		for (auto a : tileMapper.level)
			renderWindow.draw(a.rs);

		//this will change based on network updates
		player.draw(&renderWindow);
	}


	void attachRelay(Relay& relayRef) {
		relay = &relayRef;
	}
};


#pragma once
#include "GameObject.h"
#include "TileMapper.h"
#include <SFML/Graphics.hpp>
#include "Relay.h"

class Game9S{

	const float UPDATE_INTERVAL = 0.33f, ww = 1920.f, wh = 1080.f, distanceTo2b = 30.f;
	const std::string BASEPATH = "../Assets/";

	float angleTo2b;

	InputManager iMan;
	Relay* relay;
	uint64_t lastTimestamp = 0;
	

	sf::RenderWindow& renderWindow;
	sf::View view;
	TileMapper tileMapper;

public:

	
	bool first = true;
	Player player;
	sf::Event e;

	Game9S(sf::RenderWindow& w);
	~Game9S();



	void init() {

		iMan.attachObserver2b(player);

		//set up the level
		tileMapper.loadFromFile(BASEPATH + "LevelTileMap.txt", "refinery.png");

		//player.posMin = sf::Vector2f(97.f, 97.f) + sf::Vector2f(distanceTo2b, 0);	//set position to 2b's position + offset * angle

		view = sf::View(sf::Vector2f(0, 0), sf::Vector2f(ww, wh));
		renderWindow.setView(view);
	}



	void update(float frameTime) {
		player.Update(frameTime);	//make a separate function for p9s update
		view.setCenter(player.posMin);	//keep this centered to 2b not to 9s, it's unnecessary...
		draw();

		
		receiveUpdates(relay->incorporate2b());
		
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


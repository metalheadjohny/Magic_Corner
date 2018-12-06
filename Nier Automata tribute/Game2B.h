#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include "TileMapper.h"
#include "Relay.h"
#include "Roboto.h"
#include "FSM.h"


class Game2B{

	const float UPDATE_INTERVAL = 0.1f, ww = 1920.f * 0.5f, wh = 1080.f * 0.5f;
	const std::string BASEPATH = "../Assets/";
	float late = 0.f;
	
	InputManager iMan;
	Relay* relay;
	uint64_t lastTimestamp = 0;

	sf::RenderWindow& renderWindow;
	sf::View view;
	TileMapper tileMapper;
	Overlord overlord;

	sf::Event e;

	void resolveCollisions(float frameTime) 
	{
		for (auto a : tileMapper.level) {
			if (a.collides && player.s2b.sprite.getGlobalBounds().intersects(a.rs.getGlobalBounds())) {
				player.posMin -= player.velocity * frameTime * player.s2b.speed;
			}
		}

		for (Roboto& r : overlord.robotos) {
			if (!r.isDead() && player.s2b.sprite.getGlobalBounds().intersects(r.rs.getGlobalBounds())) {
				r.exterminate();
				if (!r.hacked)
					player.s2b.hp -= 26.f;
			}
		}
	}

public:

	Player player;
	bool first = true;

	Game2B(sf::RenderWindow& w);
	~Game2B();

	void init() 
	{
		player.s2b.hp = 100.f;
		iMan.attachObserver2b(player);

		tileMapper.loadFromFile(BASEPATH + "LevelTileMap.txt", "refinery.png");
		overlord.fill(tileMapper.enemyPositions);
		
		view = sf::View(sf::Vector2f(0, 0), sf::Vector2f(ww, wh));
		renderWindow.setView(view);
	}



	void update(float frameTime, sf::RenderWindow& w, GameState& gs) 
	{
		overlord.update(tileMapper, player.posMin, frameTime);

		//receive updates from the network
		Msg9S msg;
		if (relay->checkFor9SUpdates(msg))
		{
			receiveUpdates(msg);
		}
		else
		{
			relay->divinate();	//based on the current state, keep going as is by prediction and interpolate...
		}

		//receive updates from player input
		iMan.processInput2B(w, e);

		player.Update(frameTime, overlord.robotos, tileMapper.level);
		resolveCollisions(frameTime);

		view.setCenter(player.posMin);
		renderWindow.setView(view);

		if (player.stateChanged2b) 
		{
			relay->accumulate2b(player.stageUpdates2b());
			relay->relay2B();
			player.stateChanged2b = false;
		}
		else {
			//like this it won't trigger when you start shooting as mouse dir is already sent with that message
			if (player.s2b.current == Event2B::RANGED_ATTACK)
				relay->sendMouseDir(player.mouseDir);
		}

		if (overlord.robotos.size() == 0) {
			gs = GameState::VICTORY;
			first = true;
			relay->relayVictory();
		}

		if (player.s2b.hp <= 0) {
			gs = GameState::DEFEAT;
			first = true;
			relay->relayDefeat();
		}

		draw(renderWindow);
	}



	void receiveUpdates(const Msg9S& msg) 
	{
		if (lastTimestamp < msg.ms) {
			lastTimestamp = msg.ms;
			player.s9s.rot = msg.angle;
			player.s9s.dist = msg.distance;
			player.s9s.current = static_cast<Event9S>(msg.state);
		}
	}



	void draw(sf::RenderWindow& w) 
	{
		w.draw(tileMapper.background);

		for (auto a : tileMapper.level)
			w.draw(a.rs);

		overlord.draw(renderWindow);

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


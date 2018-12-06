#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include "TileMapper.h"
#include "Relay.h"
#include "Roboto.h"
#include "FSM.h"


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
	Overlord overlord;

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



	void init() 
	{
		player.s2b.hp = 100.f;
		iMan.attachObserver9s(player);

		tileMapper.loadFromFile(BASEPATH + "LevelTileMap.txt", "refinery.png");
		overlord.fill(tileMapper.enemyPositions);

		view = sf::View(sf::Vector2f(0, 0), sf::Vector2f(ww, wh));
		renderWindow.setView(view);
	}



	void update(float frameTime, sf::RenderWindow& w, GameState& gs)
	{
		overlord.update(tileMapper, player.posMin, frameTime);

		//receive updates from the network... also I hate switch syntax
		Msg2B msg;
		if (relay->checkFor2BUpdates(msg))
		{
			if (msg.type == MessageType::T_2B) {
				receiveUpdates(msg);
			}
				
			if (msg.type == MessageType::T_2B_MD_ONLY) {
				player.mouseDir = sf::Vector2f(msg.dirX, msg.dirY);
			}
				
			if (msg.type == MessageType::T_2B_VICTORY) {
				gs = GameState::VICTORY;
				first = true;
				return;
			}

			if (msg.type == MessageType::T_2B_DEFEAT) {
				gs = GameState::DEFEAT;
				first = true;
				return;
			}
				
		}
		else
		{
			relay->divinate();	//based on the current state, keep going as is by prediction and interpolate...
		}

		//receive updates from player input
		iMan.processInput9S(w, e);
		
		player.Update(frameTime, overlord.robotos, tileMapper.level, MessageType::T_9S);
		resolveCollisions(frameTime);

		view.setCenter(player.posMin);
		renderWindow.setView(view);


		if (player.stateChanged9s) {
			relay->accumulate9s(player.stageUpdates9s());
			relay->relay9S();
			player.stateChanged9s = false;
		}

		draw();
	}



	void receiveUpdates(const Msg2B& msg) 
	{
		if (lastTimestamp < msg.ms) {
			lastTimestamp = msg.ms;
			player.posMin = sf::Vector2f(msg.x, msg.y);
			player.mouseDir = sf::Vector2f(msg.dirX, msg.dirY);
			player.s2b.current = static_cast<Event2B>(msg.state);
		}
	}



	void draw() {
		
		//this stays the same either way
		renderWindow.draw(tileMapper.background);
		for (auto a : tileMapper.level)
			renderWindow.draw(a.rs);

		overlord.draw(renderWindow);

		//this will change based on network updates
		player.draw(&renderWindow);
	}


	void attachRelay(Relay& relayRef) {
		relay = &relayRef;
	}
};


#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include "TileMapper.h"
#include "Relay.h"
#include "Roboto.h"
#include "FSM.h"


class Game2B{

	const float BOT_UPDATE_INTERVAL = 1.f, ww = 1920.f * 0.5f, wh = 1080.f * 0.5f, KEEPALIVE_INTERVAL = 3.33f;
	float sinceBotUpdate = 0.f, sinceKeepalive = 0.f, sinceRemoteKeepalive = 0.f;
	const std::string BASEPATH = "../Assets/";
	float late = 0.f;
	
	InputManager iMan;
	Relay* relay;
	uint64_t lastTimestamp = 0;

	sf::RenderWindow& renderWindow;
	sf::View view;
	TileMapper tileMapper;
	Overlord overlord;

	Msg9S last9SMessage;
	std::uint64_t now;
	sf::Vector2f deltaPos;
	int STEPS = 20, CURSTEP = 0;

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
		//local updates
		if (!iMan.processInput2B(w, e)) {
			relay->disconnectFrom9S();
			gs = GameState::DEFEAT;
			return;
		}

		player.Update(frameTime, overlord.robotos, tileMapper.level);
		resolveCollisions(frameTime);

		view.setCenter(player.posMin);
		renderWindow.setView(view);

		if (overlord.robotos.size() == 0) {
			gs = GameState::VICTORY;
			relay->relayVictory();
		}

		if (player.s2b.hp <= 0) {
			gs = GameState::DEFEAT;
			relay->relayDefeat();
		}

		draw(renderWindow);

		//net code
		now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

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


		//receive updates from the network
		Msg9S msg;
		if (relay->checkFor9SUpdates(msg))
			receiveUpdates(msg);

		//check if 9s intentionally disconnected
		if (msg.type == MessageType::DISCONNECT) {
			std::cout << "Connection lost. Client has willingly disconnected from this game. Better luck in your further games." << std::endl;
			gs = GameState::DEFEAT;
			return;
		}

		//this is a guess, keepalive method is reliable
		if (relay->isDisconnected())
			std::cout << "Interruption detected during synchronization with 9S. Connection might be compromised." << std::endl;

		std::vector<int> deltaBodyCount = overlord.update2b(tileMapper, player.posMin, frameTime);

		if (deltaBodyCount.size() > 0)
			relay->relayBodyCount(deltaBodyCount);

		sinceBotUpdate += frameTime;
		if (sinceBotUpdate >= BOT_UPDATE_INTERVAL) {
			relay->updateLiveBots(overlord.robotos, now);
			sinceBotUpdate -= BOT_UPDATE_INTERVAL;
		}
		
		sinceKeepalive += frameTime;
		if (sinceKeepalive >= KEEPALIVE_INTERVAL) {
			relay->relayPulse2B();
			sinceKeepalive -= KEEPALIVE_INTERVAL;
		}

		sinceRemoteKeepalive += frameTime;
		if (sinceRemoteKeepalive >= KEEPALIVE_INTERVAL * 3.f) {
			std::cout << "You connection to 9S has deteriorated beyond repair! Sadly, this means game over." << std::endl;
			gs = GameState::DEFEAT;
		}
	}



	void receiveUpdates(const Msg9S& msg) 
	{

		if (msg.type == MessageType::PULSE) {
			sinceRemoteKeepalive = 0.f;
			return;
		}

		if (msg.type == MessageType::T_9S && lastTimestamp < msg.ms) {
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


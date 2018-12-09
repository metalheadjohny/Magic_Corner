#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include "TileMapper.h"
#include "Relay.h"
#include "Roboto.h"
#include "FSM.h"


class SteppyBoi {
	sf::Vector2f deltaPos;
	float elapsed = 0.f;
	float DURATION = 1.f;
	bool done = false;

public:

	bool isDone() { return done; }

	SteppyBoi(sf::Vector2f dPos) 
	{
		deltaPos = dPos;
	}

	void step(float dTime, sf::Vector2f& posToChange) 
	{
		elapsed += dTime;
		if (elapsed >= DURATION) 
		{
			posToChange += (DURATION - (elapsed - dTime)) * deltaPos;
			done = true;
			return;
		}
		posToChange += dTime * deltaPos;
	}
};

class Game9S{

	const float KEEPALIVE_INTERVAL = 3.33f, ww = 1920.f * 0.5f, wh = 1080.f * 0.5f;
	float sinceKeepalive = 0.f, sinceRemoteKeepalive = 0.f;
	const std::string BASEPATH = "../Assets/";
	float late = 0.f;

	InputManager iMan;
	Relay* relay;
	uint64_t lastTimestamp = 0;
	
	sf::RenderWindow& renderWindow;
	sf::View view;
	TileMapper tileMapper;
	Overlord overlord;

	Msg2B last2BMessage;
	std::uint64_t now;

	std::vector<SteppyBoi> adjustments;

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
		now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		if (relay->isDisconnected())
			std::cout << "Interruption detected during synchronization with 2B. Connection might be compromised." << std::endl;
			
		//receive updates from player input
		if (!iMan.processInput9S(w, e)) {
			gs = GameState::DEFEAT;
			return;
		}

		overlord.update9s(tileMapper, player.posMin, frameTime);

		//receive updates from the network... did I mention I hate switch syntax?
		Msg2B msg;
		if (relay->checkFor2BUpdates(msg))	//this is a one size fits all container... can be done cleaner but it's good enough for a problem of this scope
		{
			if (msg.type == MessageType::T_2B) {
				receiveUpdates(msg);
				accountForDelay();	//will set mouse dir at first, but it will be overwritten by subsequen T_2B_MD_ONLY messsages as shooting continues
			}
				
			if (msg.type == MessageType::T_2B_MD_ONLY) {
				player.mouseDir = sf::Vector2f(msg.dirX, msg.dirY);
			}

			if (msg.type == MessageType::T_2B_DEAD_BOTS) {
				overlord.synchronizeDestruction(msg.deadBots);
			}

			if(msg.type == MessageType::T_2B_ACTIVE_BOTS) {
				overlord.synchronizeAction(msg.buds, float(now - msg.ms) / 1000.f, tileMapper, player.posMin);
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

			if (msg.type == MessageType::PULSE) {
				sinceRemoteKeepalive = 0.f;
			}
		}
		
		interpolate(frameTime);
		
		player.Update(frameTime, overlord.robotos, tileMapper.level, MessageType::T_9S);
		resolveCollisions(frameTime);

		view.setCenter(player.posMin);
		renderWindow.setView(view);

		draw();

		if (player.stateChanged9s) {
			relay->accumulate9s(player.stageUpdates9s(now));
			relay->relay9S();
			player.stateChanged9s = false;
		}

		sinceKeepalive += frameTime;
		if (sinceKeepalive >= KEEPALIVE_INTERVAL) {
			relay->relayPulse9S();
			std::cout << "PULSE!" << std::endl;
			sinceKeepalive -= KEEPALIVE_INTERVAL;
		}

		sinceRemoteKeepalive += frameTime;
		if (sinceRemoteKeepalive >= KEEPALIVE_INTERVAL * 3.f) {
			std::cout << "You have disconnected from 2B! Sadly this means game over." << std::endl;
			gs = GameState::DEFEAT;
		}
	}



	void receiveUpdates(const Msg2B& msg) 
	{
		if (lastTimestamp < msg.ms) {
			last2BMessage = msg;
			lastTimestamp = msg.ms;
		}
	}



	void accountForDelay() {

		std::uint64_t deltaMilis = now - last2BMessage.ms;
		float deltaSeconds = float(deltaMilis) / 1000.0f;
		sf::Vector2f deltaPos;

		//account for lag by pushing 2b forward compared to where she was when the message was sent, this will gradually happen
		if ( (last2BMessage.state & (MOVE_DOWN | MOVE_LEFT | MOVE_RIGHT | MOVE_UP)) != 0 ) {

			float factor = 0;//deltaSeconds * player.s2b.speed;
		
			if (last2BMessage.state == MOVE_DOWN)
				deltaPos = sf::Vector2f(0.f, 1.f) * factor;
			if (last2BMessage.state == MOVE_LEFT)
				deltaPos = sf::Vector2f(-1.f, 0.f) * factor;
			if (last2BMessage.state == MOVE_UP)
				deltaPos = sf::Vector2f(0.f, -1.f) * factor;
			if (last2BMessage.state == MOVE_RIGHT)
				deltaPos = sf::Vector2f(1.f, 0.f) * factor;
		}
		
		//adds deltaPos to the interpolation stack to synchronize after movement is over
		if (last2BMessage.state == CHILL) {
			deltaPos = sf::Vector2f(last2BMessage.x, last2BMessage.y) - player.posMin;	//interpolate to the final position instead of snapping	
		}

		SteppyBoi steppyBoi(deltaPos);
		adjustments.push_back(steppyBoi);

		player.mouseDir = sf::Vector2f(last2BMessage.dirX, last2BMessage.dirY);
		player.s2b.current = static_cast<Event2B>(last2BMessage.state);
	}



	void initialRobotSync(std::uint64_t serverTime) 
	{
		std::uint64_t delta = now - serverTime;
		float deltaFloat = float(delta) / 1000.f;
		first = false;

		overlord.update9s(tileMapper, player.posMin, deltaFloat);
	}



	void interpolate(float frameTime) {
		
		for (SteppyBoi& sb : adjustments)
			sb.step(frameTime, player.posMin);

		adjustments.erase(std::remove_if(adjustments.begin(), adjustments.end(), [](SteppyBoi& sb) { return sb.isDone(); }), adjustments.end());
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


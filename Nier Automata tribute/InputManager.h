#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>

class Player;

enum Event {
	CHILL,
	MOVE_RIGHT,
	MOVE_LEFT,
	MOVE_UP, 
	MOVE_DOWN,
	DODGE,
	JUMP_ATTACK,
	RANGED_ATTACK,
	MELEE_ATTACK,
	ULTIMATE,
	SELF_DESTRUCT
};

enum Event9s {
	CHILL,
	ROTATE,
	HACK,
	SHIELD,
	PHASE,
	PUSH,
};


class InputManager{

	std::vector<Player*> observers;

public:

	std::map<Event, sf::Keyboard::Key> keyCommandMap;

	InputManager() {
		//keyCommandMap = std::map<Event, sf::Keyboard::Key>();

		keyCommandMap.insert(std::make_pair(MOVE_UP, sf::Keyboard::Key::W));
		keyCommandMap.insert(std::make_pair(MOVE_DOWN, sf::Keyboard::Key::S));
		keyCommandMap.insert(std::make_pair(MOVE_LEFT, sf::Keyboard::Key::A));
		keyCommandMap.insert(std::make_pair(MOVE_RIGHT, sf::Keyboard::Key::D));
		keyCommandMap.insert(std::make_pair(DODGE, sf::Keyboard::Key::Space));
		keyCommandMap.insert(std::make_pair(MELEE_ATTACK, sf::Keyboard::Key::E));
		keyCommandMap.insert(std::make_pair(RANGED_ATTACK, sf::Keyboard::Key::Q));
		keyCommandMap.insert(std::make_pair(ULTIMATE, sf::Keyboard::Key::R));
		keyCommandMap.insert(std::make_pair(SELF_DESTRUCT, sf::Keyboard::Key::Enter));
	}

	~InputManager();
	

	void attachObserver(Player& go){
		observers.push_back(&go);
	}


	void detachObserver(const Player& go) {
		observers.erase(std::remove(observers.begin(), observers.end(), &go), observers.end());
	}


	void notifyObservers(const InputManager& inputManager, const Event& event);
	

	void processInput(sf::RenderWindow& window, sf::Event& e);
};


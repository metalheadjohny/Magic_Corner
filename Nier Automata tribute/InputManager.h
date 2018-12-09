#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>

class Player;

enum Event2B {
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
	SELF_DESTRUCT,
	NONE2B
};

enum Event9S {
	CHILL9S,
	ROTATE_POS,
	ROTATE_NEG,
	HACK,
	SHIELD,
	PHASE,
	PUSH,
	NONE9S
};


class InputManager{

	std::vector<Player*> observers2b, observers9s;

public:

	std::map<Event2B, sf::Keyboard::Key> keyCmdMap2b;
	std::map<Event9S, sf::Keyboard::Key> keyCmdMap9s;

	InputManager() 
	{
		//2b commands
		keyCmdMap2b.insert(std::make_pair(MOVE_UP, sf::Keyboard::Key::W));
		keyCmdMap2b.insert(std::make_pair(MOVE_DOWN, sf::Keyboard::Key::S));
		keyCmdMap2b.insert(std::make_pair(MOVE_LEFT, sf::Keyboard::Key::A));
		keyCmdMap2b.insert(std::make_pair(MOVE_RIGHT, sf::Keyboard::Key::D));
		keyCmdMap2b.insert(std::make_pair(DODGE, sf::Keyboard::Key::Space));
		keyCmdMap2b.insert(std::make_pair(MELEE_ATTACK, sf::Keyboard::Key::E));
		keyCmdMap2b.insert(std::make_pair(RANGED_ATTACK, sf::Keyboard::Key::Q));
		keyCmdMap2b.insert(std::make_pair(ULTIMATE, sf::Keyboard::Key::R));
		keyCmdMap2b.insert(std::make_pair(SELF_DESTRUCT, sf::Keyboard::Key::Enter));

		//9s commands
		keyCmdMap9s.insert(std::make_pair(ROTATE_POS, sf::Keyboard::Key::Left));
		keyCmdMap9s.insert(std::make_pair(ROTATE_NEG, sf::Keyboard::Key::Right));
		keyCmdMap9s.insert(std::make_pair(HACK, sf::Keyboard::Key::Q));
		keyCmdMap9s.insert(std::make_pair(SHIELD, sf::Keyboard::Key::W));
		keyCmdMap9s.insert(std::make_pair(PUSH, sf::Keyboard::Key::E));
		keyCmdMap9s.insert(std::make_pair(PHASE, sf::Keyboard::Key::R));
	}

	~InputManager();
	

	void attachObserver2b(Player& player){
		observers2b.push_back(&player);
	}

	void detachObserver2b(const Player& player) {
		observers2b.erase(std::remove(observers2b.begin(), observers2b.end(), &player), observers2b.end());
	}

	void attachObserver9s(Player& player) {
		observers9s.push_back(&player);
	}

	void detachObserver9s(const Player& player) {
		observers9s.erase(std::remove(observers9s.begin(), observers9s.end(), &player), observers9s.end());
	}

	void notifyObservers2b(const InputManager& inputManager, const Event2B& event);
	
	void notifyObservers9s(const InputManager& inputManager, const Event9S& event);
	
	bool processInput2B(sf::RenderWindow& window, sf::Event& e);
	bool processInput9S(sf::RenderWindow& window, sf::Event& e);
};


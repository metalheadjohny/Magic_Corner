#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>

class Player;

enum Event {
	CHILL,
	MOVE_RIGHT,
	MOVE_LEFT,
	DODGE,
	JUMP,
	JUMP_ATTACK,
	RANGED_ATTACK,
	MELEE_ATTACK,
	ULTIMATE,
	SELF_DESTRUCT
};


class InputManager{

	std::vector<Player*> observers;

public:

	std::map<Event, sf::Keyboard::Key> keyCommandMap;

	InputManager() {
		//keyCommandMap = std::map<Event, sf::Keyboard::Key>();

		keyCommandMap.insert(std::make_pair(MOVE_LEFT, sf::Keyboard::Key::A));
		keyCommandMap.insert(std::make_pair(DODGE, sf::Keyboard::Key::S));
		keyCommandMap.insert(std::make_pair(MOVE_RIGHT, sf::Keyboard::Key::D));
		keyCommandMap.insert(std::make_pair(JUMP, sf::Keyboard::Key::Space));
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




	void processInput(sf::Window& window, sf::Event& e){

		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::TextEntered:
				printf("%c", e.text.unicode);
				break;
			case sf::Event::KeyPressed:
				if (e.key.code == sf::Keyboard::Escape)
					window.close();
			}
		}
		//turn this into move_left, move_right events etc... 

		if (sf::Keyboard::isKeyPressed(keyCommandMap.at(MOVE_LEFT))) {
			notifyObservers(*this, MOVE_LEFT);
			return;
		}
		if (sf::Keyboard::isKeyPressed(keyCommandMap.at(MOVE_RIGHT))) {
			notifyObservers(*this, MOVE_RIGHT);
			return;
		}
		if (sf::Keyboard::isKeyPressed(keyCommandMap.at(JUMP))) {
			notifyObservers(*this, JUMP);
			return;
		}
		if (sf::Keyboard::isKeyPressed(keyCommandMap.at(DODGE))) {
			notifyObservers(*this, DODGE);
			return;
		}
		if (sf::Keyboard::isKeyPressed(keyCommandMap.at(MELEE_ATTACK))) {
			notifyObservers(*this, MELEE_ATTACK);
			return;
		}
		if (sf::Keyboard::isKeyPressed(keyCommandMap.at(RANGED_ATTACK))) {
			notifyObservers(*this, RANGED_ATTACK);
			return;
		}

		notifyObservers(*this, CHILL);

		/*
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

		}
		*/

	}
};


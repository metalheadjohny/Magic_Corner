#include "InputManager.h"
#include "GameObject.h"


InputManager::~InputManager(){}

void InputManager::notifyObservers2b(const InputManager& inputManager, const Event2B& event) {
	for (auto o2b : observers2b)
		o2b->OnNotify2b(inputManager, event);
}

void InputManager::notifyObservers9s(const InputManager& inputManager, const Event9s& event) {
	for (auto o9s : observers9s)
		o9s->OnNotify9s(inputManager, event);
}



void InputManager::processInput(sf::RenderWindow& window, sf::Event& e) {


	sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition());
	sf::Vector2f mousePosFloat = sf::Vector2f((float)mouse_world.x, (float)mouse_world.y);

	for (auto a : observers2b)
		a->mousePos = mousePosFloat;

	for (auto a : observers9s)
		a->mousePos = mousePosFloat;

	while (window.pollEvent(e)) {
		switch (e.type) {

		case sf::Event::Closed:
			window.close();
			break;

		case sf::Event::KeyPressed:
			if (e.key.code == sf::Keyboard::Escape)
				window.close();
			break;
		}
	}

	for(auto keyCmd : keyCmdMap2b) {
		if(sf::Keyboard::isKeyPressed(keyCmd.second)) {
			notifyObservers2b(*this, keyCmd.first);
			return;
		}
	}

	for (auto keyCmd : keyCmdMap9s) {
		if (sf::Keyboard::isKeyPressed(keyCmd.second)) {
			notifyObservers9s(*this, keyCmd.first);
			return;
		}
	}

	notifyObservers2b(*this, CHILL);
	notifyObservers9s(*this, CHILL9S);
	
}
#include "InputManager.h"
#include "GameObject.h"


InputManager::~InputManager()
{
}

void InputManager::notifyObservers(const InputManager& inputManager, const Event& event) {
	for (int i = 0; i < observers.size(); i++)
		observers.at(i)->OnNotify(inputManager, event);
}

void InputManager::processInput(sf::RenderWindow& window, sf::Event& e) {


	sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition());
	sf::Vector2f mousePosFloat = sf::Vector2f((float)mouse_world.x, (float)mouse_world.y);

	for (auto a : observers)
		a->mousePos = mousePosFloat;


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

	for (auto keyCmd : keyCommandMap) {
		if (sf::Keyboard::isKeyPressed(keyCmd.second)) {
			notifyObservers(*this, keyCmd.first);
			return;
		}
	}

	notifyObservers(*this, CHILL);
}
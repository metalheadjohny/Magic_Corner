#include "InputManager.h"
#include "GameObject.h"


InputManager::~InputManager()
{
}

void InputManager::notifyObservers(const InputManager& inputManager, const Event& event) {
	for (int i = 0; i < observers.size(); i++) {
		observers.at(i)->OnNotify(inputManager, event);
	}
}
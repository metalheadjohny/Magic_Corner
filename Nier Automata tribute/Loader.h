#pragma once
#include <vector>
#include <string>
#include <map>
#include "InputManager.h"

class Player;

class Loader{

public:
	Loader(){}
	~Loader(){}

	bool LoadLevel(const std::string& levelFilePath, Player& players, const std::vector<std::string>& soundPaths, const std::string& keyBindsPath);
	bool LoadMap(const std::string& levelFilePath);
	bool LoadCharacter(Player& player);
	bool LoadSound(const std::vector<std::string>& soundPaths);
	bool LoadKeyBindings(const std::string& keyBindsPath, std::map<Event2B, sf::Keyboard::Key>& keyCommandMap);	//

};


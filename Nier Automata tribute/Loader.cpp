#include "Loader.h"
#include <fstream>

bool Loader::LoadLevel(const std::string& levelFilePath, Player& player, const std::vector<std::string>& soundPaths, const std::string& keyBindsPath) {
	
	LoadMap(levelFilePath);
	LoadCharacter(player);
	LoadSound(soundPaths);
	
	return true;
}

bool Loader::LoadMap(const std::string& levelFilePath) {
	return true;
}



bool Loader::LoadSound(const std::vector<std::string>& soundPaths) {
	return true;
}


bool Loader::LoadCharacter(Player& player) {
	return true;
}


bool Loader::LoadKeyBindings(const std::string& keyBindsPath, std::map<Event, sf::Keyboard::Key>& keyCommandMap) {

	std::string line;
	std::ifstream fileStream;
	fileStream.open(keyBindsPath);

	if (!fileStream.is_open())
		return false;

	while (std::getline(fileStream, line)) {
		
		std::string cmdName = line.substr(0, line.find_first_of('='));
		char cmdKey = line.substr(line.find_first_of('=') + 1, line.size()).at(0);

		keyCommandMap.at(static_cast<Event>(std::stoi(cmdName))) = static_cast<sf::Keyboard::Key>(cmdKey);
	}
		

	return true;
}
#pragma once
#include "SpriteSheetAnimation.h"
#include <string>
#include <vector>
#include <fstream>

enum class AnimType { IDLE, RUN_LEFT, RUN_RIGHT, JUMP, FIGHT_LEFT, FIGHT_RIGHT, SPECIAL };


class Animation{


	bool loadPngs() {

		for (int i = 0; i < paths.size(); i++) {

			sf::Texture tex;
			tex.loadFromFile(basePath + paths[i]);
			textures.push_back(tex);
		}

		return true;
	}

public:

	Animation() {
	}

	~Animation() {

	}

	sf::Sprite sprite;

	std::string basePath;
	std::vector<std::string> paths;
	std::vector<sf::Texture> textures;
	
	float animCurTime = 0.f, animMaxTime = 1.f;
	short unsigned int counter = 0;



	bool initFromVector(const std::string& basePath, const std::vector<std::string> paths) {
		this->basePath = basePath;
		this->paths = paths;

		return loadPngs();
	}



	bool initFromFile(const std::string& basePath, const std::string& pathToList) {
		
		this->basePath = basePath;

		std::ifstream fStream(pathToList);
		std::string fName;

		while (std::getline(fStream, fName))
			paths.push_back(fName);
		
		return loadPngs();
	}

	

	void play(AnimType t, const float dTime) {
		
		switch (t) {
		case AnimType::IDLE:

			break;
		case AnimType::JUMP:

			break;

		case AnimType::RUN_LEFT:

			break;
		case AnimType::RUN_RIGHT:

			break;

		}

		animCurTime  += dTime;
		if (animCurTime >= animMaxTime) {
			animCurTime -= animMaxTime;
			counter++;
			
			sprite.setTextureRect(sf::IntRect(0, 0, textures[counter].getSize().x, textures[counter].getSize().y));
			sprite.setTexture(textures[counter]);
		}
	}

};


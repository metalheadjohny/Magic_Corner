#pragma once
#include <fstream>
#include <vector>
#include <string>
#include "GameObject.h"

class TileMapper{

	sf::Vector2f TILE_SIZE = sf::Vector2f(96.f, 96.f);
	std::map<char, sf::Texture> texMap;
	sf::Texture bgTex;
	const std::string TILEPATH = "../Assets/Walls/";

	void addTile(int x, int y, char t) {

		if (t == '2') {
			enemyPositions.push_back(sf::Vector2f(y * TILE_SIZE.x, x * TILE_SIZE.y));
			return;
		}
		InanimateObject obj;
		obj.size = TILE_SIZE;
		obj.posMin = sf::Vector2f(y * TILE_SIZE.x, x * TILE_SIZE.y);
		obj.rs.setTexture( &(texMap.at(t)) );
		obj.rs.setSize(obj.size);
		obj.rs.setPosition(obj.posMin);
		if (t == '4' || t == '5')
			obj.collides = false;
		level.push_back(obj);
	}

public:
	TileMapper();
	~TileMapper();

	std::vector<InanimateObject> level;
	std::vector<sf::Vector2f> enemyPositions;
	sf::Sprite background;
	

	void loadFromFile(const std::string& path, const std::string& pathBackground) {
		
		std::ifstream file(path);
		std::vector<std::string> strings;
		std::string temp;

		while (std::getline(file, temp))
			strings.push_back(temp);
			
		temp.clear();

		//assume equal length for all rows... it should be like that no reason why not...
		level.reserve(strings.size() * strings.at(0).size());

		for (int i = 0; i < strings.size(); ++i)
			for (int j = 0; j < strings.at(i).size(); j++)
				if (strings.at(i).at(j) != '0')
					addTile(i, j, strings.at(i).at(j));

		//add background the size of the map...
		sf::Vector2f bgSize(TILE_SIZE.x * strings.at(0).size(), TILE_SIZE.y * strings.size());

		bgTex.loadFromFile(TILEPATH + pathBackground);
		bgTex.setRepeated(true);
		background.setTexture(bgTex);
		background.setScale(sf::Vector2f(bgSize.x / (float)bgTex.getSize().x, bgSize.y / (float)bgTex.getSize().y));
		//background.setTextureRect(sf::IntRect(0, 0, bgSize.x, bgSize.y));
	}

};


#pragma once
#include <SFML/Graphics.hpp>

class SpriteSheetAnimation
{
public:

	std::string path;
	sf::Texture tex;
	sf::Sprite* sprite;
	
	sf::Vector2u gridSize, cellSize;
	float swapInterval, duration, curTime = 0.f;
	short unsigned int numSprites, spriteIndex;

	SpriteSheetAnimation(sf::Sprite* sprite) {
		this->sprite = sprite;
	}

	void init(const std::string& path, const sf::Vector2u gridSize, const float duration, const int numSprites = -1) {
		
		this->path = path;
		this->gridSize = gridSize;
		this->duration = duration;
		
		tex.loadFromFile(path);
		sprite->setTexture(tex);

		cellSize.x = tex.getSize().x / gridSize.x;
		cellSize.y = tex.getSize().y / gridSize.y;
		sprite->setTextureRect(sf::IntRect(0, 0, cellSize.x, cellSize.y ));	
		
		if (numSprites == -1)
			this->numSprites = gridSize.x * gridSize.y;
		else
			this->numSprites = numSprites;

		swapInterval = duration / (float)this->numSprites;
	}

	~SpriteSheetAnimation() {

	}

	void play(float dTime) {
		
		curTime += dTime;

		if (curTime >= duration)
			curTime -= duration;

		float wat  = curTime / swapInterval;
		spriteIndex = (unsigned short int)floor(wat);
		unsigned int row = (floor) (spriteIndex % gridSize.x);
		unsigned int column = (floor) (spriteIndex / gridSize.x);

		sprite->setTextureRect(sf::IntRect(cellSize.x * row, cellSize.y * column, cellSize.x, cellSize.y));
	}


};


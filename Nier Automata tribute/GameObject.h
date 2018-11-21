#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "InputManager.h"

static float gravity = 9.81f;

class GameObject{

public:
	sf::Vector2f posMin, size;
	float rotation;
	
	GameObject();
	~GameObject();
};



class TriggerObject : GameObject {

public:
	virtual void Publish(){}
};



class InanimateObject : public GameObject {

public:
	sf::RectangleShape rs;
};





class InputManager;
enum Event;

class Player : GameObject {

public:

	const float jumpDuration = 1.0f;
	float elapsed = 0.0f;
	bool isJumping = false;

	float maxHP, maxSpeed, hp, speed;
	float mDCD, mBUCD, mSDCD, cDCD, cBUCD, cSDCD;	//max and current dodge, bot ult and self destruct cooldowns
	sf::Vector2f velocity;

	Event current = Event::CHILL, old = Event::CHILL;

	std::map<std::string, SpriteSheetAnimation> animap;
	sf::Sprite sprite;
	SpriteSheetAnimation ssa;

	Player(float maxHP, float maxSpeed)
		: maxHP(maxHP), hp(maxHP), maxSpeed(maxSpeed), speed(maxSpeed), mDCD(3.f), mBUCD(10.f), mSDCD(60.f), ssa(SpriteSheetAnimation(&sprite))
	{
		cDCD = cBUCD = cSDCD = 0.f;
	}


	void loadSSA(const std::string& path, const sf::Vector2u& gridSize, const float swapInterval, const std::string& name) {
		if (animap.find(name) == animap.end()) {
			ssa.init(path, gridSize, swapInterval);
			animap.insert(std::make_pair(name, ssa));
		}
	}

	void removeSSA(const std::string& name) {
		animap.erase(name);
	}

	void OnNotify(const InputManager& iMan, const Event& cmd);
	void Update(float dTime);
	void Jump(float dTime);
};












/* not using this any more
Player(float maxHP, float maxSpeed, const std::string& basePath, const std::string& listPath)
	: maxHP(maxHP), hp(maxHP), maxSpeed(maxSpeed), s(maxSpeed), mDCD(3.f), mBUCD(10.f), mSDCD(60.f), cat(AnimType::IDLE)
{
	cDCD = cBUCD = cSDCD = 0.f;
	loadAnim(basePath, listPath);
}

bool loadAnim(const std::string& basePath, const std::string& listPath) {
	bool result =  animation.initFromFile(basePath, listPath);
	return result;
}
*/
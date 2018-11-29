#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "InputManager.h"
#include "messageStructs.h"
#include "Projectile.h"
#include <chrono>

class InputManager;
enum Event;



class GameObject{

public:
	sf::Vector2f posMin, size;
	sf::RectangleShape rs;

	float rotation;
	
	GameObject();
	~GameObject();
};



class InanimateObject : public GameObject {

public:
	sf::RectangleShape rs;
	bool collides = true;
};



class Player : public GameObject {

public:
	float elapsed = 0.0f; 
	
	int rot;	//might not be necessary @TODO review

	float maxHP, maxSpeed, hp, speed;
	float mDCD, mBUCD, mSDCD, cDCD, cBUCD, cSDCD;	//max and current dodge, bot ult and self destruct cooldowns
	sf::Vector2f velocity, mouseDir, mousePos;

	float rot9s, dist9s;

	Event current = Event::CHILL, old = Event::CHILL;
	Event9s current9s = Event9s::CHILL, old = Event9s::CHILL;

	std::map<std::string, SpriteSheetAnimation> animap;
	sf::Sprite sprite;
	SpriteSheetAnimation ssa;
	ProjectileManager desAndTroy;



	Player(float maxHP, float maxSpeed)
		: maxHP(maxHP), hp(maxHP), maxSpeed(maxSpeed), 
		speed(maxSpeed), mDCD(3.f), mBUCD(10.f), mSDCD(60.f),
		ssa(SpriteSheetAnimation(&sprite))

	{
		cDCD = cBUCD = cSDCD = 0.f;
		mousePos = sf::Vector2f(0, 0);
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



	void draw(sf::RenderWindow* window) {
		window->draw(sprite);
		for (auto a : desAndTroy.bullets) {
			window->draw(a.rs);
		}
	}



	void stageUpdates2b(Msg2B& updateStruct) 
	{
		updateStruct.type = MsgType::T_2B;
		updateStruct.x = posMin.x;
		updateStruct.y = posMin.y;
		updateStruct.state = current;
		updateStruct.ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		
	}



	void stageUpdates9s(Msg9S& updateStruct)
	{
		updateStruct.type = MsgType::T_9S;
		updateStruct.angle = rot9s;
		updateStruct.distance = dist9s;
		updateStruct.state = current9s;
	}


	void OnNotify(const InputManager& iMan, const Event& cmd);
	void Update(float dTime);
	//void Jump(float dTime);
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
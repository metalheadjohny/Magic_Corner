#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "InputManager.h"
#include "messageStructs.h"
#include "Projectile.h"
#include <chrono>

class InputManager;


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



struct State2B 
{
	int rot;
	float maxHP = 100.f, maxSpeed = 300.f, hp = 100.f, speed = 300.f, elapsed = 0.0f;
	Event2B current = Event2B::CHILL, old = Event2B::CHILL;

	std::map<std::string, SpriteSheetAnimation> animap;
	SpriteSheetAnimation ssa;
	sf::Sprite sprite;

	ProjectileManager desAndTroy;

	State2B() : ssa(&sprite){}
};



struct State9S {

	float ANGULAR_SPEED = 60.f, PUSH_CD = 5.f, HACK_CD = 5.f, sincePush = 5.f, sinceHack = 5.f;
	float rot = 0, dist = 100;
	sf::Vector2f offset9s = sf::Vector2f(100.f, 0.f);
	const sf::Vector2f refDir = sf::Vector2f(1.0f, 0.0f);

	Event9s current = Event9s::CHILL9S, old = Event9s::CHILL9S;

	std::map<std::string, SpriteSheetAnimation> animap;
	SpriteSheetAnimation ssa;
	sf::Sprite sprite;

	State9S() : ssa(&sprite) {}
};



class Player : public GameObject {

	const std::string BASEPATH = "../Assets/";

public:

	State2B s2b;
	State9S s9s;

	bool stateChanged2b = false, stateChanged9s = false;
	sf::Vector2f velocity, mouseDir, mousePos;


	Player(float maxHP, float maxSpeed){
		
		mousePos = sf::Vector2f(0, 0);
		posMin = sf::Vector2f(97.f, 97.f);

		//load 2b
		loadSSA(BASEPATH + "2B exivus sprites\\limit_break.png", sf::Vector2u(3, 17), 5.f, "lb");
		loadSSA(BASEPATH + "2B exivus sprites\\move_left.png", sf::Vector2u(1, 1), 10.f, "walk_left");
		loadSSA(BASEPATH + "2B exivus sprites\\move_right.png", sf::Vector2u(1, 1), 10.f, "walk_right");
		loadSSA(BASEPATH + "2B exivus sprites\\magic_attack.png", sf::Vector2u(3, 2), 10.f, "ranged_attack");
		loadSSA(BASEPATH + "2B exivus sprites\\sorted\\m2b_pat_bot.png", sf::Vector2u(20, 1), 2.f, "idle");
		
		//load 9s
		loadSSA9s(BASEPATH + "Sprites\\nines.png", sf::Vector2u(1, 2), 1.f, "9s_idle");
		s9s.sprite.setOrigin(s9s.sprite.getLocalBounds().width * 0.5f, s9s.sprite.getLocalBounds().height * 0.5f * 0.5f);
	}



	void loadSSA(const std::string& path, const sf::Vector2u& gridSize, const float swapInterval, const std::string& name) {
		if (s2b.animap.find(name) == s2b.animap.end()) {
			s2b.ssa.init(path, gridSize, swapInterval);
			s2b.animap.insert(std::make_pair(name, s2b.ssa));
		}
	}



	void loadSSA9s(const std::string& path, const sf::Vector2u& gridSize, const float swapInterval, const std::string& name) {
		if (s9s.animap.find(name) == s9s.animap.end()) {
			s9s.ssa.init(path, gridSize, swapInterval);
			s9s.animap.insert(std::make_pair(name, s9s.ssa));
		}
	}



	/*void removeSSA(const std::string& name) {
		s2b.animap.erase(name);
	}*/



	void draw(sf::RenderWindow* window) {
		window->draw(s2b.sprite);
		window->draw(s9s.sprite);
		for (auto a : s2b.desAndTroy.bullets) {
			window->draw(a.rs);
		}
		window->draw(s9s.sprite);
	}



	Msg2B stageUpdates2b()
	{
		Msg2B updateStruct;
		updateStruct.type = PlayerType::T_2B;
		updateStruct.x = posMin.x;
		updateStruct.y = posMin.y;
		updateStruct.state = s2b.current;
		updateStruct.ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		return updateStruct;
	}



	Msg9S stageUpdates9s()
	{
		Msg9S updateStruct;
		updateStruct.type = PlayerType::T_9S;
		updateStruct.angle = s9s.rot;
		updateStruct.distance = s9s.dist;
		updateStruct.state = s9s.current;
		updateStruct.push = s9s.current == Event9s::PUSH ? s9s.offset9s : sf::Vector2f(0, 0);
		updateStruct.hack = s9s.current == Event9s::HACK ? s9s.offset9s : sf::Vector2f(0, 0);
		return updateStruct;
	}


	void OnNotify2b(const InputManager& iMan, const Event2B& cmd);
	void OnNotify9s(const InputManager& iMan, const Event9s& cmd);
	void Update(float dTime);
};
#pragma once
#include "GameObject.h"
#include "TileMapper.h"
#include <SFML/Graphics.hpp>

class Game9S{

	const float UPDATE_INTERVAL = 0.33f, ww = 1920.f, wh = 1080.f, distanceTo2b = 30.f;
	const std::string BASEPATH = "../Assets/";
	sf::View view;

	float angleTo2b;

	InputManager iMan;
	sf::RenderWindow& renderWindow;
	TileMapper tileMapper;

public:

	bool first = true;
	Player p9s;

	Game9S(sf::RenderWindow& w);
	~Game9S();



	void init() {

		iMan.attachObserver(p9s);

		//set up the level
		tileMapper.loadFromFile(BASEPATH + "LevelTileMap.txt", "refinery.png");

		//set up 9s
		p9s.loadSSA(BASEPATH + "2B exivus sprites\\limit_break.png", sf::Vector2u(4, 4), 1.f, "idle");
		p9s.posMin = sf::Vector2f(97.f, 97.f) + sf::Vector2f(distanceTo2b, 0);	//set position to 2b's position + offset * angle

		view = sf::View(sf::Vector2f(0, 0), sf::Vector2f(ww, wh));
		renderWindow.setView(view);
	}



	void update(float frameTime) {
		p9s.Update(frameTime);	//make a separate function for p9s update
		view.setCenter(p9s.posMin);	//keep this centered to 2b not to 9s, it's unnecessary...
		draw();
	}



	void draw() {
		
		//this stays the same either way
		renderWindow.draw(tileMapper.background);
		for (auto a : tileMapper.level)
			renderWindow.draw(a.rs);

		//this will change based on network updates
		p9s.draw(&renderWindow);
	}



	void update2b() {

	}
};


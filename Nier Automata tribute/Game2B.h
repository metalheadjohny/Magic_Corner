#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include "TileMapper.h"

class Game2B{

	const float UPDATE_INTERVAL = 0.33f, ww = 1920.f, wh = 1080.f;
	const std::string BASEPATH = "../Assets/";
	float late = 0.f;
	
	InputManager iMan;

	sf::RenderWindow& renderWindow;
	sf::View view;
	
	TileMapper tileMapper;
	sf::Event e;

	void resolveCollisions(float frameTime) 
	{
		for (auto a : tileMapper.level) {
			if (a.collides && p2b.sprite.getGlobalBounds().intersects(a.rs.getGlobalBounds())) {
				p2b.posMin -= p2b.velocity * frameTime * p2b.speed;
			}
		}
	}

public:

	Player p2b;
	bool first = true;

	Game2B(sf::RenderWindow& w);
	~Game2B();

	void init() {

		iMan.attachObserver(p2b);

		//set up the level
		tileMapper.loadFromFile(BASEPATH + "LevelTileMap.txt", "refinery.png");

		//set up 2b
		p2b.loadSSA(BASEPATH + "2B exivus sprites\\limit_break.png", sf::Vector2u(3, 17), 5.f, "lb");
		p2b.loadSSA(BASEPATH + "2B exivus sprites\\move_left.png", sf::Vector2u(1, 1), 10.f, "walk_left");
		p2b.loadSSA(BASEPATH + "2B exivus sprites\\move_right.png", sf::Vector2u(1, 1), 10.f, "walk_right");
		p2b.loadSSA(BASEPATH + "2B exivus sprites\\magic_attack.png", sf::Vector2u(3, 2), 10.f, "ranged_attack");
		p2b.loadSSA(BASEPATH + "2B exivus sprites\\sorted\\m2b_pat_bot.png", sf::Vector2u(20, 1), 2.f, "idle");
		p2b.posMin = sf::Vector2f(97.f, 97.f);

		view = sf::View(sf::Vector2f(0, 0), sf::Vector2f(ww, wh));
		renderWindow.setView(view);
	}



	void update(float frameTime, sf::RenderWindow& w) {

		iMan.processInput(w, e);

		p2b.Update(frameTime);

		view.setCenter(p2b.posMin);
		resolveCollisions(frameTime);
		draw(renderWindow);

		late += frameTime;
		if (late >= UPDATE_INTERVAL) {

			//load updates
			p2b.stageUpdates(pos2b);	//this is ugly af, make a staging class that does all this
			pos2b.Load(updatePacket);

			//send data
			tcpi.send(updatePacket);
			updatePacket.clear();

			g2b.update9s();
			g2b.updateObservers();

			//reset the interval
			late -= UPDATE_INTERVAL;
		}
	}



	void draw(sf::RenderWindow& w) {

		w.draw(tileMapper.background);
		for (auto a : tileMapper.level)
			w.draw(a.rs);

		p2b.draw(&w);
	}



	void receiveUpdates() {

	}



	void update9s() {

	}



	void updateObservers() {

	}
};


#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>



class VictoryScreen {

	sf::Font f;
	sf::Text t;
	sf::Sprite sprite;

public:

	VictoryScreen() {};
	~VictoryScreen() {};



	void setUp(sf::Sprite& loadingSprite, sf::RenderWindow& w, std::string message) {

		if (!f.loadFromFile("../Assets/Fonts/Frostbite.otf"))
			exit(369);

		t.setFont(f);
		t.setString(message);
		t.setCharacterSize(86);
		t.setFillColor(sf::Color(160, 0, 0, 255));
		t.setStyle(sf::Text::Bold);
		t.setOrigin(t.getLocalBounds().width * 0.5f, t.getLocalBounds().height * 0.5f);
		t.setPosition(w.getSize().x * 0.5f, w.getSize().y * 0.5f);

		sprite = loadingSprite;


		//resize and keep ar...
		/*
		float AR = sprite.getLocalBounds().width / sprite.getLocalBounds().height;
		float newWS = ;
		float newHS =;

		float finWS, finHS;
		if (newWS <= newHS) {
			newHS = newWS / AR;
		}
		else {
			newWS = AR * newHS;
		}*/

		
		sprite.setScale(w.getSize().x / sprite.getLocalBounds().width, w.getSize().y / sprite.getLocalBounds().height);
		sprite.setOrigin(sprite.getLocalBounds().width * 0.5f, sprite.getLocalBounds().height * 0.5f);
		sprite.setPosition(w.getSize().x * 0.5f, w.getSize().y * 0.5f);
	}



	bool update(sf::RenderWindow& w) {
		draw(w);
		return processUpdates(w);
	}



	void draw(sf::RenderWindow& w) {
		w.draw(sprite);
		w.draw(t);
	}



	bool processUpdates(sf::RenderWindow& w) {

		sf::Event e;
		bool goBack = false;

		while (w.pollEvent(e)) {
			switch (e.type) {

			case sf::Event::Closed:
				goBack = true;
				break;
			case sf::Event::KeyPressed:
				if (e.key.code == sf::Keyboard::Escape)
					goBack = true;
				break;
			}
		}

		return goBack;
	}

};
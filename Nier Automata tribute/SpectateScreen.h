#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>



class SpectateScreen {

	sf::Sprite sprite;

public:

	SpectateScreen() {};
	~SpectateScreen() {};

	void setUp(sf::Sprite& loadingSprite, sf::RenderWindow& w, std::string message = "") {

		sprite = loadingSprite;
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
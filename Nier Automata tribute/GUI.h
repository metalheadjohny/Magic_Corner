#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "FSM.h"

enum GameState;

class Button {

	GameState tag;

public:

	sf::RectangleShape btnRect;
	sf::Text btnText;

	Button* parent;
	std::vector<Button> children;

	bool focused;

	Button(sf::Text& t, sf::Vector2f tlp, Button* p, GameState type) {

		btnText = sf::Text(t);		//btnText = sf::Text(text); not sure if copy will work?
		tag = type;
		
		//btnText.setOrigin(btnText.getLocalBounds().left + btnText.getLocalBounds().width / 2.0f,
		//		btnText.getLocalBounds().top + btnText.getLocalBounds().height / 2.0f
		//	);

		btnRect.setSize(sf::Vector2f(btnText.getLocalBounds().width, btnText.getLocalBounds().height));
		btnRect.setFillColor(sf::Color::Black);
		btnRect.setPosition(tlp);
		btnText.setPosition(sf::Vector2f(btnRect.getGlobalBounds().left, btnRect.getGlobalBounds().top));
		parent = p;
	}

	Button() {};
	
	void updateText(sf::Text text) {
		
		btnText = text;		//btnText = sf::Text(text); not sure if copy will work?
		btnText.setOrigin(
			btnText.getLocalBounds().left + btnText.getLocalBounds().width / 2.0f,
			btnText.getLocalBounds().top + btnText.getLocalBounds().height / 2.0f
		);
		btnText.setPosition(sf::Vector2f(btnRect.getLocalBounds().width / 2.0f, btnRect.getLocalBounds().height / 2.0f));
	}


	void draw(sf::RenderWindow* w) {

		w->draw(btnRect);
		w->draw(btnText);
		for (auto b : children)
			b.draw(w);
	}

	void focus() {
		focused = true;
		btnText.setStyle(sf::Text::Bold | sf::Text::Underlined);
	}

	void unfocus() {
		focused = false;
		btnText.setStyle(sf::Text::Bold);
	}

	GameState getTag() { return tag; }
};






class GUI{

	Button ROOT;
	sf::Font f;
	sf::RenderWindow* renderWindow;
	GameState activeTag = GameState::MAIN_MENU;

public:
	GUI(sf::RenderWindow* w);
	~GUI();

	void init();
	bool react(sf::RenderWindow& window, sf::Event& e, GameState& gameState);
	void draw();
	void onClick(GameState& gs);
};


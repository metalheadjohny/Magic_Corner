#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class ListItem {

public:

	sf::Text ID, ip, port, name, createdAt;
	sf::RectangleShape listItemBlock;
	std::vector<sf::Text> hgAttribs;
	unsigned int CHH = 24u, STRIDE, X_OFFSET = 64u, Y_OFFSET = 64u;
	bool focused = false;

	ListItem(sf::Font& listFont) {

		ID.setCharacterSize(CHH);
		ID.setFillColor(sf::Color(160, 0, 255, 255));
		ID.setStyle(sf::Text::Italic);
		ID.setFont(listFont);
		ID.setPosition(0, 0);

		ip.setCharacterSize(CHH);
		ip.setFillColor(sf::Color(160, 0, 255, 255));
		ip.setStyle(sf::Text::Italic);
		ip.setFont(listFont);
		ip.setPosition(0, 1 * CHH);

		port.setCharacterSize(CHH);
		port.setFillColor(sf::Color(160, 0, 255, 255));
		port.setStyle(sf::Text::Italic);
		port.setFont(listFont);
		port.setPosition(0, 2 * CHH);

		name.setCharacterSize(CHH);
		name.setFillColor(sf::Color(160, 0, 255, 255));
		name.setStyle(sf::Text::Italic);
		name.setFont(listFont);
		name.setPosition(0, 3 * CHH);

		createdAt.setCharacterSize(CHH);
		createdAt.setFillColor(sf::Color(160, 0, 255, 255));
		createdAt.setStyle(sf::Text::Italic);
		createdAt.setFont(listFont);
		createdAt.setPosition(0, 4 * CHH);
	}



	void updateListItem(HostedGame hg, int offset) {

		STRIDE = 6 * CHH;

		ID.setPosition(X_OFFSET, offset * STRIDE);
		ip.setPosition(X_OFFSET, offset * STRIDE + 1 * CHH);
		port.setPosition(X_OFFSET, offset * STRIDE + 2 * CHH);
		name.setPosition(X_OFFSET, offset * STRIDE + 3 * CHH);
		createdAt.setPosition(X_OFFSET, offset * STRIDE + 4 * CHH);

		ID.setString(hg.ID);
		ip.setString(hg.ip);
		port.setString(hg.port);
		name.setString(hg.name);
		createdAt.setString(hg.createdAt);

		listItemBlock.setPosition(X_OFFSET, offset * STRIDE);
		listItemBlock.setSize(sf::Vector2f(200, 5 * CHH));
		listItemBlock.setFillColor(sf::Color::White);
	}



	void drawListItem(sf::RenderWindow& w) {
		w.draw(listItemBlock);
		w.draw(ID);
		w.draw(ip);
		w.draw(port);
		w.draw(name);
		w.draw(createdAt);
	}

};









class JoiningScreen {

	sf::Font f;
	sf::Text t;

	std::vector<ListItem> uiList;
	std::pair<std::string, int> focusedIpPort;
	bool pressed;

	sf::Sprite sprite;

public:

	JoiningScreen() {};
	~JoiningScreen() {};


	void setUp(sf::Sprite& loadingSprite, sf::RenderWindow& w, std::string message) {

		if (!f.loadFromFile("../Assets/Fonts/Frostbite.otf"))
			exit(369);

		t.setFont(f);
		t.setString(message);
		t.setCharacterSize(36);
		t.setFillColor(sf::Color(80, 0, 160, 255));
		t.setStyle(sf::Text::Bold);
		t.setOrigin(t.getLocalBounds().width * 0.5f, t.getLocalBounds().height * 0.5f);
		t.setPosition(w.getSize().x * 0.5f, w.getSize().y * 0.1f);

		sprite = loadingSprite;
		sprite.setOrigin(sprite.getLocalBounds().width * 0.5f, sprite.getLocalBounds().height * 0.5f);
		sprite.setPosition(w.getSize().x * 0.5f, w.getSize().y * 0.5f);

		focusedIpPort = std::make_pair("-1", -1);
	}



	bool update(sf::RenderWindow& w, std::pair<std::string, int>& ipPort) {
		draw(w);
		return processUpdates(w, ipPort);
	}



	void draw(sf::RenderWindow& w) {
		w.draw(sprite);
		w.draw(t);
	}



	bool processUpdates(sf::RenderWindow& w, std::pair<std::string, int>& ipPort) {

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
			case sf::Event::MouseButtonPressed:
				ipPort = onClick();
			}
		}

		highlight(w);

		return goBack;
	}


	void updateList(const std::vector<HostedGame>& games) {

		uiList.resize(games.size(), f);

		for (int i = 0; i < games.size(); i++)
			uiList[i].updateListItem(games[i], i);
	}


	void drawList(sf::RenderWindow& w) {

		for (int i = 0; i < uiList.size(); ++i)
			uiList[i].drawListItem(w);
	}


	std::pair<std::string, int> onClick() {
		return focusedIpPort;
	}


	void highlight(sf::RenderWindow& window) {

		sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition());
		sf::Vector2f mousePosFloat = sf::Vector2f((float)mouse_world.x, (float)mouse_world.y);

		for (ListItem& li : uiList) {
			if (li.listItemBlock.getGlobalBounds().contains(mousePosFloat)) {
				li.ID.setStyle(sf::Text::Style::Bold | sf::Text::Style::Underlined);
				li.ip.setStyle(sf::Text::Style::Bold | sf::Text::Style::Underlined);
				li.port.setStyle(sf::Text::Style::Bold | sf::Text::Style::Underlined);
				li.name.setStyle(sf::Text::Style::Bold | sf::Text::Style::Underlined);
				li.createdAt.setStyle(sf::Text::Style::Bold | sf::Text::Style::Underlined);
				li.focused = true;

				std::string::size_type sz;
				int temp = std::stoi(li.port.getString().toAnsiString());
				
				focusedIpPort = std::make_pair(li.ip.getString(), temp);
			}
			else {
				if (li.focused) {
					li.ID.setStyle(sf::Text::Style::Italic);
					li.ip.setStyle(sf::Text::Style::Italic);
					li.port.setStyle(sf::Text::Style::Italic);
					li.name.setStyle(sf::Text::Style::Italic);
					li.createdAt.setStyle(sf::Text::Style::Italic);
					li.focused == false;
					focusedIpPort = std::make_pair("-1", -1);
				}
			}
		}
	}

};
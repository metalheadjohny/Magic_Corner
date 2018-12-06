#include "GUI.h"

GUI::GUI(sf::RenderWindow* w) {
	
	renderWindow = w;
	ROOT.btnRect.setSize(sf::Vector2f((float)w->getSize().x, (float)w->getSize().y));
	ROOT.parent = nullptr;
	ROOT.btnRect.setFillColor(sf::Color(0, 0, 0, 0));

	mmTex.loadFromFile("../Assets/Main.jpg");
	sprite.setTexture(mmTex);
	sprite.setScale(w->getSize().x / sprite.getLocalBounds().width, w->getSize().y / sprite.getLocalBounds().height);
	sprite.setOrigin(sprite.getLocalBounds().width * 0.5f, sprite.getLocalBounds().height * 0.5f);
	sprite.setPosition(w->getSize().x * 0.5f, w->getSize().y * 0.5f);
}


GUI::~GUI()
{
}


void GUI::init() {

	
	if (!f.loadFromFile("../Assets/Fonts/Frostbite.otf"))
		exit(360);

	sf::Text hostText, joinText, specText, quitText;

	hostText.setFont(f);
	hostText.setString("Host a game (main player)");
	hostText.setCharacterSize(36);
	hostText.setFillColor(sf::Color(180, 0, 30, 255));
	hostText.setStyle(sf::Text::Bold);

	joinText.setFont(f);
	joinText.setString("Join a game (support player)");
	joinText.setCharacterSize(36);
	joinText.setFillColor(sf::Color(180, 0, 30, 255));
	joinText.setStyle(sf::Text::Bold);

	specText.setFont(f);
	specText.setString("Spectate a game");
	specText.setCharacterSize(36);
	specText.setFillColor(sf::Color(180, 0, 30, 255));
	specText.setStyle(sf::Text::Bold);

	quitText.setFont(f);
	quitText.setString("Vacate the premises");
	quitText.setCharacterSize(36);
	quitText.setFillColor(sf::Color(180, 0, 30, 255));
	quitText.setStyle(sf::Text::Bold);

	float posX = renderWindow->getSize().x * 0.5f;
	float posY = renderWindow->getSize().y * 0.5f;

	Button host = Button(hostText, sf::Vector2f(posX, posY -100), &ROOT, GameState::HOSTING);
	Button join = Button(joinText, sf::Vector2f(posX, posY - 50), &ROOT, GameState::JOINING);
	Button spec = Button(specText, sf::Vector2f(posX, posY), &ROOT, GameState::OBSERVING);
	Button quit = Button(quitText, sf::Vector2f(posX, posY + 50), &ROOT, GameState::QUITTING);

	ROOT.children.push_back(host);
	ROOT.children.push_back(join);
	ROOT.children.push_back(spec);
}


bool GUI::react(sf::RenderWindow& window, sf::Event& e, GameState& gameState) {
	
	sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	for (Button& b : ROOT.children) {
		
		if (b.btnRect.getGlobalBounds().contains(mouse_world))
		{
			if (!b.focused) {
				b.focus();
				activeTag = b.getTag();
			}
		}
		else 
		{
			if (b.focused) {
				b.unfocus();
				activeTag = GameState::MAIN_MENU;
			}
			
		}
	}


	while (window.pollEvent(e)) {
		switch (e.type) {
		
		case sf::Event::Closed:
			return false;
			break;
		
		case sf::Event::MouseButtonPressed:
			gameState = activeTag;
			onClick(gameState);
			break;
		case sf::Event::KeyPressed:
			if (e.key.code == sf::Keyboard::Escape)
				return false;
			break;
		}
	}

	draw();
	return true;
}


void GUI::draw() {
	renderWindow->draw(sprite);
	ROOT.draw(renderWindow);
}


void GUI::onClick(GameState& gs) {

	if (activeTag == GameState::QUITTING)
		exit(9999);

	if (activeTag == GameState::NONE)
		return;

	gs = activeTag;
}
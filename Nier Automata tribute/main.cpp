#include "Loader.h"
#include "GameObject.h"
#include <string>

const std::string BASEPATH = "C:\\\\Users\\metal\\\Desktop\\Network\\Assets\\";
float ww = 1920.f, wh = 1080.f;
Loader loader;
InputManager iMan;


int main() {

	sf::RenderWindow window(sf::VideoMode(ww, wh), "Nier but bad", sf::Style::Close | sf::Style::Titlebar | sf::Style::Fullscreen);
	window.setKeyRepeatEnabled(false);

	//Loader::LoadLevel();
	Player p2b(100, 100);
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\limit_break.png", sf::Vector2u(3, 17), 5.f, "lb"); 
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\move_left.png", sf::Vector2u(1, 1), 10.f, "walk_left");
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\move_right.png", sf::Vector2u(1, 1), 10.f, "walk_right");
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\sorted\\m2b_pat_bot.png", sf::Vector2u(20, 1), 2.f, "idle");

	InanimateObject floor;
	floor.rs = sf::RectangleShape(sf::Vector2f(ww, 50.0f));
	floor.rs.setPosition(sf::Vector2f(0.f, wh / 2.0f));
	floor.rs.setFillColor(sf::Color::Red);


	iMan.attachObserver(p2b);

	sf::Clock deltaClock;
	sf::Event e;

	while (window.isOpen()) {

		sf::Time dt = deltaClock.restart();

		iMan.processInput(window, e);

		p2b.Update(dt.asSeconds());

		window.clear();
		window.draw(p2b.sprite);
		window.draw(floor.rs);
		window.display();
	}

	return 0;
}
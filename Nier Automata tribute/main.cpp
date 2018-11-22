#include "Loader.h"
#include "GameObject.h"
#include "TCPInterface.h"
#include <string>

const std::string BASEPATH = "C:\\\\Users\\metal\\\Desktop\\Network\\Assets\\";
static const float UPDATE_INTERVAL = 0.33f;
float ww = 1920.f, wh = 1080.f, elapsed = 0.f, late = 0.f, frameTime = 0.0f;
Loader loader;
InputManager iMan;
TCPInterface tcpi;

InanimateObject platform;
Player p2b(100, 100);

std::vector<InanimateObject*> terrain;

void resolveCollisions();

int main() {

	sf::RenderWindow window(sf::VideoMode(ww, wh), "Nier but bad", sf::Style::Close | sf::Style::Titlebar | sf::Style::Fullscreen);
	window.setKeyRepeatEnabled(false);

	//Loader::LoadLevel();
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\limit_break.png", sf::Vector2u(3, 17), 5.f, "lb"); 
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\move_left.png", sf::Vector2u(1, 1), 10.f, "walk_left");
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\move_right.png", sf::Vector2u(1, 1), 10.f, "walk_right");
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\sorted\\m2b_pat_bot.png", sf::Vector2u(20, 1), 2.f, "idle");

	
	platform.rs = sf::RectangleShape(sf::Vector2f(ww, 50.0f));
	platform.rs.setPosition(sf::Vector2f(0.f, wh / 2.0f));
	platform.rs.setFillColor(sf::Color::Red);

	terrain.push_back(&platform);

	iMan.attachObserver(p2b);

	sf::Clock deltaClock;
	sf::Event e;

	tcpi.init("127.0.0.1", 69);
	//tcpi.getRefToClientSocket().setBlocking(false);
	tcpi.connect();

	

	sf::Packet inputPacket;
	sf::Packet updatePacket;

	Pos_9S pos9s;
	pos9s.angle = 30.0f;
	pos9s.distance = 3.5f;

	Pos_2B pos2b;
	
	

	while (window.isOpen()) {

		sf::Time dt = deltaClock.restart();
		frameTime = dt.asSeconds();
		elapsed += frameTime;
		late += frameTime;

		iMan.processInput(window, e);

		//updating locally
		p2b.Update(frameTime);


		//sending updates
		if (late >= UPDATE_INTERVAL) {

			//load updates
			p2b.stageUpdates(pos2b);	//this is ugly af, make a staging class that does all this
			pos2b.Load(updatePacket);

			//send data
			tcpi.send(updatePacket);
			updatePacket.clear();

			//reset the interval
			late -= UPDATE_INTERVAL;
		}

		resolveCollisions();

		window.clear();
		window.draw(p2b.sprite);
		window.draw(platform.rs);
		window.display();
	}

	return 0;
}


void resolveCollisions() {

	for (auto a : terrain) {
		if (p2b.sprite.getGlobalBounds().intersects(a->rs.getGlobalBounds())) {
			p2b.posMin.y -= gravity * 10.0f * frameTime;
			//p2b.posMin.y = a->rs.getGlobalBounds().top - p2b.sprite.getLocalBounds().height * 0.75f;
		}
	}
	
	
}
#include "Loader.h"
#include "GameObject.h"
#include "Relay.h"
#include "TileMapper.h"
#include <string>
#include "FSM.h"



const std::string BASEPATH = "../Assets/";
static const float UPDATE_INTERVAL = 0.33f;
float ww = 1920.f, wh = 1080.f, elapsed = 0.f, late = 0.f, frameTime = 0.0f;
Loader loader;
InputManager iMan;
TCPInterface tcpi;

sf::RenderWindow window(sf::VideoMode(ww, wh), "Nier but bad", sf::Style::Close | sf::Style::Titlebar | sf::Style::Fullscreen);
sf::View view(sf::Vector2f(0, 0), sf::Vector2f(ww, wh));

InanimateObject platform;
Player p2b(100, 300);

std::vector<InanimateObject*> terrain;

TileMapper tileMapper;

void resolveCollisions();

int main() {

	window.setKeyRepeatEnabled(false);
	window.setView(view);

	tileMapper.loadFromFile(BASEPATH + "LevelTileMap.txt", "refinery.png");

	//Loader::LoadLevel();
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\limit_break.png", sf::Vector2u(3, 17), 5.f, "lb"); 
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\move_left.png", sf::Vector2u(1, 1), 10.f, "walk_left");
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\move_right.png", sf::Vector2u(1, 1), 10.f, "walk_right");
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\magic_attack.png", sf::Vector2u(3, 2), 10.f, "ranged_attack");
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\sorted\\m2b_pat_bot.png", sf::Vector2u(20, 1), 2.f, "idle");
	p2b.posMin = sf::Vector2f(97.f, 97.f);

	
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

		view.setCenter(p2b.posMin);
		window.setView(view);


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
		window.draw(tileMapper.background);
		for (auto a : tileMapper.level) {
			window.draw(a.rs);
		}
		
		p2b.draw(&window);
		//window.draw(platform.rs);
		window.display();
	}

	return 0;
}


void resolveCollisions() {

	for (auto a : tileMapper.level) {
		if (a.collides && p2b.sprite.getGlobalBounds().intersects(a.rs.getGlobalBounds())) {
			//handle collision for top down, im switchin' baby
			p2b.posMin -= p2b.velocity * frameTime * p2b.speed;
		}
	}
	
	
}
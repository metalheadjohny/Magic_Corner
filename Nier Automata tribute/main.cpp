#include "Loader.h"
#include "GameObject.h"
#include "Relay.h"
#include "TileMapper.h"
#include "FSM.h"
#include "GUI.h"
#include "MatchMaker.h"
#include "LoadingScreen.h"
#include <string>


const std::string BASEPATH = "../Assets/";
static const float UPDATE_INTERVAL = 0.33f;
static const float HOST_LIST_UPDATE_INTERVAL = 10.0f;
float ww = 1920.f, wh = 1080.f, elapsed = 0.f, elapsedSinceHostUpdate = 0.f, late = 0.f, frameTime = 0.0f;
bool run = true;

Loader loader;
InputManager iMan;
TCPInterface tcpi;
TileMapper tileMapper;
MatchMaker matchMaker;
GameState gameState = GameState::MAIN_MENU;
LoadingScreen lsHosting, lsJoining;

sf::RenderWindow window(sf::VideoMode(ww, wh), "Nier but bad", sf::Style::Close | sf::Style::Titlebar | sf::Style::Fullscreen);
sf::View view(sf::Vector2f(0, 0), sf::Vector2f(ww, wh));

Player p2b(100, 300);



void resolveCollisions();
std::string random_string(size_t length);

int main() {

	//initiate some game systems
	window.setKeyRepeatEnabled(false);

	GUI gui(&window);
	gui.init();

	matchMaker.setHost("127.0.0.1");

	sf::Clock deltaClock;
	sf::Event e;
	
	//prepare loading screens
	sf::Texture texHosting, texJoining;
	texHosting.loadFromFile(BASEPATH + "loading_2b.jpg");
	texJoining.loadFromFile(BASEPATH + "loading_9s.jpg");

	sf::Sprite picHosting, picJoining;
	picHosting.setTexture(texHosting);
	picJoining.setTexture(texJoining);

	lsHosting.setUp(picHosting, window, "Waiting for 9s...");
	lsJoining.setUp(picJoining, window, "Searching for 2b...");

	//hosting params
	bool hostedAlready = false;

	//jam frame updates in here
	while (window.isOpen()) {

		//frame timing
		sf::Time dt = deltaClock.restart();
		frameTime = dt.asSeconds();
		elapsed += frameTime;
		late += frameTime;
		

		if (gameState == GameState::MAIN_MENU)
		{
			gui.react(window, e, gameState);
			gui.draw();
		}


		if (gameState == GameState::HOSTING) 
		{
			//loading screen while waiting for joins
			if (!hostedAlready) {
				matchMaker.host();
				hostedAlready = true;
				tcpi.listen();
			}
			else {
				//listen for an incoming connection!
				tcpi.accept();
				if (lsHosting.update(window))
					gameState = GameState::MAIN_MENU;
			}

			
			//when someone accepts
			//matchMaker.unhost();
			//hostedAlready = false
			//switch to GameState::PLAYER1
		}


		if (gameState == GameState::JOINING) 
		{
			elapsedSinceHostUpdate += frameTime;
			if (elapsedSinceHostUpdate > HOST_LIST_UPDATE_INTERVAL) {
				elapsedSinceHostUpdate -= HOST_LIST_UPDATE_INTERVAL;

				std::vector<HostedGame> games = matchMaker.listGames();
			}

			if (lsJoining.update(window))
				gameState = GameState::MAIN_MENU;

			//when accepted by the sever
			//switch to GameState::PLAYER2
		}


		if (gameState == GameState::OBSERVING) 
		{

		}


		window.display();
		window.clear();
	}

	window.setView(view);

	tileMapper.loadFromFile(BASEPATH + "LevelTileMap.txt", "refinery.png");


	//Loader::LoadLevel();
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\limit_break.png", sf::Vector2u(3, 17), 5.f, "lb"); 
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\move_left.png", sf::Vector2u(1, 1), 10.f, "walk_left");
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\move_right.png", sf::Vector2u(1, 1), 10.f, "walk_right");
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\magic_attack.png", sf::Vector2u(3, 2), 10.f, "ranged_attack");
	p2b.loadSSA(BASEPATH + "2B exivus sprites\\sorted\\m2b_pat_bot.png", sf::Vector2u(20, 1), 2.f, "idle");
	p2b.posMin = sf::Vector2f(97.f, 97.f);

	iMan.attachObserver(p2b);



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
		gui.draw();
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
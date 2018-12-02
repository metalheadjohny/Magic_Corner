#include "Relay.h"
#include "FSM.h"
#include "GUI.h"
#include "MatchMaker.h"
#include "HostingScreen.h"
#include "JoiningScreen.h"
#include "Game2B.h"
#include "Game9s.h"
#include <string>

#include "InputManager.h"

static const float HOST_LIST_UPDATE_INTERVAL = 3.0f;
float ww = 1920.f * 0.5f, wh = 1080.f * 0.5f, elapsed = 0.f, elapsedSinceHostUpdate = 0.f, late = 0.f, frameTime = 0.0f;
bool run = true;

std::pair<std::string, int> joinIpPort;

MatchMaker matchMaker;
Relay relay;

GameState gameState = GameState::MAIN_MENU;

HostingScreen hScreen;
JoiningScreen jScreen;

InputManager iMan;

sf::RenderWindow window(sf::VideoMode(ww, wh), "Nier but bad", sf::Style::Close | sf::Style::Titlebar);

int main() {

	//initiate some game systems
	window.setKeyRepeatEnabled(false);

	GUI gui(&window);
	Game2B g2b(window);
	Game9S g9s(window);
	gui.init();

	//network stuff
	matchMaker.setHost("127.0.0.1");
	relay.init();
	relay.tcpi.unblock();

	sf::Clock deltaClock;
	sf::Event e;
	
	//prepare loading screens
	sf::Texture texHosting, texJoining;
	texHosting.loadFromFile("../Assets/loading_2b.jpg");
	texJoining.loadFromFile("../Assets/loading_9s.jpg");

	sf::Sprite picHosting, picJoining;
	picHosting.setTexture(texHosting);
	picJoining.setTexture(texJoining);

	hScreen.setUp(picHosting, window, "Waiting for 9s...");
	jScreen.setUp(picJoining, window, "Searching for 2b...");

	//jam frame updates in here
	while (window.isOpen() && run) {

		//frame timing
		sf::Time dt = deltaClock.restart();
		frameTime = dt.asSeconds();
		elapsed += frameTime;
		late += frameTime;
		

		if (gameState == GameState::MAIN_MENU)
		{
			run = gui.react(window, e, gameState);
		}



		if (gameState == GameState::HOSTING) 
		{
			//loading screen while waiting for joins
			if (!matchMaker.hosting) {
				matchMaker.host();
				relay.tcpi.listen();
			}
			else {
				//listen for an incoming connection!
				if (relay.tcpi.accept()) {					
					relay.tcpi.closeListener();
					matchMaker.unhost();
					gameState = GameState::PLAYER_2B;
				}
				if (hScreen.update(window)) {
					gameState = GameState::MAIN_MENU;
					matchMaker.unhost();
				}
			}
		}



		if (gameState == GameState::JOINING) 
		{
			elapsedSinceHostUpdate += frameTime;
			if (elapsedSinceHostUpdate > HOST_LIST_UPDATE_INTERVAL) {
				elapsedSinceHostUpdate -= HOST_LIST_UPDATE_INTERVAL;
				jScreen.updateList(matchMaker.listGames());
			}

			joinIpPort = std::make_pair("-1", -1);
			if (jScreen.update(window, joinIpPort))
				gameState = GameState::MAIN_MENU;

			jScreen.drawList(window);

			//when accepted by the sever
			if (joinIpPort.second != -1) {
				relay.tcpi.block();
				relay.tcpi.connect(joinIpPort.first, joinIpPort.second);
				relay.tcpi.unblock();
				gameState = GameState::PLAYER_9S;
			}
			
		}



		if (gameState == GameState::OBSERVING) 
		{
			//ignore for now... implement if there is enough time...
			//receive updates from player one and render everything
		}



		if (gameState == GameState::PLAYER_2B)
		{
			if (g2b.first) {
				g2b.init();
				relay.attachPlayerObserver(g2b.player);
				g2b.attachRelay(relay);
				g2b.first = false;
			}

			g2b.update(frameTime, window);
		}



		if (gameState == GameState::PLAYER_9S)
		{
			if (g9s.first) {
				g9s.init();
				relay.attachPlayerObserver(g9s.player);
				g9s.attachRelay(relay);
				g9s.first = false;
			}

			g9s.update(frameTime, window);
		}



		window.display();
		window.clear();
	}

	matchMaker.unhost();
	return 0;
}
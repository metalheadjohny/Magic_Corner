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
float ww = 1920.f, wh = 1080.f, elapsed = 0.f, elapsedSinceHostUpdate = 0.f, late = 0.f, frameTime = 0.0f;
bool run = true;
std::pair<std::string, int> joinIpPort;

MatchMaker matchMaker;
Relay relay;

GameState gameState = GameState::MAIN_MENU;

HostingScreen hScreen;
JoiningScreen jScreen;

InputManager iMan;

sf::RenderWindow window(sf::VideoMode(ww, wh), "Nier but bad", sf::Style::Close | sf::Style::Titlebar | sf::Style::Fullscreen);

int main() {

	//initiate some game systems
	window.setKeyRepeatEnabled(false);

	GUI gui(&window);
	Game2B g2b(window);
	Game9S g9s(window);
	gui.init();

	matchMaker.setHost("127.0.0.1");
	relay.establish();

	iMan.attachObserver(g2b.p2b);
	iMan.attachObserver(g9s.p9s);

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
				gameState = GameState::PLAYER_9S;	//switch to GameState::PLAYER2
				relay.tcpi.connect(joinIpPort.first, joinIpPort.second);
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
				g2b.first = false;
			}

			g2b.update(frameTime, window);

			//send position, state of animation and battle commands
			g2b.update9s();
			//receive the updates from the other player
			g2b.receiveUpdates();

			//send updates to observers
			g2b.updateObservers();
		}



		if (gameState == GameState::PLAYER_9S)
		{
			if (g9s.first) {
				g9s.init();
				g9s.first = false;
			}

			g9s.update(frameTime);
			//send degree of rotation, state of animation and support commands
			//receive the updates from the other player
			//render game
		}



		window.display();
		window.clear();
	}

	matchMaker.unhost();
	return 0;
}
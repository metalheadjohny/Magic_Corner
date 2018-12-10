#include "Relay.h"
#include "FSM.h"
#include "GUI.h"
#include "MatchMaker.h"
#include "HostingScreen.h"
#include "JoiningScreen.h"
#include "DefeatScreen.h"
#include "VictoryScreen.h"
#include "SpectateScreen.h"
#include "Game2B.h"
#include "Game9s.h"
#include <string>

#include "InputManager.h"

static const float HOST_LIST_UPDATE_INTERVAL = 10.f;
float ww = 1920.f * 0.5f, wh = 1080.f * 0.5f, elapsed = 0.f, elapsedSinceHostUpdate = 10.f, late = 0.f, frameTime = 0.0f;
bool run = true;
bool g2bfirst = true, g9sfirst = true, canStart9s = false;
std::uint64_t deltaMs = 0;

std::pair<std::string, int> joinIpPort;

MatchMaker matchMaker;
Relay relay;

GameState gameState = GameState::MAIN_MENU;

HostingScreen  hScreen;
JoiningScreen  jScreen;
VictoryScreen  vScreen;
DefeatScreen   dScreen;
SpectateScreen sScreen;

InputManager iMan;

sf::RenderWindow window(sf::VideoMode(ww, wh), "Nier but bad", sf::Style::Close | sf::Style::Titlebar);

int main() {

	//initiate some game systems
	window.setKeyRepeatEnabled(false);

	GUI gui(&window);
	gui.init();

	Game2B* g2bPtr = nullptr;
	Game9S* g9sPtr = nullptr;

	//network stuff
	matchMaker.setHost("127.0.0.1");
	relay.init();
	relay.tcpi.unblock();

	sf::Clock deltaClock;
	sf::Event e;
	
	//prepare loading screens
	sf::Texture texHosting, texJoining, texVictory, texDefeat, texSpectating;
	texHosting.loadFromFile("../Assets/loading_2b.jpg");
	texJoining.loadFromFile("../Assets/loading_9s.jpg");
	texVictory.loadFromFile("../Assets/Victory.jpg");
	texDefeat.loadFromFile("../Assets/Defeat.jpg");
	texSpectating.loadFromFile("../Assets/emil2.jpg");

	sf::Sprite picHosting, picJoining, picVictory, picDefeat, picSpectating;
	picHosting.setTexture(texHosting);
	picJoining.setTexture(texJoining);
	picVictory.setTexture(texVictory);
	picDefeat.setTexture(texDefeat);
	picSpectating.setTexture(texSpectating);

	hScreen.setUp(picHosting, window, "Waiting for 9s...");
	jScreen.setUp(picJoining, window, "Searching for 2b...");
	vScreen.setUp(picVictory, window, "[FOR THE GLORY OF MANKIND]");
	dScreen.setUp(picDefeat,  window, "2B... It was an honor to fight with you. Truly.");
	sScreen.setUp(picSpectating, window);

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
			//start listening for connections and inform the server about it

			if (!matchMaker.hosting) {
				relay.tcpi.listen();
				matchMaker.host();
			}
			else {
				//listen for an incoming connection, trying to accept each frame, once it happens, game is on!
				//also inform the server that hosting is no longer open to new clients
				if (relay.tcpi.accept()) {	
					relay.tcpi.closeListener();
					matchMaker.unhost();
					gameState = GameState::PLAYER_2B;
				}

				//allow the user to stop hosting and inform the server if he does...
				if (hScreen.update(window)) {
					gameState = GameState::MAIN_MENU;
					matchMaker.unhost();
				}
			}
		}



		if (gameState == GameState::JOINING) 
		{
			//each HOST_LIST_UPDATE_INTERVAL, ask the server for a new list of hosts, and update the local list with it
			elapsedSinceHostUpdate += frameTime;
			if (elapsedSinceHostUpdate > HOST_LIST_UPDATE_INTERVAL) {
				elapsedSinceHostUpdate -= HOST_LIST_UPDATE_INTERVAL;
				jScreen.updateList(matchMaker.listGames());
			}

			//listen for a click on the joining window, and update the IP/port pair from the list
			joinIpPort = std::make_pair("-1", -1);

			//however if it returns true it means the user wants to stop looking for a game, by clicking escape or close...
			if (jScreen.update(window, joinIpPort))
				gameState = GameState::MAIN_MENU;

			//draw the list each frame
			jScreen.drawList(window);

			//try to connect to the server, when accept comes through game is on! it's set to blocking as this should be instantaneous...
			//however in a real scenario someone else might connect first before the list updates (within HOST_LIST_UPDATE_INTERVAL)
			//And this could hang until timeout (set to 10 seconds). It should be non blocking in that case but for this scenario this is fine
			if (joinIpPort.second != -1) {
				relay.tcpi.block();
				if (relay.tcpi.connect(joinIpPort.first, joinIpPort.second))
					gameState = GameState::PLAYER_9S;
				relay.tcpi.unblock();
			}
			
		}



		if (gameState == GameState::PLAYER_2B)
		{
			if (g2bfirst) {
				g2bPtr = new Game2B(window);
				g2bPtr->init();
				relay.attachPlayerObserver(g2bPtr->player);
				g2bPtr->attachRelay(relay);
				g2bfirst = false;
				//relay.tcpi.block();
				relay.sendStartingMessage();
				std::cout << "Starting message sent!" << std::endl;
				//relay.tcpi.unblock();
				continue;
			}

			g2bPtr->update(frameTime, window, gameState);
		}



		if (gameState == GameState::PLAYER_9S)
		{
			if (g9sfirst) 
			{
				g9sPtr = new Game9S(window);
				g9sPtr->init();
				relay.attachPlayerObserver(g9sPtr->player);
				g9sPtr->attachRelay(relay);
				g9sfirst = false;
				g9sPtr->first = true;
			}

			if (!canStart9s) {
				canStart9s = relay.receiveStartingMessage(deltaMs);
				if (canStart9s) {
					g9sPtr->initialRobotSync(deltaMs);
					std::cout << "Starting message sent!" << std::endl;
				}
				continue;
			}
			
			
			g9sPtr->update(frameTime, window, gameState);
		}



		if (gameState == GameState::VICTORY)
		{
			if (g2bPtr != nullptr || g9sPtr != nullptr)
			{
				relay.tcpi.disconnect();
				window.setView(window.getDefaultView());

				delete g2bPtr;
				delete g9sPtr;
				g2bPtr = nullptr;
				g9sPtr = nullptr;

				g2bfirst = true;
				g9sfirst = true;
				canStart9s = false;
			}

			if (vScreen.update(window))
				gameState = GameState::MAIN_MENU;
		}



		if (gameState == GameState::DEFEAT)
		{
			if (g2bPtr != nullptr || g9sPtr != nullptr) 
			{
				relay.tcpi.disconnect();
				window.setView(window.getDefaultView());

				delete g2bPtr;
				delete g9sPtr;
				g2bPtr = nullptr;
				g9sPtr = nullptr;

				g2bfirst = true;
				g9sfirst = true;
				canStart9s = false;
			}


			if (dScreen.update(window))
				gameState = GameState::MAIN_MENU;
		}



		if (gameState == GameState::OBSERVING)
		{
			if(sScreen.update(window))
				gameState = GameState::MAIN_MENU;
		}

		window.display();
		window.clear();
	}

	matchMaker.unhost();
	return 0;
}
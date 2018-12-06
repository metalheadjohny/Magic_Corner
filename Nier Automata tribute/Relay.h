#pragma once
#include "TCPInterface.h"
#include <vector>

class Player;

//All networking should be here from the view of main! And only here! Facade pattern!
class Relay{

	//@TODO: make this into an object with timestamps and more than just position...
	std::string ipAddr = "";

	Player* player;

public:

	TCPInterface tcpi;

	std::vector<Msg2B> updates2b;
	std::vector<Msg9S> updates9s;

	//vector for observers as well... tbi

	Relay();
	~Relay();

	void init();
	void establish();
	void relay2B();
	void relay9S();
	void relayVictory();
	void relayDefeat();


	bool checkFor2BUpdates(Msg2B& msg) 
	{
		sf::Packet p;

		if (tcpi.receive9s(p)) {
			msg.decipher(p);
			msg.print();
			return true;
		}
		return false;
	}

	bool checkFor9SUpdates(Msg9S& msg) 
	{
		sf::Packet p;

		if (tcpi.receive2b(p)) {
			msg.decipher(p);
			msg.print();
			return true;
		}
		return false;
	}

	//try to guess the hell is gonna happen in between updates
	void divinate();


	void accumulate2b(Msg2B msg);
	void accumulate9s(Msg9S msg);

	void attachPlayerObserver(Player& pRef) {
		player = &pRef;
	}

	void sendMouseDir(sf::Vector2f mouseDir) {
		
		sf::Packet p;

		sf::Int32 tempType = static_cast<int>(MessageType::T_2B_MD_ONLY);

		p << tempType << mouseDir.x << mouseDir.y;

		tcpi.send2b(p);
	}
};


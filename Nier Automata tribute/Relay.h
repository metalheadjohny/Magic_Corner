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

	//hang until someone connects if server, or until connected if client... decide on arhitecture
	void establish();

	//wrap all the outbound data into a sf::Packet and send it to the client/server... decide on architecture!!!
	void relay();
	
	//receive inbound data and inject it into the main loop, this should act like another processInput, because it is!
	//it should call divinate on previous results if no new data was available, which will let it run somewhat believably regardless of network issues 
	bool checkFor2BUpdates(Msg2B& msg) 
	{
		sf::Packet p;

		if (tcpi.receive(p)) {
			msg.decipher(p);
			msg.print();	//@TODO stop printing later
			return true;
		}
		return false;
	}

	void incorporate9s() {

	}

	//try to guess the hell is gonna happen in between updates
	void divinate();


	void accumulate2b(Msg2B msg);
	void accumulate9s(Msg9S msg);

	void attachPlayerObserver(Player& pRef) {
		player = &pRef;
	}
};


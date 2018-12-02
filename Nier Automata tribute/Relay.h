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
	Msg2B incorporate2b() {

		Msg2B msg;
		sf::Packet p;
		tcpi.receive(p);
		msg.decipher(p);
		msg.print();	//todo stop printing later

		return msg;
	}

	void incorporate9s() {

	}

	//try to guess the hell is gonna happen in between updates
	void divinate();

	//store messages of 2b accumulated each frame... filter them later
	void accumulate(Msg2B msg);

	void attachPlayerObserver(Player& pRef) {
		player = &pRef;
	}
};


#pragma once
#include "TCPInterface.h"
#include <vector>

//All networking should be here! And only here! Facade pattern!
class Relay{

	//@TODO: make this into an object with timestamps and more than just position...
	std::vector<std::pair<float, float>> history;

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
	void incorporate();

	//try to guess the hell is gonna happen in between updates
	void divinate();

	void accumulate();
};


#pragma once
#include "TCPInterface.h"
#include <chrono>
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
	void divinate(); //not used for now


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



	void relayBodyCount(std::vector<int> deltaBodyCount) {

		sf::Int32 bodyCount = deltaBodyCount.size();

		sf::Packet p;

		p << static_cast<sf::Int32>(MessageType::T_2B_DEAD_BOTS);
		
		p << bodyCount;
		
		for (int i = 0; i < bodyCount; i++)
			p << sf::Int32(deltaBodyCount[i]);

		tcpi.send2b(p);
	}



	std::vector<int> receiveBodyCount() {
		std::vector<int> result;
		sf::Packet p;
	}


	///not used, for now...
	void sendStartingMessage() {

		sf::Packet p;

		p << (static_cast<sf::Int32>(MessageType::T_2B_START)) << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		tcpi.send2b(p);
	}

	bool receiveStartingMessage(std::uint64_t& delta) {

		std::uint64_t curMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::uint64_t otherMs;

		sf::Int32 type32;
		MessageType type;

		sf::Packet p;

		if (tcpi.receive9s(p)) {
			p >> type32;
			type = static_cast<MessageType>(type32);

			if (type == MessageType::T_2B_START) {
				p >> otherMs;
				delta = curMs - otherMs;
				return true;
			}
		}
		return false;
	}
};
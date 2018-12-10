#pragma once
#include <SFML/Network.hpp>
#include "InputManager.h"
#include <iostream>



enum MessageType {
	T_2B,
	T_9S,
	T_2B_MD_ONLY,
	T_2B_VICTORY,
	T_2B_START,
	T_2B_DEAD_BOTS,
	T_2B_ACTIVE_BOTS,
	T_2B_DEFEAT,
	DISCONNECT,
	PULSE
};


struct BotUpdateData {
	sf::Vector2f pos;
	sf::Int32 OG_INDEX;

	BotUpdateData(sf::Vector2f position, sf::Int32 originalIndex) {
		pos = position;
		OG_INDEX = originalIndex;
	}
};



struct Msg2B {

	MessageType type = MessageType::T_2B;
	sf::Int32 int32type;
	Event2B state;
	sf::Int32 int32state;
	std::uint64_t ms;
	std::vector<sf::Int32> deadBots;
	std::vector<BotUpdateData> buds;

	float x = 0, y = 0, dirX = 0, dirY = 0;



	//yes I really hate switch syntax
	void decipher(sf::Packet& p) {
		
		p >> int32type;
		
		type = static_cast<MessageType>(int32type);

		if (type == MessageType::T_2B) {
			p >> int32state >> x >> y >> dirX >> dirY >> ms;
			state = static_cast<Event2B>(int32state);
		}

		if (type == MessageType::T_2B_MD_ONLY) {
			p >> dirX >> dirY;
			return;
		}

		if (type == MessageType::T_2B_DEAD_BOTS) {
			int vSize = 0;
			p >> vSize;
			for (int i = 0; i < vSize; i++) {
				sf::Int32 curOGI;
				p >> curOGI;
				deadBots.push_back(curOGI);
			}
			return;
		}

		if (type == MessageType::T_2B_ACTIVE_BOTS) {

			sf::Int32 botCount;

			p >> ms;
			p >> botCount;

			for (int i = 0; i < botCount; i++) 
			{
				sf::Int32 ogIndex;
				float bX, bY;
				p >>ogIndex >> bX >> bY;

				buds.push_back(BotUpdateData(sf::Vector2f(bX, bY), ogIndex));
			}

		}
	}


	//not really used except for testing, so it doesn't have all the message types... also switch sucks
	void print() {

		if (type == MessageType::T_2B_MD_ONLY) 
		{
			std::cout << "Type: T_2B_MD_ONLY" << std::endl;
			std::cout << "Dir:  " << dirX << " " << dirY << std::endl;
			return;
		}

		if(type == MessageType::T_2B_DEAD_BOTS) 
		{
			std::cout << "Dead bots: ";

			for (auto i : deadBots)
				std::cout << i << ", ";

			std::cout << ";" << std::endl << std::endl;

			return;
		}
	}

};



struct Msg9S {

	MessageType type = MessageType::T_9S;
	sf::Int32 int32type;
	Event9S state;
	sf::Int32 int32state;
	std::uint64_t ms;

	float angle, distance;
	sf::Vector2f push, hack;

	void decipher(sf::Packet& p) {

		p >> int32type;
		type = static_cast<MessageType>(int32type);

		if (type == MessageType::T_9S) {
			p >> int32state >> angle >> distance >> push.x >> push.y >> hack.x >> hack.y >> ms;
			state = static_cast<Event9S>(int32state);
			print();
		}

	}

	void print() {
		std::cout << "Type:  " << int32type << std::endl;
		std::cout << "State: " << int32state << std::endl;
		std::cout << "Angle: " << angle << "; Distance: " << distance << std::endl;
		std::cout << "Push:  " << push.x << push.y << std::endl;
		std::cout << "Hack:  " << hack.x << hack.y << std::endl;
		std::cout << "Stamp: " << ms << std::endl << std::endl;
	}
};
#pragma once
#include <SFML/Network.hpp>
#include "InputManager.h"
#include <iostream>



enum MessageType {
	T_2B,
	T_9S,
	T_2B_MD_ONLY,
	T_2B_VICTORY,
	T_2B_DEFEAT
};



struct Msg2B {

	MessageType type = MessageType::T_2B;
	sf::Int32 int32type;
	Event2B state;
	sf::Int32 int32state;
	std::uint64_t ms;

	float x = 0, y = 0, dirX = 0, dirY = 0;

	void decipher(sf::Packet& p) {
		
		p >> int32type;
		
		type = static_cast<MessageType>(int32type);

		if (type == MessageType::T_2B_MD_ONLY) {
			p >> dirX >> dirY;
			return;
		}
		else {
			p >> int32state >> x >> y >> dirX >> dirY >> ms;
			state = static_cast<Event2B>(int32state);
		}
	}

	void print() {

		if (type == MessageType::T_2B_MD_ONLY) {
			std::cout << "Type: T_2B_MD_ONLY" << std::endl;
			std::cout << "Dir:  " << dirX << " " << dirY << std::endl;
		}
		else {
			std::cout << "Type:   T_2B " << std::endl;
			std::cout << "State: " << int32state << std::endl;
			std::cout << "Pos:   " << x << " " << y << std::endl;
			std::cout << "Dir:   " << dirX << " " << dirY << std::endl;
			std::cout << "Stamp: " << ms << std::endl << std::endl;
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

		bool onlyMousePos = false;



		p >> int32type >> int32state >> angle >> distance >> push.x >> push.y >> hack.x >> hack.y >> ms;
		state = static_cast<Event9S>(int32state);
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
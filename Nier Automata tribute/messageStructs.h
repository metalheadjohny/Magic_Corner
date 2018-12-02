#pragma once
#include <SFML/Network.hpp>
#include <iostream>



enum PlayerType {
	T_2B,
	T_9S
};



struct Msg2B {

	float x, y;
	sf::Int32 state;
	std::uint64_t ms;
	PlayerType type = PlayerType::T_2B;
	sf::Int32 int32type;

	void decipher(sf::Packet& p) {
		p >> int32type >> x >> y >> state >> ms;
		type = static_cast<PlayerType>(int32type);
	}

	void print() {
		std::cout << "Type:  " << int32type << std::endl;
		std::cout << "Pos:   " << x << " " << y << std::endl;
		std::cout << "State: " << int32type << std::endl;
		std::cout << "Stamp: " << ms << std::endl << std::endl;
	}

};



struct Msg9S {

	float angle, distance;
	sf::Int32 state;
	std::uint64_t ms;
	PlayerType type = PlayerType::T_9S;
	sf::Int32 int32type;

	void decipher(sf::Packet& p) {
		p >> int32type >> angle >> distance >> state >> ms;
		type = static_cast<PlayerType>(int32type);
	}

	void print() {
		std::cout << "Type:  " << int32type << std::endl;
		std::cout << "Pos:   " << angle << " " << distance << std::endl;
		std::cout << "State: " << int32type << std::endl;
		std::cout << "Stamp: " << ms << std::endl << std::endl;
	}
};
#pragma once
#include <SFML/Network.hpp>
#include <iostream>



enum MsgType {
	T_2B,
	T_9S
};



struct Msg2B {

	float x, y;
	sf::Int32 state;
	std::uint64_t ms;
	MsgType type = MsgType::T_2B;

};



struct Msg9S {

	float angle;
	float distance;
	sf::Int32 state;
	MsgType type = MsgType::T_9S;
};
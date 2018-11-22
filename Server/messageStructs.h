#pragma once

#include <SFML/Network.hpp>
#include <iostream>

struct Pos_9S {
	float angle, distance;
};

struct Vel_2B {
	float x, y;
};


enum MsgType {
	T_2B,
	T_9S
};

enum CMD_9S {
	ROTATE,
	HACK,
	SHIELD,
	PHASE
};


class Msg {

protected:
	virtual void Load(sf::Packet& p) = 0;
	virtual void Decipher(sf::Packet& p) = 0;
	virtual void Print() = 0;
};




class Pos_2B : Msg {

public:

	float x, y;
	MsgType type = MsgType::T_2B;
	int typeInt = (int)MsgType::T_2B;

	void Load(sf::Packet& p) {
		p << x << y << type;
	}

	void Decipher(sf::Packet& p) {
		p >> x >> y >> typeInt;
	}

	void Print() {
		std::cout << "2B coordinates (x, y) = (" << x << ", " << y << ")" << std::endl;
	}
};


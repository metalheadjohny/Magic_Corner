#include "Relay.h"
#include "GameObject.h"

Relay::Relay()
{
}


Relay::~Relay()
{
}


void Relay::init() {
	ipAddr = sf::IpAddress::getLocalAddress().toString();
	tcpi.init(ipAddr, 36963);
}


void Relay::establish()
{
}


void Relay::relay()
{
	sf::Packet p;

	for (auto msg : updates2b)
		p << msg.type << msg.x << msg.y << msg.state << msg.ms;

	updates2b.clear();
	tcpi.send(p);
}


//void Relay::incorporate9s(){}


void Relay::divinate()
{
}


void Relay::accumulate(Msg2B msg)
{
	updates2b.push_back(msg);
}
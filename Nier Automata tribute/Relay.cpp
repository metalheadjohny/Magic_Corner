#include "Relay.h"
#include "GameObject.h"
#include "Roboto.h"
#include "messageStructs.h"

Relay::Relay()
{
}


Relay::~Relay()
{
}


void Relay::init() {
	ipAddr = sf::IpAddress::getLocalAddress().toString();
	tcpi.init(ipAddr, 36963, *this);
}


void Relay::establish()
{
}


void Relay::relay2B()
{
	sf::Packet p;

	for (auto msg : updates2b)
		p << msg.type << msg.state << msg.x << msg.y << msg.dirX << msg.dirY << msg.ms;

	tcpi.send2b(p);
	updates2b.clear();
}



void Relay::relay9S() 
{	
	sf::Packet p;

	for (auto msg : updates9s)
		p << msg.type << msg.state << msg.angle << msg.distance << msg.push.x << msg.push.y << msg.hack.x << msg.hack.y << msg.ms;

	tcpi.send9s(p);

	updates9s.clear();
}



void Relay::accumulate2b(Msg2B msg)
{
	updates2b.push_back(msg);
}



void Relay::accumulate9s(Msg9S msg)
{
	updates9s.push_back(msg);
}



void Relay::relayVictory()
{
	sf::Packet p;

	p << MessageType::T_2B_VICTORY;

	//just in case...
	for (int i = 0; i < 10; i++) {
		tcpi.send2b(p);
		std::cout << "VICTORY NOTIFICATION SENT!" << std::endl;
	}
}



void Relay::relayDefeat()
{
	sf::Packet p;

	p << MessageType::T_2B_DEFEAT;

	for (int i = 0; i < 10; i++) {
		tcpi.send2b(p);
		std::cout << "DEFEAT NOTIFICATION SENT!" << std::endl;
	}
}



void Relay::divinate() {

}



void Relay::updateLiveBots(const std::vector<Roboto>& bots, std::uint64_t now) {

	sf::Int32 botCount = bots.size();
	sf::Packet p;
	p << static_cast<sf::Int32>(MessageType::T_2B_ACTIVE_BOTS);
	p << now;
	p << botCount;

	for (auto r : bots) {
		BotUpdateData bud = r.getCurrentData();
		p << bud.OG_INDEX << bud.pos.x << bud.pos.y;
	}

	tcpi.send2b(p);
}
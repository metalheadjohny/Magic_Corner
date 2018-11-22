#pragma once

#include "messageStructs.h"
#include <string>
#include <vector>

class TCPInterface {

public:
	sf::Socket::Status status;
	sf::TcpListener listener;

	//std::string received;

	std::vector<sf::TcpSocket*> observers, clients;

	std::string SERVER_IP;
	int SERVER_PORT;

	TCPInterface();
	~TCPInterface();

	void init(std::string serverIp, int serverPort);
	void connect();
	void listen();
	void accept();
	void send(sf::Packet p);
	void receive(sf::Packet& p);
};


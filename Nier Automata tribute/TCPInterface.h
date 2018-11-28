#pragma once

#include "messageStructs.h"
#include <string>
#include <vector>

class TCPInterface {

	sf::TcpSocket socket;
	sf::TcpSocket cooperator;
	sf::Socket::Status status;
	sf::TcpListener listener;

	std::vector<sf::TcpSocket> observers;

	std::string SERVER_IP;
	int SERVER_PORT;


public:
	TCPInterface();
	~TCPInterface();

	void init(std::string serverIp, int serverPort);
	void connect();
	void listen();
	void accept();
	void send(sf::Packet p);
	void receive(sf::Packet& p);
	
	sf::TcpSocket& getRefToClientSocket() { return socket; }
};


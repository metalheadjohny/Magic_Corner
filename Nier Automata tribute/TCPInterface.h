#pragma once

#include "messageStructs.h"
#include <string>
#include <vector>

class TCPInterface {

	sf::TcpListener listener;
	sf::TcpSocket socket;
	sf::TcpSocket cooperator;
	sf::Socket::Status status;

	std::vector<sf::TcpSocket> observers;

	std::string SERVER_IP;
	int SERVER_PORT;


public:
	TCPInterface();
	~TCPInterface();

	void init(std::string serverIp, int serverPort);
	void listen();
	bool accept();
	void closeListener();
	void connect(std::string ip, int port);
	void send(sf::Packet p);
	void send9s(sf::Packet p);
	void receive(sf::Packet& p);
	
	sf::TcpSocket& getRefToClientSocket() { return socket; }
};


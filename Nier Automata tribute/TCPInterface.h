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
	bool send2b(sf::Packet& p);
	bool send9s(sf::Packet& p);
	bool receive2b(sf::Packet& p);
	bool receive9s(sf::Packet& p);
	void disconnect();


	void block() {
		listener.setBlocking(true);
		cooperator.setBlocking(true);
		socket.setBlocking(true);
	}



	void unblock() {
		listener.setBlocking(false);
		cooperator.setBlocking(false);
		socket.setBlocking(false);
	}
	


	sf::TcpSocket& getRefToClientSocket() { 
		return socket; 
	}
};


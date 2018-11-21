#pragma once

#include <SFML/Network.hpp>
#include <string>

class TCPInterface {

	sf::TcpSocket socket;
	sf::TcpSocket cooperator;
	sf::Socket::Status status;

	sf::TcpListener listener;

	const std::string SERVER_IP = "192.168.0.1";
	const int SERVER_PORT = 0;


public:
	TCPInterface();
	~TCPInterface();

	void init();
	void listen();
	void 

	
};


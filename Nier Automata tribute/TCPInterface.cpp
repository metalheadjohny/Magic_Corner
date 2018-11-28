#include "TCPInterface.h"
#include <iostream>



TCPInterface::TCPInterface(){

	listener.setBlocking(false);
	cooperator.setBlocking(false);
}



TCPInterface::~TCPInterface() {
}



void TCPInterface::init(std::string serverIp, int serverPort) {

	SERVER_IP = serverIp;
	SERVER_PORT = serverPort;	
}



void TCPInterface::connect() {

	status = socket.connect(SERVER_IP, SERVER_PORT);

	while (status != sf::Socket::Done) {

		status = socket.connect(SERVER_IP, SERVER_PORT);
		std::cout << "TFW socket is not done yet. Trying again..." << std::endl;
	}
	std::cout << "It's hammertime!" << std::endl;
}



void TCPInterface::listen() {
	while (listener.listen(SERVER_PORT) != sf::Socket::Done)
		std::cout << "Not listening. Not listening!" << std::endl;
}

void TCPInterface::accept() {
	if (listener.accept(cooperator) != sf::Socket::Done)
		std::cout << "Failed to accept a new client..." << std::endl;
	else
		std::cout << "Cooperator found!" << std::endl;
}



void TCPInterface::send(sf::Packet p) {
	
	int tries = 1;
	while (socket.send(p) != sf::Socket::Done) {
		
		if (++tries == 10) {
			std::cout << "Giving up!" << std::endl;
			break;
		}
	}
}



void TCPInterface::receive(sf::Packet& p) {

	status = socket.receive(p);

	if (status != sf::Socket::Status::NotReady)
		std::cout << p << std::endl;
}

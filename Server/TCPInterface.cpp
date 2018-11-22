#include "TCPInterface.h"
#include <iostream>



TCPInterface::TCPInterface(){

	listener.setBlocking(false);
}



TCPInterface::~TCPInterface() {
}



void TCPInterface::init(std::string serverIp, int serverPort) {

	SERVER_IP = serverIp;
	SERVER_PORT = serverPort;	
}



void TCPInterface::listen() {
	
	while (listener.listen(SERVER_PORT) != sf::Socket::Done)
		std::cout << "Not listening. Not listening!" << std::endl;
}

void TCPInterface::accept() {

	sf::TcpSocket* newClient = new sf::TcpSocket;

	if (listener.accept(*newClient) != sf::Socket::Done)
		//std::cout << "Failed to accept a new client..." << std::endl;
		delete newClient;
	else {
		std::cout << "Client found!" << std::endl;
		newClient->setBlocking(false);
		clients.push_back(newClient);
	}

}



void TCPInterface::send(sf::Packet p) {
	
	for (int i = 0; i < clients.size(); i++){
		bool sent = clients[i]->send(p) == sf::Socket::Status::Done;
	}
}



void TCPInterface::receive(sf::Packet& p) {

	for (int i = 0; i < clients.size(); i++) {
		status = clients[i]->receive(p);	
	}
}

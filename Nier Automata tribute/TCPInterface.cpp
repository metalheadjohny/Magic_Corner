#include "TCPInterface.h"
#include <iostream>



TCPInterface::TCPInterface(){

	listener.setBlocking(false);
	cooperator.setBlocking(false);
}



TCPInterface::~TCPInterface() {
}



void TCPInterface::init(std::string serverIp, int serverPort) 
{
	SERVER_IP = serverIp;
	SERVER_PORT = serverPort;	
}



void TCPInterface::listen() 
{
	listener.listen(SERVER_PORT);
	std::cout << "Waiting for 9S to connect..." << std::endl;
}



void TCPInterface::closeListener() 
{
	listener.close();
}



bool TCPInterface::accept() 
{
	if (listener.accept(cooperator) != sf::Socket::Done)
		return false;

	std::cout << "Cooperator found!" << std::endl;
	return true;
}



void TCPInterface::connect(std::string ip, int port) 
{
	status = socket.connect(ip, port);

	while (status != sf::Socket::Done) {

		status = socket.connect(SERVER_IP, SERVER_PORT);
		std::cout << "TFW socket is not done yet. Trying again..." << std::endl;
	}
	std::cout << "It's hammertime!" << std::endl;
}



void TCPInterface::send(sf::Packet p) 
{
	int tries = 1;
	while (socket.send(p) != sf::Socket::Done) {
		
		if (++tries == 10) {
			std::cout << "Giving up!" << std::endl;
			break;
		}
	}
}



void TCPInterface::send9s(sf::Packet p) 
{
	int tries = 1;
	while (cooperator.send(p) != sf::Socket::Done) {

		if (++tries == 10) {
			std::cout << "Giving up!" << std::endl;
			break;
		}
	}
}



void TCPInterface::receive(sf::Packet& p) 
{
	status = socket.receive(p);

	if (status != sf::Socket::Status::NotReady)
		std::cout << p << std::endl;
}

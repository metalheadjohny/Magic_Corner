#include "TCPInterface.h"
#include <iostream>



TCPInterface::TCPInterface(){
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




/*always make sure to use the send(const void* data, std::size_t size, std::size_t& sent) overload
 which returns the number of bytes actually sent in the sent reference parameter after the function returns.*/

bool TCPInterface::send2b(sf::Packet& p)
{
	int tries = 1;

	status = cooperator.send(p);
	while (status != sf::Socket::Done || status == sf::Socket::Status::Partial) {

		cooperator.send(p);

		if (++tries == 10) {
			std::cout << "Giving up on 2b send!" << std::endl;
			return false;
		}
	}
	
	if (status == sf::Socket::Done)
		return true;
	else
		return false;
}



bool TCPInterface::send9s(sf::Packet& p) 
{
	int tries = 1;
	status = cooperator.send(p);
	while (status != sf::Socket::Done || status == sf::Socket::Status::Partial) {

		cooperator.send(p);

		if (++tries == 10) {
			std::cout << "Giving up on 9s send!" << std::endl;
			return false;
		}
	}

	if (status == sf::Socket::Done)
		return true;
	else
		return false;
}



bool TCPInterface::receive(sf::Packet& p) 
{
	status = socket.receive(p);
	if (status != sf::Socket::Status::NotReady)
		return true;

	return false;
}

#include "TCPInterface.h"
#include <iostream>
#include "Relay.h"



TCPInterface::TCPInterface(){
}



TCPInterface::~TCPInterface() {
}



void TCPInterface::init(std::string& serverIp, int serverPort, Relay& rel) 
{
	SERVER_IP = serverIp;
	SERVER_PORT = serverPort;	
	relay = &rel;
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



bool TCPInterface::connect(std::string ip, int port) 
{
	sf::Time timeout = sf::seconds(10);
	status = socket.connect(ip, port, timeout);

	//while (status != sf::Socket::Done) {}
	//status = socket.connect(SERVER_IP, SERVER_PORT);
	//std::cout << "TFW socket is not done connecting yet. Trying again..." << std::endl;
	
	if (status == sf::Socket::Status::Done) {
		std::cout << "It's hammertime!" << std::endl;
		return true;
	}
	return false;
}




/*always make sure to use the send(const void* data, std::size_t size, std::size_t& sent) overload
 which returns the number of bytes actually sent in the sent reference parameter after the function returns.*/

bool TCPInterface::send2b(sf::Packet& p)
{
	int tries = 1;
	status = cooperator.send(p);
	while (status != sf::Socket::Done || status == sf::Socket::Status::Partial) 
	{
		cooperator.send(p);

		if (++tries == 10) 
		{
			std::cout << "Giving up on 2b send!" << std::endl;
			return false;
		}
	}

	if (status == sf::Socket::Disconnected)
		relay->notifyDisconnect();
	else
		relay->allIsWell();
	
	return status == sf::Socket::Done ? true : false;
}



bool TCPInterface::send9s(sf::Packet& p) 
{
	int tries = 1;
	status = socket.send(p);
	while (status != sf::Socket::Done || status == sf::Socket::Status::Partial) {

		socket.send(p);

		if (++tries == 10) {
			std::cout << "Giving up on 9s send!" << std::endl;
			return false;
		}
	}

	if (status == sf::Socket::Disconnected)
		relay->notifyDisconnect();

	return status == sf::Socket::Done ? true : false;
}



bool TCPInterface::receive2b(sf::Packet& p) 
{
	status = cooperator.receive(p);
	if (status == sf::Socket::Status::NotReady || status == sf::Socket::Status::Error)
		return false;

	if (status == sf::Socket::Disconnected) {
		relay->notifyDisconnect();
		return false;
	}
		

	return true;
}



bool TCPInterface::receive9s(sf::Packet& p) 
{
	status = socket.receive(p);
	if (status == sf::Socket::Status::NotReady || status == sf::Socket::Status::Error || status == sf::Socket::Status::Disconnected)
		return false;

	if (status == sf::Socket::Disconnected) {
		relay->notifyDisconnect();
		return false;
	}

	return true;
}


void TCPInterface::disconnect() {
	socket.disconnect();
	cooperator.disconnect();
	relay->notifyDisconnect();
}
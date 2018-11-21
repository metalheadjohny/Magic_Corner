#include "TCPInterface.h"
#include <iostream>


TCPInterface::TCPInterface(){
}


TCPInterface::~TCPInterface() {
}

void TCPInterface::init() {

	status = socket.connect(SERVER_IP, SERVER_PORT);

	while(status != sf::Socket::Done){

		status = socket.connect(SERVER_IP, SERVER_PORT);
		std::cout << "TFW socket is not done yet. Trying again..." << std::endl;
	}
	
}



void TCPInterface::listen() {
	
	while (listener.listen(SERVER_PORT) != sf::Socket::Done)
		std::cout << "Not listening. Not listening!" << std::endl;


	if (listener.accept(cooperator) != sf::Socket::Done)
		std::cout << "Failed to accept a new client..." << std::endl;
	else
		std::cout << "Cooperator found!" << std::endl;

}

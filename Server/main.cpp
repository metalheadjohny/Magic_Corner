#include "TCPInterface.h"
#include <string>

static const float UPDATE_INTERVAL = 0.33f;
float elapsed = 0.f, late = 0.f;
TCPInterface tcpi;

int main() {

	sf::Clock deltaClock;

	tcpi.init("127.0.0.1", 69);
	tcpi.listen();

	//tcpi.getRefToClientSocket().setBlocking(false);

	sf::Packet inputPacket, updatePacket;
	updatePacket << "Server rekt";

	Pos_2B pos2b;

	while (true) {

		tcpi.accept();

		sf::Time dt = deltaClock.restart();
		float frameTime = dt.asSeconds();
		elapsed += frameTime;
		late += frameTime;

		if (late >= UPDATE_INTERVAL) {
			tcpi.send(updatePacket);
			late -= UPDATE_INTERVAL;
		}

		tcpi.receive(inputPacket);
		if (inputPacket.getDataSize() > 1) {
			pos2b.Decipher(inputPacket);
			pos2b.Print();
			inputPacket.clear();
		}
		
	}

	return 0;
}
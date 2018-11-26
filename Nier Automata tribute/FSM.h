#pragma once

enum State {
	MAIN_MENU,
	HOSTING,
	JOINING,
	OBSERVING
};


class FSM{

public:

	FSM();
	~FSM();

	virtual void processInput() = 0;


};


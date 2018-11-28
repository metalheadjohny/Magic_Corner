#pragma once

enum GameState {
	MAIN_MENU,
	HOSTING,
	JOINING,
	OBSERVING,
	QUITTING,
	NONE
};


class FSM{

public:

	FSM();
	~FSM();

	virtual void processInput() = 0;
};


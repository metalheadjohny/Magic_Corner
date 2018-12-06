#pragma once

enum GameState {
	MAIN_MENU,
	HOSTING,
	JOINING,
	OBSERVING,
	QUITTING,
	PLAYER_2B,
	PLAYER_9S,
	DEFEAT,
	VICTORY,
	NONE
};


class FSM{

public:

	FSM();
	~FSM();

	virtual void processInput() = 0;
};


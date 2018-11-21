#pragma once

struct Position {
	float x, y;
};

struct Velocity {
	float x, y;
};

class Msg {

protected:
	virtual void Load(void* p, size_t length);
	virtual void Send();
	virtual void Decipher();
};

class PosMsg : Msg {

	void Load(void* p, size_t length) {
		
	}
};


#pragma once

#include <raylib.h>

class Player {
private:

public:
	Vector3 position;
	float speed;
	const int width = 32, height = 32;

	Player(){}
	virtual ~Player(){}

	void render();
	void update();

};

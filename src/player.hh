#pragma once

#include <raylib.h>

class Player {
private:
	const float speed_min = 0.3;
	const float speed_max = 5.0;

public:
	Vector3 position;
	Vector2 visual_position; // Position where the sprite is rendered
	float speed;
	const int width = 32, height = 32;

	Player(){}
	virtual ~Player(){}

	void render();
	void update();

};

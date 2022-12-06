#pragma once

#include <raylib.h>

class Player {
private:
	const float speed_min = 1.0;
	const float speed_max = 8.0;

	void steer();

public:
	Vector3 position;
	Vector2 visual_position; // Position where the sprite is rendered
	float speed;
	const int width = 32, height = 32;
	const float steer_speed = 2.0;

	Player(){}
	virtual ~Player(){}

	void render();
	void update();

};

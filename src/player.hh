#pragma once

#include <raylib.h>

class Player {
private:
	constexpr static float speed_min = 1.5;
	constexpr static float speed_max = 10.0;

	void steer();

public:
	Vector3 position;
	Vector2 visual_position; // Position where the sprite is rendered
	float speed;
	bool dead;
	constexpr static int width = 32, height = 32;
	constexpr static float steer_speed = 3.0;

	Player();
	virtual ~Player(){}

	void render();
	void update();

};

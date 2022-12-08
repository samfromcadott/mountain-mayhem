#pragma once

class Thing {
private:

public:
	Vector3 position, velocity;
	Vector2 visual_position; // Position where the sprite is rendered
	int width, height;

	Thing(Vector3 position, Vector3 velocity, int width, int height);
	Thing(){}
	virtual ~Thing(){}

	void render();
	void update();
	bool collide();

};

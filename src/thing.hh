#pragma once

class Thing {
private:

public:
	Vector3 position, velocity;
	Vector2 visual_position; // Position where the sprite is rendered
	float speed;
	int width, height;

	Thing();
	virtual ~Thing(){}

	void render();
	void update();
	bool collide();

};

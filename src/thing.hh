#pragma once

class Thing {
private:

public:
	Texture2D* sprite;
	Vector3 position, velocity;
	Vector2 visual_position; // Position where the sprite is rendered
	int width, height;

	Thing(Texture2D* sprite, Vector3 position, Vector3 velocity, int width, int height);
	Thing(){}
	virtual ~Thing(){}

	void render();
	void update();
	bool collide();

};

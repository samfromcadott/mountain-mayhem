#include <raylib.h>
#include <raymath.h>

#include "player.hh"

void Player::render() {
	DrawRectangle(position.x - width/2, position.z - height, width, height, RED);

}

void Player::update() {
	Vector3 direction = {1.0, 0.0, 0.0};
	float acceleration = 0.1;

	Vector3 velocity = Vector3Scale(direction, speed);
	position = Vector3Add(position, velocity);

	speed += acceleration; // Modify speed

}

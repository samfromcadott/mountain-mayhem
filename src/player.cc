#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "globals.hh"
#include "hill.hh"
#include "player.hh"

void Player::render() {
	DrawRectangle(visual_position.x - width/2, visual_position.y - height, width, height, RED);

}

void Player::update() {
	int index = int(position.x / hill.segment_length); // Index of current segment
	Slope segment = hill.segments[index];
	float hill_height = hill.get_height(position.x);

	Vector3 direction;
	float acceleration;

	if ( segment == Slope::DOWN ){
		direction = Vector3Normalize( {1.0, 0.0, 1.0} );
		acceleration = 0.2;
	}
	else if ( segment == Slope::FLAT ) {
		direction = {1.0, 0.0, 0.0};
		acceleration = -0.1;
	}

	Vector3 velocity = Vector3Scale(direction, speed);
	position = Vector3Add(position, velocity);
	// visual_position = Vector2Add(visual_position, {velocity.x, velocity.z});
	visual_position = {position.x, hill_height+position.y+192};

	speed += acceleration; // Modify speed

	// Clamp speed
	if ( speed < speed_min ) speed = speed_min;
	if ( speed > speed_max ) speed = speed_max;

}

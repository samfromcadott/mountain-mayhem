#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <iostream>

#include "globals.hh"
#include "hill.hh"
#include "player.hh"

Player::Player() {
	position = {128, 64, 0};
	speed = 2.0;
	dead = false;

}

void Player::render() {
	if (dead) {
		Color explosion_color = BLACK;
		const float rate = 8; // Rate explosion color changes

		if ( int( GetTime() * rate ) % 2 == 0 ) explosion_color = WHITE;

		DrawCircle(visual_position.x - width/2, visual_position.y - height/2, width/2, explosion_color);

		return;

	}

	float offset = 0.0;
	int index = int(position.x / hill.segment_length); // Index of current segment
	if ( hill.segments[index] == Slope::DOWN ) offset = 32; // When doing down hill use other sprite

	Vector2 sprite_position = {
		std::floor(visual_position.x - width/2), // floor prevent jittering
		std::floor(visual_position.y - height)
	};

	// DrawRectangle(visual_position.x - width/2, visual_position.y - height, width, height, RED);
	DrawTextureRec(player_sprite, { offset, 0, float(width), float(height) }, sprite_position, WHITE);

}

void Player::update() {
	if (dead) return;

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
		acceleration = -0.2;
	}

	steer();

	Vector3 velocity = Vector3Scale(direction, speed);
	position = Vector3Add(position, velocity);

	visual_position = {position.x, hill_height-position.y};

	speed += acceleration; // Modify speed

	// Clamp speed
	if ( speed < speed_min ) speed = speed_min;
	if ( speed > speed_max ) speed = speed_max;

}

void Player::steer() {
	if ( IsKeyDown(KEY_UP) ) position.y += steer_speed;
	if ( IsKeyDown(KEY_DOWN) ) position.y -= steer_speed;

	if (position.y > hill.road_width) position.y = hill.road_width;
	if (position.y < 0) position.y = 0;

}

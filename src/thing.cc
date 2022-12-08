#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "globals.hh"
#include "hill.hh"
#include "player.hh"
#include "thing.hh"

Thing::Thing() {

}

void Thing::render() {
	DrawRectangle(visual_position.x - width/2, visual_position.y - height, width, height, ORANGE);

}

void Thing::update() {
	float hill_height = hill.get_height(position.x);

	position = Vector3Add(position, velocity);
	visual_position = {position.x, hill_height-position.y};

	if ( collide() ) std::cout << "Collided" << '\n';

}

bool Thing::collide() {
	if (player.position.z != position.z) return false;

	Rectangle player_rect = {
		player.visual_position.x - player.width/2,
		player.visual_position.y - player.height,
		player.width,
		player.height
	};
	Rectangle this_rect = {visual_position.x - width/2, visual_position.y - height, float(width), float(height)};

	return CheckCollisionRecs(player_rect, this_rect);

}
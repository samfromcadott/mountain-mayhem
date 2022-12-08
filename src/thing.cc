#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "globals.hh"
#include "hill.hh"
#include "player.hh"
#include "thing.hh"

Thing::Thing(Vector3 position, Vector3 velocity, int width, int height) {
	this->position = position;
	this->velocity = velocity;
	this->width = width;
	this->height = height;

}

void Thing::render() {
	DrawRectangle(visual_position.x - width/2, visual_position.y - height, width, height, ORANGE);

}

void Thing::update() {
	float hill_height = hill.get_height(position.x);

	position = Vector3Add(position, velocity);
	visual_position = {position.x, hill_height-position.y};

	if ( collide() ) player.dead = true;

}

bool Thing::collide() {
	// if (player.position.z != position.z) return false;

	Rectangle player_rect = {
		player.visual_position.x - player.width/2,
		player.visual_position.y - player.height,
		player.width,
		player.height
	};
	Rectangle this_rect = {visual_position.x - width/2, visual_position.y - height, float(width), float(height)};
	Vector2 player_center = Vector2Add( player.visual_position, {player.width/2, player.height/2} );
	Vector2 this_center = Vector2Add( visual_position, { float(width)/2, float(height)/2 } );

	// return CheckCollisionRecs(player_rect, this_rect);
	return CheckCollisionCircles(player_center, player.width/4, this_center, width/4);

}

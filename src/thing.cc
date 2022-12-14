#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "globals.hh"
#include "hill.hh"
#include "player.hh"
#include "thing.hh"

Thing::Thing(Texture2D* sprite, Vector3 position, Vector3 velocity, int width, int height) {
	this->sprite = sprite;
	this->position = position;
	this->velocity = velocity;
	this->width = width;
	this->height = height;

}

void Thing::render() {
	// DrawRectangle(visual_position.x - width/2, visual_position.y - height, width, height, ORANGE);
	Vector2 sprite_position = {visual_position.x - width/2, visual_position.y - height};
	DrawTextureRec(*sprite, { 0, 0, float(width), float(height) }, sprite_position, WHITE);

}

void Thing::update() {
	float hill_height = hill.get_height(position.x);

	position = Vector3Add(position, velocity);
	visual_position = {position.x, hill_height-position.y};

	if ( collide()  && !player.dead ) {
		player.dead = true;
		PlaySound(explosion_sound);
		StopMusicStream(theme);
	}

}

bool Thing::collide() {
	Vector2 player_center = Vector2Add( player.visual_position, {player.width/2, player.height/2} );
	Vector2 this_center = Vector2Add( visual_position, { float(width)/2, float(height)/2 } );

	return CheckCollisionCircles(player_center, player.width/4, this_center, width/4);

}

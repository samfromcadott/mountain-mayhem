#include <vector>
#include <cmath>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

#include "globals.hh"
#include "player.hh"
#include "hill.hh"
#include "thing.hh"

Hill::Hill() {
	segments = {Slope::FLAT, Slope::FLAT, Slope::FLAT, Slope::FLAT, Slope::FLAT, Slope::FLAT};
	heights = {0, 0, 0, 0, 0, 0};

	// Add segments until the hill is at least 256 pixels long
	while ( segments.size() * segment_length < 256 ) {
		add_segments();
	}

}

void Hill::render() {
	Vector2 start = {0, 0}, end;
	Color grass_color, road_color;

	for (auto& segment : segments) {
		if ( segment == Slope::DOWN ) {
			end = Vector2Add(start, {segment_length, segment_length});
			road_color = {116, 70, 0, 255};
			grass_color = {16, 116, 0, 255};
		}
		else if ( segment == Slope::FLAT ) {
			end = Vector2Add(start, {segment_length, 0});
			road_color = {175, 105, 0, 255};
			grass_color = {25, 185, 0, 255};
		}


		Vector2 road_points[] = { start, end, {end.x, end.y-road_width}, {start.x, start.y-road_width} };

		DrawRectangle(start.x, start.y-512, segment_length, 1024, grass_color);
		DrawTriangleFan(road_points, 4, road_color);

		// Segment view for debugging
		// DrawLine(start.x, start.y, end.x, end.y, BLACK);
		// DrawLine(start.x, start.y, start.x, start.y-road_width, BLUE);

		start = end;

	}

}

void Hill::update() {
	// If the last segment is less than 128 pixels from the player generate more segments
	float hill_length = segments.size() * segment_length;
	if ( hill_length - 256 <= player.position.x ) {
		// segments.push_back(Slope::FLAT);
		// heights.push_back( heights.back() );
		add_segments();
	}
}

float Hill::get_height(float x) {
	int index = int(x / segment_length); // Index of current segment
	float t = (x - index * segment_length) / segment_length; // Proportional distance to next segment

	// Lerp between the height of this segment and the next one
	float y = Lerp(heights[index], heights[index+1], t);
	return y;

}

void Hill::add_segments() {
	Slope type = Slope::DOWN; // Type of segment added
	if ( segments.back() == Slope::DOWN ) type = Slope::FLAT; // Make sure slops aren't repeated

	int length = GetRandomValue(1, 6); // Length of added segments

	for (int i = 0; i < length; i++) {
		float height = heights.back();
		if ( segments.back() == Slope::DOWN ) // Add height if the last slope was down
			height += segment_length;

		segments.push_back(type);
		heights.push_back(height);

		// Add thing to segment
		if ( GetRandomValue(0, 5) == 0 && type == Slope::FLAT )
			add_thing( segments.size()*segment_length - segment_length/2 );

	}

}

void Hill::add_thing(int x) {
	Thing new_thing;
	Vector3 position = {
		float(x),
		float( GetRandomValue(0, road_width) ),
		float( get_height(x) )
	};
	Vector3 velocity = {0, 0, 0};

	// Choose what thing is placed
	int choice = GetRandomValue(1, 100);
	if ( choice == 1 ) {
		position.y = 256;
		velocity.y = -5.0;
		new_thing = Thing(&bigfoot_sprite, position, velocity, 32, 32);
	}
	// Rock
	else if (choice >= 2 && choice <= 60) {
		new_thing = Thing(&rock_sprite, position, velocity, 32, 32);
	}
	// Tree
	else if (choice >= 61 && choice <= 100) {
		new_thing = Thing(&tree_sprite, position, velocity, 32, 64);
	}


	thing_list.push_back(new_thing);

}

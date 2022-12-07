#include <vector>
#include <cmath>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

#include "globals.hh"
#include "player.hh"
#include "hill.hh"

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

		DrawRectangle(start.x, start.y-256, segment_length, 512, grass_color);
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
	if ( hill_length - 128 <= player.position.x ) {
		segments.push_back(Slope::FLAT);
		heights.push_back( heights.back() );
	}
}

float Hill::get_height(float x) {
	int index = int(x / segment_length); // Index of current segment
	float t = (x - index * segment_length) / segment_length; // Proportional distance to next segment

	// Lerp between the height of this segment and the next one
	float y = std::lerp(heights[index], heights[index+1], t);
	return y;

}

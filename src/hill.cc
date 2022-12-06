#include <vector>
#include <cmath>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

#include "hill.hh"

void Hill::render() {
	Vector2 start = {0, 256-64}, end;
	for (auto& segment : segments) {
		if ( segment == Slope::DOWN ) end = Vector2Add(start, {segment_length, segment_length});
		if ( segment == Slope::FLAT ) end = Vector2Add(start, {segment_length, 0});

		DrawLine(start.x, start.y, end.x, end.y, BLACK);
		DrawLine(start.x, start.y, start.x, start.y-road_width, BLUE);
		// Vector2 points[] = { start, end, {end.x, end.y-128}, {start.x, start.y-128} };
		// DrawTriangleFan(points, 4, BROWN);

		start = end;

	}

}

void Hill::update() {

}

float Hill::get_height(float x) {
	int index = int(x / segment_length); // Index of current segment
	float t = (x - index * segment_length) / segment_length; // Proportional distance to next segment

	// Lerp between the height of this segment and the next one
	float y = std::lerp(heights[index], heights[index+1], t);
	return y;

}

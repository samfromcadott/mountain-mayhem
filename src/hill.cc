#include <vector>
#include <raylib.h>
#include <raymath.h>

#include "hill.hh"

void Hill::render() {
	Vector2 start = {0, 100}, end;
	for (auto& segment : segments) {
		if ( segment == Slope::DOWN ) end = Vector2Add(start, {segment_length, segment_length});
		if ( segment == Slope::FLAT ) end = Vector2Add(start, {segment_length, 0});

		DrawLine(start.x, start.y, end.x, end.y, BLACK);

		start = end;

	}

}

void Hill::update() {

}

#pragma once

#include <vector>

enum Slope {DOWN, FLAT};

class Hill {
private:
	void add_segments();

public:
	constexpr static float segment_length = 32;
	constexpr static float road_width = 128;

	std::vector<Slope> segments;
	std::vector<float> heights; // Starting height of each segment

	Hill();
	virtual ~Hill(){}

	void render();
	void update();

	float get_height(float x);

};

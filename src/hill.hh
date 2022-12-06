#pragma once

#include <vector>

enum Slope {DOWN, FLAT};

class Hill {
private:

public:
	const float segment_length = 32;
	const float road_width = 128;

	std::vector<Slope> segments;
	std::vector<float> heights; // Starting height of each segment

	Hill(){}
	virtual ~Hill(){}

	void render();
	void update();

	float get_height(float x);

};

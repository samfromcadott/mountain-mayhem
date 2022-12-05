#pragma once

#include <vector>

enum Slope {DOWN, FLAT};

class Hill {
private:

public:
	const float segment_length = 32;

	std::vector<Slope> segments;

	Hill(){}
	virtual ~Hill(){}

	void render();
	void update();
};

#pragma once

#include <raylib.h>
#include <vector>

class Hill;
class Player;
class Thing;

const int FPS = 60;

extern Player player;
extern Hill hill;
extern std::vector<Thing> thing_list;

// Images
extern Texture2D rock_sprite;
extern Texture2D tree_sprite;

// Audio
extern Sound explosion_sound;

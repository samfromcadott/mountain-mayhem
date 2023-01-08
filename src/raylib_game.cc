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
#include "raylib.h"

#if defined(PLATFORM_WEB)
	#define CUSTOM_MODAL_DIALOGS
	#include <emscripten/emscripten.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

#include "globals.hh"
#include "player.hh"
#include "hill.hh"
#include "thing.hh"

#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
	#define LOG(...) printf(__VA_ARGS__)
#else
	#define LOG(...)
#endif

static const int screenWidth = 256;
static const int screenHeight = 256;

static unsigned int screenScale = 3;

static RenderTexture2D target = { 0 };

void start_game();
static void UpdateDrawFrame(); // Main loop function (needed for web support)

Player player;
Hill hill;
Camera2D camera;
std::vector<Thing> thing_list;

// Textures
Texture2D player_sprite;
Texture2D rock_sprite;
Texture2D tree_sprite;
Texture2D bigfoot_sprite;

// Audio
Sound explosion_sound;
Music theme;

int main() {
#if !defined(_DEBUG)
	SetTraceLogLevel(LOG_NONE); // Disable raylib trace log messsages
#endif

	// Initialization
	InitWindow(screenWidth*screenScale, screenHeight*screenScale, "Mountain Mayhem");
	InitAudioDevice();

	target = LoadRenderTexture(screenWidth, screenHeight);
	SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

	camera.offset = (Vector2){ 96, 192 };
	camera.rotation = 0.0;
	camera.zoom = 1.0;

	player_sprite = LoadTexture("resources/player.png");
	rock_sprite = LoadTexture("resources/rock.png");
	tree_sprite = LoadTexture("resources/tree.png");
	bigfoot_sprite = LoadTexture("resources/bigfoot.png");

	explosion_sound = LoadSound("resources/explode.wav");
	theme = LoadMusicStream("resources/theme.mp3");

	start_game();

#if defined(PLATFORM_WEB)
	// Main loop for web
	emscripten_set_main_loop(UpdateDrawFrame, FPS, 1);
#else
	// Main loop for desktop
	SetTargetFPS(FPS);

	while (!WindowShouldClose()) {
		UpdateDrawFrame();
	}
#endif

	// Unloading
	UnloadRenderTexture(target);
	UnloadTexture(player_sprite);
	UnloadTexture(rock_sprite);
	UnloadTexture(tree_sprite);
	UnloadTexture(bigfoot_sprite);
	UnloadSound(explosion_sound);
	UnloadMusicStream(theme);

	CloseAudioDevice();
	CloseWindow();

	return 0;

}

void start_game() {
	player = Player();
	hill = Hill();
	thing_list.clear();

	StopMusicStream(theme);
	PlayMusicStream(theme);

}

void UpdateDrawFrame() {
	if ( IsKeyPressed(KEY_R) ) start_game();

	UpdateMusicStream(theme);

	// Update
	hill.update();
	player.update();

	for (int i = 0; i < thing_list.size(); i++) {
		if ( thing_list[i].position.x < player.position.x - 128 ) thing_list.erase( thing_list.begin()+i );
		thing_list[i].update();
	}

	camera.target.x = std::floor(player.position.x);
	camera.target.y = hill.get_height(player.position.x);

	BeginTextureMode(target);

		// Render
		ClearBackground(RAYWHITE);

		BeginMode2D(camera);
			hill.render();

			for (int i = 0; i < thing_list.size(); i++) {
				thing_list[i].render();
			}

			player.render();
		EndMode2D();

		// Distance counter
		const int box_offset = 8;
		const int text_offset = box_offset+2;
		const int text_size = 8;

		int distance = (player.position.x - 128) / hill.segment_length;
		DrawRectangle(box_offset, box_offset, 64, 12, WHITE);
		DrawText(TextFormat("%09im", distance), text_offset, text_offset, text_size, BLACK);

	EndTextureMode();

	BeginDrawing();
		ClearBackground(RAYWHITE);

		// Draw render texture to screen scaled as required
		DrawTexturePro(
			target.texture,
			(Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height },
			(Rectangle){ 0, 0, (float)target.texture.width*screenScale, (float)target.texture.height*screenScale },
			(Vector2){ 0, 0 },
			0.0f,
			WHITE
		);

	EndDrawing();

}
#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <iostream>

#include "globals.hh"
#include "hill.hh"
#include "player.hh"

Player::Player() {
	position = {128, 64, 0};
	speed = 2.0;
	dead = false;

}

void Player::render() {
	if (dead) {
		Color explosion_color = BLACK;
		const float rate = 8; // Rate explosion color changes

		if ( int( GetTime() * rate ) % 2 == 0 ) explosion_color = WHITE;

		DrawCircle(visual_position.x - width/2, visual_position.y - height/2, width/2, explosion_color);

		return;

	}

	float offset = 0.0;
	int index = int(position.x / hill.segment_length); // Index of current segment
	if ( hill.segments[index] == Slope::DOWN ) offset = 32; // When doing down hill use other sprite

	Vector2 sprite_position = {
		std::floor(visual_position.x - width/2), // floor prevent jittering
		std::floor(visual_position.y - height)
	};

	// DrawRectangle(visual_position.x - width/2, visual_position.y - height, width, height, RED);
	DrawTextureRec(player_sprite, { offset, 0, float(width), float(height) }, sprite_position, WHITE);

}

void Player::update() {
	if (dead) return;

	int index = int(position.x / hill.segment_length); // Index of current segment
	Slope segment = hill.segments[index];
	float hill_height = hill.get_height(position.x);

	Vector3 direction;
	float acceleration;

	if ( segment == Slope::DOWN ){
		direction = Vector3Normalize( {1.0, 0.0, 1.0} );
		acceleration = 0.2;
	}
	else if ( segment == Slope::FLAT ) {
		direction = {1.0, 0.0, 0.0};
		acceleration = -0.2;
	}

	steer();

	Vector3 velocity = Vector3Scale(direction, speed);
	position = Vector3Add(position, velocity);

	visual_position = {position.x, hill_height-position.y};

	speed += acceleration; // Modify speed

	// Clamp speed
	if ( speed < speed_min ) speed = speed_min;
	if ( speed > speed_max ) speed = speed_max;

}

void Player::steer() {
	if ( IsKeyDown(KEY_UP) ) position.y += steer_speed;
	if ( IsKeyDown(KEY_DOWN) ) position.y -= steer_speed;

	if (position.y > hill.road_width) position.y = hill.road_width;
	if (position.y < 0) position.y = 0;

}
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

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

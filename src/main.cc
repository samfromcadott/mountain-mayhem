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
Thing thing;
std::vector<Thing> thing_list;

int main() {
#if !defined(_DEBUG)
	SetTraceLogLevel(LOG_NONE); // Disable raylib trace log messsages
#endif

	// Initialization
	InitWindow(screenWidth*screenScale, screenHeight*screenScale, "Mountain Mayhem");

	target = LoadRenderTexture(screenWidth, screenHeight);
	SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

	camera.offset = (Vector2){ 128, 192 };
	camera.rotation = 0.0;
	camera.zoom = 1.0;

	start_game();

	thing = Thing({128, 128, 0}, {0, -2, 0}, 32, 32);

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
	CloseWindow();

	return 0;

}

void start_game() {
	player = Player();
	hill = Hill();

}

void UpdateDrawFrame() {
	if ( IsKeyPressed(KEY_R) ) start_game();

	// Update
	hill.update();
	player.update();
	// thing.update();
	for (int i = 0; i < thing_list.size(); i++) {
		// if ( thing_list[i].position.x < player.position.x - 128 ) thing_list.erase( thing_list.begin()+i );
		thing_list[i].update();
	}

	camera.target.x = std::floor(player.position.x);
	camera.target.y = hill.get_height(player.position.x);

	BeginTextureMode(target);

		// Render
		ClearBackground(RAYWHITE);

		BeginMode2D(camera);
			hill.render();
			// player.render();
			// thing.render();
			for (int i = 0; i < thing_list.size(); i++) {
				thing_list[i].render();
			}

			player.render();
		EndMode2D();

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

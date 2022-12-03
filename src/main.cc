#include "raylib.h"

#if defined(PLATFORM_WEB)
	#define CUSTOM_MODAL_DIALOGS
	#include <emscripten/emscripten.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
	#define LOG(...) printf(__VA_ARGS__)
#else
	#define LOG(...)
#endif

typedef enum {
	SCREEN_LOGO = 0,
	SCREEN_TITLE,
	SCREEN_GAMEPLAY,
	SCREEN_ENDING
} GameScreen;

static const int screenWidth = 256;
static const int screenHeight = 256;

static unsigned int screenScale = 3;
// static unsigned int prevScreenScale = 1;

static RenderTexture2D target = { 0 };

static void UpdateDrawFrame();      // Update and Draw one frame

int main() {
#if !defined(_DEBUG)
	SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif

	InitWindow(screenWidth*screenScale, screenHeight*screenScale, "Mountain Mayhem");

	target = LoadRenderTexture(screenWidth, screenHeight);
	SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		UpdateDrawFrame();
	}
#endif

	UnloadRenderTexture(target);
	CloseWindow();

	return 0;

}

void UpdateDrawFrame() {
	// Screen scale logic (x2)
	// if (IsKeyPressed(KEY_ONE)) screenScale = 1;
	// else if (IsKeyPressed(KEY_TWO)) screenScale = 2;
	// else if (IsKeyPressed(KEY_THREE)) screenScale = 3;

	// if (screenScale != prevScreenScale) {
	// 	// Scale window to fit the scaled render texture
	// 	SetWindowSize(screenWidth*screenScale, screenHeight*screenScale);
	//
	// 	// Scale mouse proportionally to keep input logic inside the 256x256 screen limits
	// 	SetMouseScale(1.0f/(float)screenScale, 1.0f/(float)screenScale);
	//
	// 	prevScreenScale = screenScale;
	// }-----------------------------------------------------------------------------

	BeginTextureMode(target);
		ClearBackground(RAYWHITE);

		DrawRectangle(10, 10, screenWidth - 20, screenHeight - 20, SKYBLUE);
		DrawLine(32, 32, 128, 96, BLACK);

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

		DrawCircleLines(GetMouseX(), GetMouseY(), 10, MAROON);

	EndDrawing();
}

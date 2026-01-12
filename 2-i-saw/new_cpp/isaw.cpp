#include "raylib.h"
#include "raymath.h"

// Screen dimensions
//#define SCREEN_WIDTH 2560
//#define SCREEN_HEIGHT 1600

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main() {
	//initialize the window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Zombie Love");
	SetTargetFPS(70);

	bool gameOver = false;

	while (!WindowShouldClose()) {
		//Game Logic
		//Drawing
		BeginDrawing();
		ClearBackground(RAYWHITE);

		EndDrawing();
	}

	CloseWindow();
	
	return 0;
}

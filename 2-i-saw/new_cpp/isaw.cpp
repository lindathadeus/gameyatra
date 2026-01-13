#include "raylib.h"
#include "raymath.h"

// Screen dimensions
//#define SCREEN_WIDTH 2560
//#define SCREEN_HEIGHT 1600

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Entity structure with body dimensions
typedef struct Entity {
    Vector2 position; // Center of the head
    float radius;     // Head radius
    float bodyWidth;  // Body width
    float bodyHeight; // Body height
} Entity;

// Cage structure     
typedef struct Cage {
    Vector2 position;
    float width;
    float height;
} Cage;

// Platform structure
typedef struct Platform {
        Vector2 position;
        float width;
        float height;
};

int main() {
	//initialize the window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Zombie Love");
	SetTargetFPS(70);

	// Player Setup
	Entity player = {{400, 400}, 10.0f, 20.0f, 40.0f};

	// Zombie Setup
	Entity zombie = {{200, 400}, 10.0f, 20.0f, 40.0f};
	const float zombieSpeed = 2.0f;
	const float followThreshold = 150.0f;

	// Cage Setup
	Cage cage = {{600, 300}, 100, 150};

	// Platform Setup
	Platform platform = {{50, 450}, 700, 30};

	// Game Variables
	bool gameOver = false;

	while (!WindowShouldClose()) {
		
		//Game Logic
		if(!gameOver) {
			
		}
		//Drawing
		BeginDrawing();
		ClearBackground(RAYWHITE);

		EndDrawing();
	}

	CloseWindow();
	
	return 0;
}

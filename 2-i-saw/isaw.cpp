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

// Updated drawing function for entities
void DrawEntity(Entity entity, Color headColor, Color bodyColor) {
    // Draw body (rectangle)
    Vector2 bodyPosition = {entity.position.x - entity.bodyWidth / 2, entity.position.y + entity.radius};
    DrawRectangleV(bodyPosition, (Vector2){entity.bodyWidth, entity.bodyHeight}, bodyColor);

    // Draw head (circle)
    DrawCircleV(entity.position, entity.radius, headColor);
}

// Function to draw a cage with vertical bars
void DrawCage(Cage cage, bool isZombieSafe, int barCount, Color color) {
    // Bar width and spacing calculation
    float barWidth = cage.width / (barCount * 2 - 1); // Space between bars is equal to the bar width
    for (int i = 0; i < barCount; i++) {
        // Calculate the position of each bar
        float barX = cage.position.x + i * 2 * barWidth;
	if (!isZombieSafe)
        DrawRectangle(barX, cage.position.y - cage.height, barWidth, cage.height, color);
	else
        DrawRectangle(barX, cage.position.y, barWidth, cage.height, color);
    }

    // Draw the top and bottom of the cage
    DrawRectangle(cage.position.x, cage.position.y, cage.width, barWidth, color); // Top
    DrawRectangle(cage.position.x, cage.position.y + cage.height - barWidth, cage.width, barWidth, color); // Bottom
}

// Function to draw the platform
void DrawPlatform(Platform P, Color color) {
        DrawRectangle(P.position.x, P.position.y, P.width, P.height, color);
}

// Function to check if an entity is inside the cage
bool IsEntityInCage(Entity entity, Cage cage) {
    return (entity.position.x > cage.position.x &&
            entity.position.x < cage.position.x + cage.width &&
            entity.position.y > cage.position.y &&
            entity.position.y < cage.position.y + cage.height);
}

// Function to check if two positions are within a threshold
bool IsWithinThreshold(Vector2 pos1, Vector2 pos2, float threshold) {
    return Vector2Distance(pos1, pos2) <= threshold;
}

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
	bool isZombieSafe = false;

	while (!WindowShouldClose()) {
		
		//Game Logic
		if(!gameOver) {
			// Player movement
			if (IsKeyDown(KEY_RIGHT)) player.position.x += 4.0f;
			if (IsKeyDown(KEY_LEFT)) player.position.x -= 4.0f;
			
			// Is Zombie safe?
			isZombieSafe = IsEntityInCage(zombie, cage);

			// Zombie movement
			if (IsWithinThreshold(player.position, zombie.position, followThreshold)) {
				if (!isZombieSafe) {
				    	Vector2 direction = Vector2Subtract(player.position, zombie.position);
				    	zombie.position = Vector2Add(zombie.position, Vector2Scale(Vector2Normalize(direction), zombieSpeed));
				} else {
					zombie.position.x = cage.position.x + cage.width / 2;
				}
			}			
		}
		//Drawing
		BeginDrawing();
		ClearBackground(RAYWHITE);

		if (!gameOver) {
			// Draw Player
			DrawEntity(player, BLACK, BLACK);
			
			// Draw Zombie
			DrawEntity(zombie, PINK, PINK);

			// Draw Cage
			DrawCage(cage, isZombieSafe, 5, DARKGRAY);

			// Draw Platform
			DrawPlatform(platform, BROWN);
		}

		EndDrawing();
	}

	CloseWindow();
	
	return 0;
}

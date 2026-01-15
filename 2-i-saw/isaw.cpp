#include "raylib.h"
#include "raymath.h"

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

enum class ZombieState {
	//idle,
	Chasing,
	InCage
};

enum class PlayerState {
	Hugged,
	SafeDistant,
	TrulySafe
};

// Updated drawing function for entities
void DrawEntity(Entity entity, Color headColor, Color bodyColor) {
    // Draw body (rectangle)
    Vector2 bodyPosition = {entity.position.x - entity.bodyWidth / 2, entity.position.y + entity.radius};
    DrawRectangleV(bodyPosition, (Vector2){entity.bodyWidth, entity.bodyHeight}, bodyColor);

    // Draw head (circle)
    DrawCircleV(entity.position, entity.radius, headColor);
}

float rotationAngle = 0.0f; // Rotation angle for falling

// Function to draw a rotated entity during the fall animation
void DrawRotatedEntity(Entity entity, float angle, Color headColor, Color bodyColor)
{
	entity.position.x = entity.position.x - entity.bodyHeight - 3*entity.radius;
	entity.position.y = entity.position.y + 3*entity.radius;
    // Draw body (rectangle)
	DrawRectangleV((Vector2) {entity.position.x, entity.position.y}, 
	(Vector2){entity.bodyHeight, entity.bodyWidth}, bodyColor);

    // Draw head (circle)
	Vector2 headPos = {
        entity.position.x - entity.bodyHeight + 3*entity.radius,         
        entity.position.y + entity.radius
    };
    DrawCircleV(headPos, entity.radius, headColor);
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

// Function to check collision between two entities (circular collision)
bool IsCollision(Entity entity1, Entity entity2) {
	return Vector2Distance(entity1.position, entity2.position) <= (entity1.radius + entity2.radius);
}

// Function to check if the entity can move
bool canMove(Entity player, float delta_x) {
	float new_position = player.position.x + delta_x;
	return (new_position > 0) && (new_position < SCREEN_WIDTH);
}

// Update player position as per the key pressed
void movePlayer(Entity* player, const float playerSpeed) {
	// Player movement
	if (IsKeyDown(KEY_RIGHT)) if (canMove(*player, playerSpeed)) player->position.x += playerSpeed;
	if (IsKeyDown(KEY_LEFT)) if (canMove(*player, -playerSpeed)) player->position.x -= playerSpeed;
}

// Update zombie position as per the player's position based on a threshold
void moveZombie(Entity* zombie, Entity* player, Cage* cage, ZombieState zombieState, const float zombieSpeed, const float followThreshold) {
	//zombie will chase the player and also gets caught in the cage
	if (IsWithinThreshold(player->position, zombie->position, followThreshold)) {
		if (zombieState == ZombieState::Chasing) {
			Vector2 direction = Vector2Subtract(player->position, zombie->position);
			zombie->position = Vector2Add(zombie->position, Vector2Scale(Vector2Normalize(direction), zombieSpeed)); 
		} else if (zombieState == ZombieState::InCage) {
			zombie->position.x = cage->position.x + cage->width / 2;
			//player.position.x = cage.position.x - cage.width / 4;
		}
	}

}

int main() {
	//initialize the window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Zombie Love");
	SetTargetFPS(70);

	// Player Setup
	Entity player = {{400, 400}, 10.0f, 20.0f, 40.0f};
	const float playerSpeed = 4.0f;

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
		
		PlayerState playerState ;
		ZombieState zombieState ;
		//Game Logic
		if(!gameOver) {
			// Player movement
			movePlayer(&player, playerSpeed);	
			
			zombieState =
    				IsEntityInCage(zombie, cage) ? ZombieState::InCage
                                 			: ZombieState::Chasing;

			// Zombie movement
			moveZombie(&zombie, &player, &cage, zombieState, zombieSpeed, followThreshold);

			// Did Zombie hug?
			playerState =
				IsCollision(player, zombie) ? PlayerState::Hugged : PlayerState::SafeDistant;
		}
			
		//Drawing
		BeginDrawing();
		ClearBackground(RAYWHITE);

		if (!gameOver) {
			// Draw Player
			if (playerState != PlayerState::Hugged) DrawEntity(player, BLACK, BLACK);
			else {
				DrawRotatedEntity(player, rotationAngle, BLACK, BLACK);
				//gameOver = true;
			}
			// Draw Zombie
			DrawEntity(zombie, PINK, PINK);

			// Draw Cage
			DrawCage(cage, zombieState == ZombieState::InCage, 5, DARKGRAY);

			// Draw Platform
			DrawPlatform(platform, BROWN);
		}

		EndDrawing();
	}

	CloseWindow();
	
	return 0;
}

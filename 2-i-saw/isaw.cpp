#include "raylib.h"
#include "raymath.h"

// Screen dimensions
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

// Function to check if two positions are within a threshold
bool IsWithinThreshold(Vector2 pos1, Vector2 pos2, float threshold) {
    return Vector2Distance(pos1, pos2) <= threshold;
}

// Function to check if an entity is inside the cage
bool IsEntityInCage(Entity entity, Cage cage) {
    return (entity.position.x > cage.position.x &&
            entity.position.x < cage.position.x + cage.width &&
            entity.position.y > cage.position.y &&
            entity.position.y < cage.position.y + cage.height);
}


// Updated drawing function for entities
void DrawEntity(Entity entity, Color headColor, Color bodyColor) {
    // Draw body (rectangle)
    Vector2 bodyPosition = {entity.position.x - entity.bodyWidth / 2, entity.position.y + entity.radius};
    DrawRectangleV(bodyPosition, (Vector2){entity.bodyWidth, entity.bodyHeight}, bodyColor);

    // Draw head (circle)
    DrawCircleV(entity.position, entity.radius, headColor);
}

// Function to check collision between two entities (circular collision)
bool IsCollision(Entity entity1, Entity entity2) {
    return Vector2Distance(entity1.position, entity2.position) <= (entity1.radius + entity2.radius);
}

// Function to draw a cage with vertical bars
void DrawCage(Cage cage, int barCount, Color color) {
    // Bar width and spacing calculation
    float barWidth = cage.width / (barCount * 2 - 1); // Space between bars is equal to the bar width
    for (int i = 0; i < barCount; i++) {
        // Calculate the position of each bar
        float barX = cage.position.x + i * 2 * barWidth;
        DrawRectangle(barX, cage.position.y, barWidth, cage.height, color);
    }

    // Draw the top and bottom of the cage
    DrawRectangle(cage.position.x, cage.position.y, cage.width, barWidth, color); // Top
    DrawRectangle(cage.position.x, cage.position.y + cage.height - barWidth, cage.width, barWidth, color); // Bottom
}

// Additional variables for the falling animation
float fallTimer = 0.0f;    // Tracks time for falling animation
bool isFalling = false;    // Indicates if the player is in the falling state
float rotationAngle = 0.0f; // Rotation angle for falling

// Function to draw a rotated entity during the fall animation
void DrawRotatedEntity(Entity entity, float angle, Color headColor, Color bodyColor) {
    Vector2 origin = {entity.position.x, entity.position.y + entity.radius + entity.bodyHeight / 2};
    Vector2 bodyPosition = {0, entity.radius}; // Offset for the body

    // Draw body
    DrawRectanglePro(
        (Rectangle){entity.position.x - entity.bodyWidth / 2, entity.position.y + entity.radius, entity.bodyWidth, entity.bodyHeight},
        (Vector2){entity.bodyWidth / 2, entity.bodyHeight / 2},
        angle,
        bodyColor
    );

    // Draw head (rotated with body)
    DrawCircleV(Vector2Add(origin, Vector2Rotate(bodyPosition, angle * DEG2RAD)), entity.radius, headColor);
}

int main() {
    // Initialize the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Reddie Love");
    SetTargetFPS(60);

    // Player setup
    Entity player = {{400, 300}, 10.0f, 20.0f, 40.0f};

    // Reddie setup
    Entity zombie = {{200, 300}, 10.0f, 20.0f, 40.0f};
    const float zombieSpeed = 2.0f;
    const float followThreshold = 150.0f;

    // Cage setup
    Cage cage = {{600, 100}, 100, 150}; // Wider cage for visual improvement

    // Game variables
    int level = 1;
    bool isReddieSafe = false;
    bool gameOver = false;
    const char *levelNarrative = "";

    while (!WindowShouldClose()) {
        if (!gameOver) {
            // Level narratives
            switch (level) {
                case 1:
                    levelNarrative = "I loved her but she was a zombie. So I put her safe in a cage.";
                    break;
                case 2:
                    levelNarrative = "I knew she loved me too, because she always tried to hug me.";
                    break;
                case 3:
                    levelNarrative = "Sometimes, the cage was hard to reach.";
                    cage.position = (Vector2){300, 450}; // Move the cage for difficulty
                    break;
                default:
                    levelNarrative = "To be continued...";
                    break;
            }

            if (!isFalling) {
                // Player movement
                if (IsKeyDown(KEY_RIGHT)) player.position.x += 4.0f;
                if (IsKeyDown(KEY_LEFT)) player.position.x -= 4.0f;
                if (IsKeyDown(KEY_UP)) player.position.y -= 4.0f;
                if (IsKeyDown(KEY_DOWN)) player.position.y += 4.0f;

                // Reddie movement
                if (IsWithinThreshold(player.position, zombie.position, followThreshold)) {
                    Vector2 direction = Vector2Subtract(player.position, zombie.position);
                    zombie.position = Vector2Add(zombie.position, Vector2Scale(Vector2Normalize(direction), zombieSpeed));
                }

                // Check if the zombie is in the cage
                if (IsEntityInCage(zombie, cage)) {
                    isReddieSafe = true;
                }

                // Level progression
                if (isReddieSafe) {
                    level++;
                    isReddieSafe = false;
                    zombie.position = (Vector2){200, 300}; // Reset zombie position
                }

                // Check for collision between player and zombie
                if (IsCollision(player, zombie)) {
                    isFalling = true;
                    fallTimer = 2.0f; // 2 seconds for fall animation
                    rotationAngle = 0.0f; // Start rotation at 0
                }
            } else {
                // Falling animation logic
                fallTimer -= GetFrameTime();
                rotationAngle += 100.0f * GetFrameTime(); // Rotate 100 degrees per second
                player.position.y += 100.0f * GetFrameTime(); // Move down

                if (fallTimer <= 0.0f) {
                    gameOver = true;
                    levelNarrative = "Game Over. The zombie hugged you!";
                }
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw level narrative
        DrawText(levelNarrative, 10, 10, 20, DARKGRAY);

        if (!gameOver) {
            // Draw player
            if (!isFalling) {
                DrawEntity(player, BROWN, BROWN);
            } else {
                DrawRotatedEntity(player, rotationAngle, BROWN, BROWN);
            }

            // Draw zombie
            DrawEntity(zombie, RED, RED);

            // Draw cage
            DrawCage(cage, 5, DARKGRAY); // 5 bars for the cage
        } else {
            // Display Game Over screen
            DrawText("Press R to Restart", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 20, DARKGRAY);
        }

        EndDrawing();

        // Restart game if game over
        if (gameOver && IsKeyPressed(KEY_R)) {
            gameOver = false;
            level = 1;
            player.position = (Vector2){400, 300};
            zombie.position = (Vector2){200, 300};
            isFalling = false;
            fallTimer = 0.0f;
        }
    }

    // Close the window
    CloseWindow();

    return 0;
}

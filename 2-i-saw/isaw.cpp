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
} Platform;

// States for a Zombie Entity
enum class ZombieState {
	Chasing,
	InCage
};

// States for a Player Entity
enum class PlayerState {
	Hugged,
	SafeDistant
};

#define NARRATIVE_COUNT 3

typedef struct Narrative {
    const char* entries[NARRATIVE_COUNT];
    int selectedIndex;
} Narrative; 

void InitNarrative(Narrative* nr) {
    nr->entries[0] = "I loved her but she was a zombie. So I put her safe in a cage.";
    nr->entries[1] = "I knew she loved me too, because she always tried to hug me.";
    nr->entries[2] = "Sometimes, the cage was hard to reach.";
    nr->selectedIndex = 0;
}

void DrawNarrative(const Narrative* nr, int i) {
        DrawText(nr->entries[i], 10, 10, 20, DARKGRAY);
}

// Level for holding all the above objects
typedef struct Level {
	Entity player;
	Entity zombie;
	Cage cage;
	Platform platform;
	const char* levelNarrative = "";

	PlayerState playerState;
	ZombieState zombieState;

	bool gameOver;
	unsigned int level_id = 0;
} Level;

typedef struct LevelManager {
	Level currentLevel;
} LevelManager;

// Updated drawing function for entities
void DrawEntity(Entity entity, Color headColor, Color bodyColor) {
    // Draw body (rectangle)
    Vector2 bodyPosition = {entity.position.x - entity.bodyWidth / 2, entity.position.y + entity.radius};
    DrawRectangleV(bodyPosition, (Vector2){entity.bodyWidth, entity.bodyHeight}, bodyColor);

    // Draw head (circle)
    DrawCircleV(entity.position, entity.radius, headColor);
}

// Function to draw a rotated entity during the fall animation
void DrawRotatedEntity(Entity entity, Color headColor, Color bodyColor)
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
        DrawRectangle(barX, !isZombieSafe ? cage.position.y - cage.height : cage.position.y, barWidth, cage.height, color);
    }

    // Draw the top and bottom of the cage
    DrawRectangle(cage.position.x, cage.position.y, cage.width, barWidth, color); // Top
    DrawRectangle(cage.position.x, cage.position.y + cage.height - barWidth, cage.width, barWidth, color); // Bottom
}

// Function to draw the platform
void DrawPlatform(Platform P, Color color) {
        DrawRectangle(P.position.x, P.position.y, P.width, P.height, color);
}

// Gameplay Helpers
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
bool CanMove(Entity player, float delta_x) {
	float new_x = player.position.x + delta_x;
	return (new_x > 0) && (new_x < SCREEN_WIDTH);
}

// Update player position as per the key pressed
void UpdatePlayer(Entity* player, const float playerSpeed) {
	// Player movement
	if (IsKeyDown(KEY_RIGHT)) if (CanMove(*player, playerSpeed)) player->position.x += playerSpeed;
	if (IsKeyDown(KEY_LEFT)) if (CanMove(*player, -playerSpeed)) player->position.x -= playerSpeed;
}

// Update zombie position as per the player's position based on a threshold
void UpdateZombie(Entity* zombie, Entity* player, Cage* cage, ZombieState zombieState, const float zombieSpeed, const float followThreshold) {
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

// Level infra
// =======================================================
// Level Lifecycle
// =======================================================

void InitLevel(Level* level) {
    level->player = {{400, 400}, 10, 20, 40};
    level->zombie = {{200, 400}, 10, 20, 40};
    level->cage   = {{600, 300}, 100, 150};
    level->platform = {{50, 450}, 700, 30};
	
    level->levelNarrative = "I loved her but she was a zombie, So I put her in a ";

    level->gameOver = false;
    level->playerState = PlayerState::SafeDistant;
    level->zombieState = ZombieState::Chasing;
}

void UpdateLevel(Level* level) {
    if (level->gameOver) return;

    UpdatePlayer(&level->player, 4.0f);

    level->zombieState =
        IsEntityInCage(level->zombie, level->cage)
        ? ZombieState::InCage
        : ZombieState::Chasing;

    UpdateZombie(&level->zombie,
                 &level->player,
                 &level->cage,
                 level->zombieState,
                 2.0f,
                 150.0f);
        
    level->playerState =
        IsCollision(level->player, level->zombie)
        ? PlayerState::Hugged
        : PlayerState::SafeDistant;
}

void DrawLevel(Level* level, Narrative* nr) {
    if (level->playerState == PlayerState::Hugged)
        DrawRotatedEntity(level->player, BLACK, BLACK);
    else
        DrawEntity(level->player, BLACK, BLACK);

    DrawEntity(level->zombie, PINK, PINK);
    DrawCage(level->cage, level->zombieState == ZombieState::InCage, 5, DARKGRAY);
    DrawPlatform(level->platform, BROWN);
    DrawNarrative(nr, level->level_id);
}

// Game Menu
#define MENU_ITEM_COUNT 3

typedef struct Menu {
    const char* entries[MENU_ITEM_COUNT];
    int selectedIndex;
} Menu;

void InitMenu(Menu* menu) { 
    menu->entries[0] = "Start Game";
    menu->entries[1] = "Select Level";
    menu->entries[2] = "Exit";
    menu->selectedIndex = 0;
}


enum class GameState {
    Menu,
    Playing,
    Exit
};

GameState UpdateMenu(Menu* menu) {
    if (IsKeyPressed(KEY_DOWN)) {
        menu->selectedIndex++;
        if (menu->selectedIndex >= MENU_ITEM_COUNT)
            menu->selectedIndex = 0;
    }

    if (IsKeyPressed(KEY_UP)) {
        menu->selectedIndex--;
        if (menu->selectedIndex < 0)
            menu->selectedIndex = MENU_ITEM_COUNT - 1;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        switch (menu->selectedIndex) {
            case 0: return GameState::Playing;
            case 2: return GameState::Exit;
        }
    }
    return GameState::Menu;
}

void DrawMenu(const Menu* menu) {
    DrawText("Zombie", 250, 50, 40, BLACK);
    DrawText("Love",   390, 50, 40, PINK);

    for (int i = 0; i < MENU_ITEM_COUNT; i++) {
        Color color = (i == menu->selectedIndex) ? PINK : BLACK;
        DrawText(menu->entries[i], 150, 230 + i * 40, 30, color);
    }
}

// Game Loop
int main() {
	//initialize the window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Zombie Love");
	SetTargetFPS(70);

	// Level Setup
	LevelManager levelManager;
	Menu menu;
	Narrative nr;

	InitLevel(&levelManager.currentLevel);
	InitMenu(&menu);
	InitNarrative(&nr);

	GameState gameState = GameState::Menu;

	while (!WindowShouldClose() && (gameState != GameState::Exit)) {
		//Game Logic
    		switch (gameState) {
        		case GameState::Menu:
            			gameState = UpdateMenu(&menu);
            			break;
        		case GameState::Playing:
            			UpdateLevel(&levelManager.currentLevel);
            			break;
        		default: break;
    		}
			
		//Drawing
		BeginDrawing();
		ClearBackground(RAYWHITE);

		switch (gameState) {
			case GameState::Menu:
				DrawMenu(&menu);
				break;

			case GameState::Playing:
				DrawLevel(&levelManager.currentLevel, &nr);
				break;

			default:
				DrawMenu(&menu);
				break;
		}
		
		EndDrawing();
	}

	CloseWindow();
	
	return 0;
}

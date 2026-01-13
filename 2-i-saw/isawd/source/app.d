import raylib;

// Entity structure with body dimensions
struct Entity {
	Vector2 position; // Center of the head
	float radius;     // Head radius
	float bodyWidth;  // Body width
	float bodyHeight; // Body height
};

// Cage structure
struct Cage {
	Vector2 position;
	float width;
	float height;
};

// Platform structure
struct Platform {
	Vector2 position;
	float width;
	float height;
};

void main()
{
    // call this before using raylib
    validateRaylibBinding();

    InitWindow(800, 600, "Zombie Love");
    SetTargetFPS(50);

	//Player Setup
	Entity player = {{400, 400}, 10.0f, 20.0f, 40.0f};

	// Zombie Setup
	Entity zombie = {{200, 400}, 10.0f, 20.0f, 40.0f};
	const float zombieSpeed = 2.0f;
	const float followThreshold = 150.0f;

	// Cage Setup
	Cage cage = {{600, 300}, 100, 150};
	
	// Platform
	Platform platform = {{50, 450}, 700, 30};

	// Game Variables
	bool gameOver = false;
	
    	while (!WindowShouldClose())
    	{
		// Game Logic
		if(!gameOver) {
			
		}
	
        	BeginDrawing();
        	ClearBackground(Colors.RAYWHITE);
        
        	EndDrawing();
    	}
    	
	CloseWindow();
}

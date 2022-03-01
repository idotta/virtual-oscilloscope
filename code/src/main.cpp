#include <raylib.h>

#define PHYSAC_IMPLEMENTATION
#include <extras/physac.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 450;

constexpr float VELOCITY = 0.5f;

int main(void)
{
    // Initialization
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GameTest - physics movement");

    // Initialize physics and default physics bodies
    InitPhysics();

    // Create floor and walls rectangle physics body
    PhysicsBody floor = CreatePhysicsBodyRectangle(Vector2{.x = SCREEN_WIDTH / 2.0f, .y = (float)SCREEN_HEIGHT}, (float)SCREEN_WIDTH, 100, 10);
    PhysicsBody platformLeft = CreatePhysicsBodyRectangle(Vector2{.x = SCREEN_WIDTH * 0.25f, .y = SCREEN_HEIGHT * 0.6f}, SCREEN_WIDTH * 0.25f, 10, 10);
    PhysicsBody platformRight = CreatePhysicsBodyRectangle(Vector2{.x = SCREEN_WIDTH * 0.75f, .y = SCREEN_HEIGHT * 0.6f}, SCREEN_WIDTH * 0.25f, 10, 10);
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle(Vector2{.x = -5, .y = SCREEN_HEIGHT / 2.0f}, 10, (float)SCREEN_HEIGHT, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle(Vector2{.x = (float)SCREEN_WIDTH + 5, .y = SCREEN_HEIGHT / 2.0f}, 10, (float)SCREEN_HEIGHT, 10);

    // Disable dynamics to floor and walls physics bodies
    floor->enabled = false;
    platformLeft->enabled = false;
    platformRight->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;

    // Create movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle(Vector2{.x = SCREEN_WIDTH / 2.0f, .y = SCREEN_HEIGHT / 2.0f}, 50, 50, 1);
    body->freezeOrient = true; // Constrain body rotation to avoid little collision torque amounts

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Texture2D texture = LoadTexture(ASSETS_PATH"test.png"); // Check README.md for how this works

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdatePhysics(); // Update physics system

        if (IsKeyPressed(KEY_R)) // Reset physics input
        {
            // Reset movement physics body position, velocity and rotation
            body->position = Vector2{.x = SCREEN_WIDTH / 2.0f, .y = SCREEN_HEIGHT / 2.0f};
            body->velocity = Vector2{.x = 0, .y = 0};
            SetPhysicsBodyRotation(body, 0);
        }

        // Horizontal movement input
        if (IsKeyDown(KEY_RIGHT))
            body->velocity.x = VELOCITY;
        else if (IsKeyDown(KEY_LEFT))
            body->velocity.x = -VELOCITY;

        // Vertical movement input checking if player physics body is grounded
        if (IsKeyDown(KEY_UP) && body->isGrounded)
            body->velocity.y = -VELOCITY * 4;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        const int texture_x = SCREEN_WIDTH / 2 - texture.width / 2;
        const int texture_y = SCREEN_HEIGHT / 2 - texture.height / 2;
        DrawTexture(texture, texture_x, texture_y, WHITE);

        DrawFPS(SCREEN_WIDTH - 90, SCREEN_HEIGHT - 30);

        // Draw created physics bodies
        int bodiesCount = GetPhysicsBodiesCount();
        for (int i = 0; i < bodiesCount; i++)
        {
            PhysicsBody body = GetPhysicsBody(i);

            int vertexCount = GetPhysicsShapeVerticesCount(i);
            for (int j = 0; j < vertexCount; j++)
            {
                // Get physics bodies shape vertices to draw lines
                // Note: GetPhysicsShapeVertex() already calculates rotation transformations
                Vector2 vertexA = GetPhysicsShapeVertex(body, j);

                int jj = (((j + 1) < vertexCount) ? (j + 1) : 0); // Get next vertex or first to close the shape
                Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

                DrawLineV(vertexA, vertexB, GREEN); // Draw a line between two vertex positions
            }
        }

        DrawText("Use 'ARROWS' to move player", 10, 10, 10, WHITE);
        DrawText("Press 'R' to reset example", 10, 30, 10, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    ClosePhysics(); // Unitialize physics

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

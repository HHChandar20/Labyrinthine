#include "raylib.h"
#include "src/DAL/Cell.h"
#include "src/DAL/Ball.h"

int main() 
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Mazeon");
    SetTargetFPS(60);

    // Test Cell struct
    Cell testCell;
    testCell.x = 5;
    testCell.y = 10;

    // Test Ball struct
    Ball testBall(100.0f, 100.0f, 0, 0, 12.0f);

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawText("Mazeon - Maze Game", 20, 20, 20, WHITE);
        DrawText(TextFormat("Test Cell: (%d, %d)", testCell.x, testCell.y), 20, 50, 20, WHITE);
        DrawText(TextFormat("Cell has walls: %s", testCell.walls[0] ? "Yes" : "No"), 20, 80, 20, WHITE);
        
        DrawText(TextFormat("Ball Position: (%.1f, %.1f)", testBall.x, testBall.y), 20, 110, 20, BLUE);
        DrawText(TextFormat("Ball Grid: (%d, %d)", testBall.cellX, testBall.cellY), 20, 140, 20, BLUE);
        
        // Draw the ball
        DrawCircle(testBall.x, testBall.y, testBall.radius, SKYBLUE);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
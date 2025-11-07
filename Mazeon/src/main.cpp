#include "raylib.h"
#include "src/DAL/Cell.h"

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

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawText("Mazeon - Maze Game", 20, 20, 20, WHITE);
        DrawText(TextFormat("Test Cell: (%d, %d)", testCell.x, testCell.y), 20, 50, 20, WHITE);
        DrawText(TextFormat("Cell has walls: %s", testCell.walls[0] ? "Yes" : "No"), 20, 80, 20, WHITE);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
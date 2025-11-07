#include "raylib.h"

int main() 
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Mazeon");
    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawText("Mazeon - Maze Game", 20, 20, 20, WHITE);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
#include "raylib.h"
#include "src/DAL/Cell.h"
#include "src/DAL/Ball.h"
#include "src/DAL/PathNode.h"
#include "src/DAL/Teleport.h"
#include "src/BLL/MazeGenerator.h"
#include <vector>

int main() 
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Mazeon");
    SetTargetFPS(60);

    // Test MazeGenerator with a small maze
    std::vector<std::vector<Cell>> testMaze;
    int mazeWidth = 15;
    int mazeHeight = 10;
    int cellSize = 50;
    
    MazeGenerator::Generate(testMaze, mazeWidth, mazeHeight);

    // Calculate offset to center the maze
    float offsetX = (screenWidth - mazeWidth * cellSize) / 2.0f;
    float offsetY = (screenHeight - mazeHeight * cellSize) / 2.0f;

    while (!WindowShouldClose()) 
    {
        // Regenerate maze on SPACE press
        if (IsKeyPressed(KEY_SPACE)) 
        {
            MazeGenerator::Generate(testMaze, mazeWidth, mazeHeight);
        }

        BeginDrawing();
        ClearBackground(Color{15, 15, 25, 255});
        
        DrawText("Mazeon - Maze Generation Complete!", 20, 20, 28, WHITE);
        DrawText("Press SPACE to regenerate maze", 20, 55, 20, GRAY);
        DrawText(TextFormat("Maze size: %dx%d", mazeWidth, mazeHeight), 20, 85, 20, GRAY);
        
        // Draw maze
        for (int y = 0; y < mazeHeight; y++) 
        {
            for (int x = 0; x < mazeWidth; x++) 
            {
                const Cell& cell = testMaze[y][x];
                float cellX = offsetX + x * cellSize;
                float cellY = offsetY + y * cellSize;
                float wallThickness = 4.0f;

                Color wallColor = Color{60, 70, 90, 255};

                // Draw walls
                if (cell.walls[0]) // top
                {
                    DrawRectangle(cellX, cellY - wallThickness / 2, 
                                cellSize, wallThickness, wallColor);
                }
                if (cell.walls[1]) // right
                {
                    DrawRectangle(cellX + cellSize - wallThickness / 2, cellY, 
                                wallThickness, cellSize, wallColor);
                }
                if (cell.walls[2]) // bottom
                {
                    DrawRectangle(cellX, cellY + cellSize - wallThickness / 2, 
                                cellSize, wallThickness, wallColor);
                }
                if (cell.walls[3]) // left
                {
                    DrawRectangle(cellX - wallThickness / 2, cellY, 
                                wallThickness, cellSize, wallColor);
                }
            }
        }
        
        // Highlight start and end
        float startX = offsetX + cellSize / 2;
        float startY = offsetY + cellSize / 2;
        DrawCircle(startX, startY, 12, GREEN);
        
        float endX = offsetX + (mazeWidth - 1) * cellSize + cellSize / 2;
        float endY = offsetY + (mazeHeight - 1) * cellSize + cellSize / 2;
        DrawCircle(endX, endY, 12, RED);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
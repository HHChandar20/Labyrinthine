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

    // Test MazeGenerator with teleports
    std::vector<std::vector<Cell>> testMaze;
    std::vector<Teleport> testTeleports;
    int mazeWidth = 25;
    int mazeHeight = 17;
    int cellSize = 35;
    int numTeleports = 3;
    
    double startTime = GetTime();
    MazeGenerator::Generate(testMaze, mazeWidth, mazeHeight);
    MazeGenerator::GenerateTeleports(testMaze, testTeleports, 
                                     mazeWidth, mazeHeight, 
                                     mazeWidth - 1, mazeHeight - 1, 
                                     numTeleports);
    double endTime = GetTime();
    double generationTime = (endTime - startTime) * 1000.0; // Convert to ms

    // Calculate offset to center the maze
    float offsetX = (screenWidth - mazeWidth * cellSize) / 2.0f;
    float offsetY = (screenHeight - mazeHeight * cellSize) / 2.0f;

    // Teleport colors
    Color teleportColors[] = {
        {200, 100, 255, 255}, // Purple
        {255, 150, 50, 255},  // Orange
        {50, 255, 150, 255},  // Cyan
        {255, 100, 150, 255}, // Pink
        {100, 150, 255, 255}  // Light Blue
    };

    while (!WindowShouldClose()) 
    {
        // Regenerate maze on SPACE press
        if (IsKeyPressed(KEY_SPACE)) 
        {
            startTime = GetTime();
            MazeGenerator::Generate(testMaze, mazeWidth, mazeHeight);
            MazeGenerator::GenerateTeleports(testMaze, testTeleports, 
                                           mazeWidth, mazeHeight, 
                                           mazeWidth - 1, mazeHeight - 1, 
                                           numTeleports);
            endTime = GetTime();
            generationTime = (endTime - startTime) * 1000.0;
        }

        BeginDrawing();
        ClearBackground(Color{15, 15, 25, 255});
        
        DrawText("Mazeon - Optimized Generation", 20, 20, 28, WHITE);
        DrawText("Press SPACE to regenerate", 20, 55, 20, GRAY);
        DrawText(TextFormat("Generation time: %.2f ms", generationTime), 20, 85, 20, YELLOW);
        DrawText(TextFormat("Teleports: %d", (int)testTeleports.size()), 20, 115, 20, PURPLE);
        
        // Draw maze
        for (int y = 0; y < mazeHeight; y++) 
        {
            for (int x = 0; x < mazeWidth; x++) 
            {
                const Cell& cell = testMaze[y][x];
                float cellX = offsetX + x * cellSize;
                float cellY = offsetY + y * cellSize;
                float wallThickness = 3.0f;

                Color wallColor = Color{60, 70, 90, 255};

                // Draw walls
                if (cell.walls[0])
                {
                    DrawRectangle(cellX, cellY - wallThickness / 2, 
                                cellSize, wallThickness, wallColor);
                }
                if (cell.walls[1])
                {
                    DrawRectangle(cellX + cellSize - wallThickness / 2, cellY, 
                                wallThickness, cellSize, wallColor);
                }
                if (cell.walls[2])
                {
                    DrawRectangle(cellX, cellY + cellSize - wallThickness / 2, 
                                cellSize, wallThickness, wallColor);
                }
                if (cell.walls[3])
                {
                    DrawRectangle(cellX - wallThickness / 2, cellY, 
                                wallThickness, cellSize, wallColor);
                }
            }
        }
        
        // Draw teleports
        for (const Teleport& tp : testTeleports) 
        {
            float x1 = offsetX + tp.x1 * cellSize + cellSize / 2.0f;
            float y1 = offsetY + tp.y1 * cellSize + cellSize / 2.0f;
            float x2 = offsetX + tp.x2 * cellSize + cellSize / 2.0f;
            float y2 = offsetY + tp.y2 * cellSize + cellSize / 2.0f;

            Color color = teleportColors[tp.id % 5];

            DrawCircle(x1, y1, 12.0f, color);
            DrawCircle(x1, y1, 6.0f, Fade(WHITE, 0.7f));

            DrawCircle(x2, y2, 12.0f, color);
            DrawCircle(x2, y2, 6.0f, Fade(WHITE, 0.7f));
        }
        
        // Highlight start and end
        float startX = offsetX + cellSize / 2;
        float startY = offsetY + cellSize / 2;
        DrawCircle(startX, startY, 10, GREEN);
        
        float endX = offsetX + (mazeWidth - 1) * cellSize + cellSize / 2;
        float endY = offsetY + (mazeHeight - 1) * cellSize + cellSize / 2;
        DrawCircle(endX, endY, 10, RED);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
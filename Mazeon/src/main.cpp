#include "raylib.h"
#include "src/DAL/Cell.h"
#include "src/DAL/Ball.h"
#include "src/DAL/PathNode.h"
#include "src/DAL/Teleport.h"
#include <vector>

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

    // Test PathNode struct
    std::vector<PathNode> testPath;
    testPath.push_back(PathNode(0, 0, 1));  // Start, moving right
    testPath.push_back(PathNode(1, 0, 1));  // Continue right
    testPath.push_back(PathNode(2, 0, 2));  // Turn down

    // Test Teleport struct
    std::vector<Teleport> testTeleports;
    testTeleports.push_back(Teleport(3, 3, 10, 8, 0));   // Purple teleport
    testTeleports.push_back(Teleport(5, 2, 15, 12, 1));  // Orange teleport

    const char* directions[] = {"UP", "RIGHT", "DOWN", "LEFT"};

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
        
        // Display path nodes
        DrawText("Test Path:", 20, 170, 20, GREEN);
        for (size_t i = 0; i < testPath.size(); i++) 
        {
            DrawText(TextFormat("Node %d: (%d, %d) -> %s", 
                     i, testPath[i].x, testPath[i].y, 
                     directions[testPath[i].direction]), 
                     20, 200 + i * 25, 18, GREEN);
        }
        
        // Display teleports
        DrawText("Test Teleports:", 20, 280, 20, PURPLE);
        for (size_t i = 0; i < testTeleports.size(); i++) 
        {
            DrawText(TextFormat("Teleport %d: (%d, %d) <-> (%d, %d)", 
                     testTeleports[i].id,
                     testTeleports[i].x1, testTeleports[i].y1,
                     testTeleports[i].x2, testTeleports[i].y2), 
                     20, 310 + i * 25, 18, PURPLE);
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
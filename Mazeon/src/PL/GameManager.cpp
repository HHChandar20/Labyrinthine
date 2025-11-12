#include "GameManager.h"

GameManager::GameManager() 
{
    InitWindow(screenWidth, screenHeight, "Mazeon");
    SetTargetFPS(60);

    // Initialize colors
    bgColor = { 15, 15, 25, 255 };
    wallColor = { 60, 70, 90, 255 };
    ballColor = { 120, 200, 255, 255 };
    glowColor = { 150, 220, 255, 100 };
    goalColor = { 100, 255, 120, 255 };

    game.Initialize();
}

GameManager::~GameManager() 
{
    CloseWindow();
}

void GameManager::Run() 
{
    while (!WindowShouldClose()) 
    {
        Update();
        Draw();
    }
}

void GameManager::Update() 
{
    // TODO: Add game update logic
}

void GameManager::Draw() 
{
    BeginDrawing();
    ClearBackground(bgColor);

    DrawMaze();
    DrawGoal();
    DrawBall();

    // Draw UI
    DrawText(TextFormat("Level %d", game.GetCurrentLevel() + 1), 20, 40, 28, WHITE);

    EndDrawing();
}

void GameManager::DrawMaze() 
{
    const auto& maze = game.GetMaze();
    int mazeWidth = game.GetMazeWidth();
    int mazeHeight = game.GetMazeHeight();
    int cellSize = game.GetCellSize();
    float offsetX = game.GetOffsetX();
    float offsetY = game.GetOffsetY();

    for (int y = 0; y < mazeHeight; y++) 
    {
        for (int x = 0; x < mazeWidth; x++) 
        {
            const Cell& cell = maze[y][x];
            float cellX = offsetX + x * cellSize;
            float cellY = offsetY + y * cellSize;

            float wallThickness = 4.0f;

            if (cell.walls[0]) 
            {
                DrawRectangle(cellX, cellY - wallThickness / 2, cellSize, wallThickness, wallColor);
            }

            if (cell.walls[1])
            {
                DrawRectangle(cellX + cellSize - wallThickness / 2, cellY, wallThickness, cellSize, wallColor);
            }
            
            if (cell.walls[2])
            {
                DrawRectangle(cellX, cellY + cellSize - wallThickness / 2, cellSize, wallThickness, wallColor);
            }
            
            if (cell.walls[3]) 
            {
                DrawRectangle(cellX - wallThickness / 2, cellY, wallThickness, cellSize, wallColor);
            }
        }
    }
}

void GameManager::DrawGoal() 
{
    int cellSize = game.GetCellSize();
    float offsetX = game.GetOffsetX();
    float offsetY = game.GetOffsetY();
    int goalX = game.GetGoalX();
    int goalY = game.GetGoalY();

    float goalCenterX = offsetX + goalX * cellSize + cellSize / 2.0f;
    float goalCenterY = offsetY + goalY * cellSize + cellSize / 2.0f;

    DrawCircle(goalCenterX, goalCenterY, 14.0f, goalColor);
}

void GameManager::DrawBall() 
{
    const Ball& ball = game.GetBall();

    DrawCircle(ball.x, ball.y, ball.radius + 6.0f, glowColor);
    DrawCircle(ball.x, ball.y, ball.radius, ballColor);
    DrawCircle(ball.x, ball.y, ball.radius - 4.0f, Fade(WHITE, 0.6f));
}
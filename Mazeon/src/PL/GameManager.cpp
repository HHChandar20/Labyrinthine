#include "GameManager.h"
#include <cmath>

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
    game.HandleInput();
    game.Update();
}

void GameManager::Draw() 
{
    BeginDrawing();
    ClearBackground(bgColor);

    DrawMaze();
    DrawGoal();
    DrawBall();

    // Draw UI
    int currentLevel = game.GetCurrentLevel();
    DrawText(TextFormat("Level %d", currentLevel + 1), 20, 40, 28, WHITE);

    // Draw timer
    if (game.IsTimedMode()) 
    {
        float timeRemaining = game.GetTimeRemaining();
        Color timerColor = timeRemaining > 10.0f ? WHITE : RED;
        DrawText(TextFormat("Time: %.1f", timeRemaining), 20, 75, 24, timerColor);
    }

    // Draw moves counter
    if (game.IsLimitedMovesMode()) 
    {
        int movesMade = game.GetMovesMade();
        int movesRemaining = game.GetMovesRemaining();
        Color movesColor = movesRemaining - movesMade > 5 ? WHITE : RED;
        int yPos = game.IsTimedMode() ? 85 : 55;
        
        DrawText(TextFormat("Moves: %d/%d", movesMade, movesRemaining), 20, yPos + 20, 24, movesColor);
    }

    // Draw level complete screen
    if (game.IsLevelComplete()) 
    {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));

        const char* text = "Level Complete!";
        int textWidth = MeasureText(text, 50);
        DrawText(text, screenWidth / 2 - textWidth / 2, screenHeight / 2 - 50, 50, ballColor);

        const char* continueText = "Press SPACE or Click for Next Level";
        int continueWidth = MeasureText(continueText, 24);
        DrawText(continueText, screenWidth / 2 - continueWidth / 2, screenHeight / 2 + 20, 24, WHITE);
    }

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
    float pulse = (sin(GetTime() * 3.0f) + 1.0f) * 0.5f;

    DrawCircle(goalCenterX, goalCenterY, 18.0f + pulse * 4.0f, Fade(goalColor, 0.3f));
    DrawCircle(goalCenterX, goalCenterY, 14.0f, goalColor);
    DrawCircle(goalCenterX, goalCenterY, 8.0f, Fade(goalColor, 0.5f));
}

void GameManager::DrawBall() 
{
    const Ball& ball = game.GetBall();

    DrawCircle(ball.x, ball.y, ball.radius + 6.0f, glowColor);
    DrawCircle(ball.x, ball.y, ball.radius, ballColor);
    DrawCircle(ball.x, ball.y, ball.radius - 4.0f, Fade(WHITE, 0.6f));
}
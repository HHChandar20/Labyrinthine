#include "GameManager.h"
#include <cmath>

GameManager::GameManager() 
{
    InitWindow(Game::GetScreenWidth(), Game::GetScreenHeight(), "Mazeon");
    SetTargetFPS(60);

    // Initialize colors
    bgColor = { 15, 15, 25, 255 };
    wallColor = { 60, 70, 90, 255 };
    ballColor = { 120, 200, 255, 255 };
    glowColor = { 150, 220, 255, 100 };
    goalColor = { 100, 255, 120, 255 };
    dotColor = { 255, 200, 100, 255 };

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
    DrawDirectionDots();
    DrawBall();
    DrawUI();

    if (game.IsLevelComplete()) 
    {
        DrawLevelCompleteScreen();
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
    
    // Smooth pulsing animation with sine wave
    float pulse = (sin(GetTime() * 3.0f) + 1.0f) * 0.5f;

    // Outer glow with pulse
    DrawCircle(goalCenterX, goalCenterY, 18.0f + pulse * 4.0f, Fade(goalColor, 0.3f));
    // Middle ring
    DrawCircle(goalCenterX, goalCenterY, 14.0f, goalColor);
    // Inner highlight with subtle pulse
    DrawCircle(goalCenterX, goalCenterY, 8.0f + pulse * 2.0f, Fade(goalColor, 0.5f));
}

void GameManager::DrawDirectionDots() 
{
    // Don't draw dots while ball is moving or level is complete
    if (game.IsBallMoving() || game.IsLevelComplete()) return;

    const Ball& ball = game.GetBall();
    const auto& availableDirections = game.GetAvailableDirections();
    int cellSize = game.GetCellSize();

    Vector2 mousePos = GetMousePosition();

    for (int dir : availableDirections) 
    {
        float dotX = ball.x;
        float dotY = ball.y;

        if (dir == 0) dotY -= cellSize;      // up
        else if (dir == 1) dotX += cellSize; // right
        else if (dir == 2) dotY += cellSize; // down
        else if (dir == 3) dotX -= cellSize; // left

        // Faster pulsing for interactive feel
        float dotPulse = (sin(GetTime() * 4.0f) + 1.0f) * 0.5f;
        
        // Check if mouse is hovering over this dot
        float dist = sqrt(pow(mousePos.x - dotX, 2) + pow(mousePos.y - dotY, 2));
        bool isHovered = dist < 25.0f;
        
        // Draw larger glow when hovered
        if (isHovered) 
        {
            // Enhanced pulse when hovered
            DrawCircle(dotX, dotY, 15.0f + dotPulse * 5.0f, Fade(dotColor, 0.6f));
            DrawCircle(dotX, dotY, 12.0f, dotColor);
        }
        else 
        {
            // Standard pulsing animation
            DrawCircle(dotX, dotY, 10.0f + dotPulse * 3.0f, Fade(dotColor, 0.4f));
            DrawCircle(dotX, dotY, 8.0f, dotColor);
        }
    }
}

void GameManager::DrawBall() 
{
    const Ball& ball = game.GetBall();

    DrawCircle(ball.x, ball.y, ball.radius + 6.0f, glowColor);
    DrawCircle(ball.x, ball.y, ball.radius, ballColor);
    DrawCircle(ball.x, ball.y, ball.radius - 4.0f, Fade(WHITE, 0.6f));
}

void GameManager::DrawUI() 
{
    int currentLevel = game.GetCurrentLevel();
    DrawText(TextFormat("Level %d", currentLevel + 1), 20, 40, 28, WHITE);

    // Draw timer with enhanced visuals
    if (game.IsTimedMode()) 
    {
        float timeRemaining = game.GetTimeRemaining();
        
        // Color changes based on remaining time
        Color timerColor;
        const char* timerPrefix = "Time: ";
        
        if (timeRemaining > 20.0f) 
        {
            timerColor = WHITE;
        }
        else if (timeRemaining > 10.0f) 
        {
            timerColor = YELLOW;
            timerPrefix = "TIME: ";
        }
        else 
        {
            // Pulse red when time is critical
            float pulse = (sin(GetTime() * 8.0f) + 1.0f) * 0.5f;
            timerColor = Fade(RED, 0.7f + pulse * 0.3f);
            timerPrefix = "TIME!! ";
            
            // Add warning text
            if (timeRemaining <= 5.0f) 
            {
                int screenWidth = Game::GetScreenWidth();
                const char* warningText = "HURRY!";
                int warningWidth = MeasureText(warningText, 40);
                DrawText(warningText, screenWidth / 2 - warningWidth / 2, 100, 40, Fade(RED, 0.5f + pulse * 0.5f));
            }
        }
        
        DrawText(TextFormat("%s%.1f", timerPrefix, timeRemaining), 20, 75, 24, timerColor);
        
        // Draw progress bar
        float barWidth = 200.0f;
        float barHeight = 8.0f;
        float barX = 20.0f;
        float barY = 105.0f;
        
        // Background
        DrawRectangle(barX, barY, barWidth, barHeight, Fade(DARKGRAY, 0.5f));
        
        // Progress (based on time limit)
        float timeLimit = 65.0f - ((currentLevel - 4) * 5.0f);
        if (currentLevel >= 9 && currentLevel < 14) 
        {
            timeLimit = 50.0f - ((currentLevel - 9) * 3.0f);
        }
        else if (currentLevel >= 20) 
        {
            timeLimit = 40.0f;
        }
        
        float progress = timeRemaining / timeLimit;
        if (progress < 0.0f) progress = 0.0f;
        
        Color barColor = progress > 0.3f ? GREEN : RED;
        DrawRectangle(barX, barY, barWidth * progress, barHeight, barColor);
    }

    // Draw moves counter
    if (game.IsLimitedMovesMode()) 
    {
        int movesMade = game.GetMovesMade();
        int movesRemaining = game.GetMovesRemaining();
        Color movesColor = movesRemaining - movesMade > 5 ? WHITE : RED;
        int yPos = game.IsTimedMode() ? 120 : 75;
        
        DrawText(TextFormat("Moves: %d/%d", movesMade, movesRemaining), 20, yPos, 24, movesColor);
    }

    // Draw instructions
    if (!game.IsBallMoving() && !game.IsLevelComplete()) 
    {
        int screenHeight = Game::GetScreenHeight();
        DrawText("WASD or Arrow Keys to move", 20, screenHeight - 80, 20, Fade(WHITE, 0.7f));
        DrawText("Or click on the direction dots", 20, screenHeight - 55, 18, Fade(GRAY, 0.7f));
    }
    else if (game.IsBallMoving()) 
    {
        int screenHeight = Game::GetScreenHeight();
        DrawText("Moving...", 20, screenHeight - 60, 20, Fade(YELLOW, 0.7f));
    }
}


void GameManager::DrawLevelCompleteScreen() 
{
    int screenWidth = Game::GetScreenWidth();
    int screenHeight = Game::GetScreenHeight();

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));

    const char* text = "Level Complete!";
    int textWidth = MeasureText(text, 50);
    DrawText(text, screenWidth / 2 - textWidth / 2, screenHeight / 2 - 50, 50, ballColor);

    const char* continueText = "Press SPACE or Click for Next Level";
    int continueWidth = MeasureText(continueText, 24);
    DrawText(continueText, screenWidth / 2 - continueWidth / 2, screenHeight / 2 + 20, 24, WHITE);
}
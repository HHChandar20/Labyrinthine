#include "GameManager.h"
#include <cmath>
#include <string>

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
    dotColor = { 255, 200, 100, 255 };

    teleportColors = 
    {
        { 200, 100, 255, 255 }, // Purple
        { 255, 150, 50, 255 },  // Orange
        { 50, 255, 150, 255 },  // Cyan
        { 255, 100, 150, 255 }, // Pink
        { 100, 150, 255, 255 }, // Light Blue
        { 255, 220, 50, 255 },  // Yellow
        { 255, 50, 80, 255 },   // Red
        { 150, 255, 100, 255 }, // Lime Green
        { 150, 100, 200, 255 }, // Deep Purple
        { 255, 120, 200, 255 }, // Hot Pink
        { 50, 200, 255, 255 },  // Sky Blue
        { 255, 180, 120, 255 }, // Peach
        { 100, 255, 255, 255 }, // Aqua
        { 200, 150, 255, 255 }, // Lavender
        { 255, 200, 100, 255 }  // Gold
    };

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
    DrawTeleports();
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
    bool fogMode = game.IsFogMode();
    const auto& exploredCells = game.GetExploredCells();
    const Ball& ball = game.GetBall();
    float currentFogRadius = game.GetCurrentFogRadius();

    // First draw the entire maze (walls for explored areas)
    for (int y = 0; y < mazeHeight; y++)
    {
        for (int x = 0; x < mazeWidth; x++)
        {
            bool isExplored = true;
            bool isCurrentlyVisible = true;

            if (fogMode)
            {
                // Only draw if explored OR currently visible around ball
                bool isExplored = exploredCells[y][x];
                bool isCurrentlyVisible = false;

                // Check if this cell is currently within visibility radius of ball
                float dist = sqrt(pow(x - ball.cellX, 2) + pow(y - ball.cellY, 2));
                isCurrentlyVisible = (dist <= currentFogRadius);
            }

            if (!fogMode || isExplored || isCurrentlyVisible)
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

    // Then draw fog of war overlay for unexplored areas that are not currently visible
    if (fogMode)
    {
        for (int y = 0; y < mazeHeight; y++)
        {
            for (int x = 0; x < mazeWidth; x++)
            {
                if (!exploredCells[y][x])
                {
                    // Check if this cell is currently within visibility radius of ball
                    float dist = sqrt(pow(x - ball.cellX, 2) + pow(y - ball.cellY, 2));
                    bool isCurrentlyVisible = (dist <= currentFogRadius);

                    if (!isCurrentlyVisible)
                    {
                        float cellX = offsetX + x * cellSize;
                        float cellY = offsetY + y * cellSize;
                        DrawRectangle(cellX, cellY, cellSize, cellSize, Fade(BLACK, 1.0f));
                    }
                }
            }
        }
    }
}
void GameManager::DrawTeleports()
{
    if (!game.IsTeleportMode()) return;

    const auto& teleports = game.GetTeleports();
    int cellSize = game.GetCellSize();
    float offsetX = game.GetOffsetX();
    float offsetY = game.GetOffsetY();
    bool fogMode = game.IsFogMode();
    const auto& exploredCells = game.GetExploredCells();
    const Ball& ball = game.GetBall();
    float currentFogRadius = game.GetCurrentFogRadius();

    for (const Teleport& tp : teleports)
    {
        // Check if teleport ends are explored OR currently visible
        bool tp1Visible = false;
        bool tp2Visible = false;

        if (fogMode)
        {
            // Check if teleport positions are explored
            bool tp1Explored = exploredCells[tp.y1][tp.x1];
            bool tp2Explored = exploredCells[tp.y2][tp.x2];

            // Check if teleport positions are currently visible
            float dist1 = sqrt(pow(tp.x1 - ball.cellX, 2) + pow(tp.y1 - ball.cellY, 2));
            float dist2 = sqrt(pow(tp.x2 - ball.cellX, 2) + pow(tp.y2 - ball.cellY, 2));
            bool tp1Current = (dist1 <= currentFogRadius);
            bool tp2Current = (dist2 <= currentFogRadius);

            tp1Visible = tp1Explored || tp1Current;
            tp2Visible = tp2Explored || tp2Current;
        }
        else
        {
            tp1Visible = tp2Visible = true;
        }

        // Only draw if both teleport ends are visible in some way
        if (fogMode && (!tp1Visible || !tp2Visible))
        {
            continue;
        }

        float pulse = (sin(GetTime() * 4.0f) + 1.0f) * 0.5f;

        float x1 = offsetX + tp.x1 * cellSize + cellSize / 2.0f;
        float y1 = offsetY + tp.y1 * cellSize + cellSize / 2.0f;
        float x2 = offsetX + tp.x2 * cellSize + cellSize / 2.0f;
        float y2 = offsetY + tp.y2 * cellSize + cellSize / 2.0f;

        Color color = teleportColors[tp.id % teleportColors.size()];

        DrawCircle(x1, y1, 16.0f + pulse * 3.0f, Fade(color, 0.3f));
        DrawCircle(x1, y1, 12.0f, color);
        DrawCircle(x1, y1, 6.0f, Fade(WHITE, 0.7f));

        DrawCircle(x2, y2, 16.0f + pulse * 3.0f, Fade(color, 0.3f));
        DrawCircle(x2, y2, 12.0f, color);
        DrawCircle(x2, y2, 6.0f, Fade(WHITE, 0.7f));
    }
}

void GameManager::DrawGoal()
{
    int cellSize = game.GetCellSize();
    float offsetX = game.GetOffsetX();
    float offsetY = game.GetOffsetY();
    int goalX = game.GetGoalX();
    int goalY = game.GetGoalY();
    bool fogMode = game.IsFogMode();
    const auto& exploredCells = game.GetExploredCells();
    const Ball& ball = game.GetBall();
    float currentFogRadius = game.GetCurrentFogRadius();

    // Check if goal is explored OR currently visible
    bool goalVisible = true;

    if (fogMode)
    {
        bool goalExplored = exploredCells[goalY][goalX];
        float dist = sqrt(pow(goalX - ball.cellX, 2) + pow(goalY - ball.cellY, 2));
        bool goalCurrent = (dist <= currentFogRadius);
        goalVisible = goalExplored || goalCurrent;
    }

    // Only draw goal if visible in some way
    if (fogMode && !goalVisible)
    {
        return;
    }

    float goalCenterX = offsetX + goalX * cellSize + cellSize / 2.0f;
    float goalCenterY = offsetY + goalY * cellSize + cellSize / 2.0f;
    float pulse = (sin(GetTime() * 3.0f) + 1.0f) * 0.5f;

    DrawCircle(goalCenterX, goalCenterY, 18.0f + pulse * 4.0f, Fade(goalColor, 0.3f));
    DrawCircle(goalCenterX, goalCenterY, 14.0f, goalColor);
    DrawCircle(goalCenterX, goalCenterY, 8.0f, Fade(goalColor, 0.5f));
}

void GameManager::DrawDirectionDots() 
{
    if (game.IsBallMoving() || game.IsLevelComplete()) return;

    const Ball& ball = game.GetBall();
    const auto& availableDirections = game.GetAvailableDirections();
    int cellSize = game.GetCellSize();

    bool canMove = !game.IsLimitedMovesMode() || game.GetMovesMade() < game.GetMovesRemaining();

    for (int dir : availableDirections) 
    {
        float dotX = ball.x;
        float dotY = ball.y;

        if (dir == 0) dotY -= cellSize;
        else if (dir == 1) dotX += cellSize;
        else if (dir == 2) dotY += cellSize;
        else if (dir == 3) dotX -= cellSize;

        float dotPulse = (sin(GetTime() * 4.0f) + 1.0f) * 0.5f;
        Color drawColor = canMove ? dotColor : Fade(dotColor, 0.3f);
    
        DrawCircle(dotX, dotY, 10.0f + dotPulse * 3.0f, Fade(drawColor, 0.4f));
        DrawCircle(dotX, dotY, 8.0f, drawColor);
    }
}

void GameManager::DrawBall() 
{
    const Ball& ball = game.GetBall();
    bool fogMode = game.IsFogMode();
    float currentFogRadius = game.GetCurrentFogRadius();

    // Draw a subtle pulsing glow around the ball when in fog mode
    if (fogMode)
    {
        // Calculate glow size based on fog radius (convert from cells to pixels)
        float glowSize = currentFogRadius * game.GetCellSize() * 0.8f;
        DrawCircle(ball.x, ball.y, glowSize, Fade(glowColor, 0.1f));
    }

    DrawCircle(ball.x, ball.y, ball.radius + 6.0f, glowColor);
    DrawCircle(ball.x, ball.y, ball.radius, ballColor);
    DrawCircle(ball.x, ball.y, ball.radius - 4.0f, Fade(WHITE, 0.6f));
}

void GameManager::DrawUI() 
{
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

    if (!game.IsBallMoving() && !game.IsLevelComplete()) 
    {
        std::string instructionText = "WASD or click dots";
    
        if (game.IsLimitedMovesMode() && game.GetMovesMade() >= game.GetMovesRemaining()) 
        {
            instructionText = "Out of moves! Press R to restart";
        }

        DrawText(instructionText.c_str(), 20, screenHeight - 80, 20, Fade(WHITE, 0.7f));
    }
}

void GameManager::DrawLevelCompleteScreen() 
{
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));

    const char* text = "Level Complete!";
    int textWidth = MeasureText(text, 50);
    DrawText(text, screenWidth / 2 - textWidth / 2, screenHeight / 2 - 50, 50, ballColor);

    const char* continueText = "Press SPACE or Click for Next Level";
    int continueWidth = MeasureText(continueText, 24);
    DrawText(continueText, screenWidth / 2 - continueWidth / 2, screenHeight / 2 + 20, 24, WHITE);
}
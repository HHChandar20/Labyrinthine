#include "Game.h"
#include "../BLL/MazeGenerator.h"
#include "../DAL/SaveManager.h"

Game::Game()
    : currentLevel(0), levelComplete(false),
    timedMode(false), teleportMode(false), limitedMovesMode(false),
    timeLimit(0.0f), timeRemaining(0.0f), movesRemaining(0), movesMade(0),
    mazeWidth(0), mazeHeight(0), offsetX(0), offsetY(0),
    goalX(0), goalY(0), ballController(nullptr),
    fogMode(false) {}

Game::~Game() 
{
    if (ballController) 
    {
        delete ballController;
    }
}

void Game::Initialize() 
{
    currentLevel = SaveManager::LoadProgress();
    GenerateLevel();
}

float Game::GetCurrentFogRadius() const
{
    if (ballController && fogMode)
    {
        return ballController->GetCurrentFogRadius();
    }
    return 0.0f;
}

void Game::ConfigureLevelDifficulty()
{
    // Reset modes
    timedMode = false;
    teleportMode = false;
    limitedMovesMode = false;
    fogMode = false;
    teleports.clear();

    // Level 1-3: Normal maze (increasing size)
    if (currentLevel >= 0 && currentLevel < 3)
    {
        mazeWidth = 9 + currentLevel * 2;
        mazeHeight = 7 + currentLevel * 2;
    }
    // Level 4-6: Timer only
    else if (currentLevel >= 3 && currentLevel < 6)
    {
        mazeWidth = 15;
        mazeHeight = 11;
        timedMode = true;
        timeLimit = 60.0f - (currentLevel - 3) * 10.0f;
        timeRemaining = timeLimit;
    }
    // Level 7-9: Limited moves only
    else if (currentLevel >= 6 && currentLevel < 9)
    {
        mazeWidth = 17;
        mazeHeight = 13;
        limitedMovesMode = true;
        movesRemaining = 25 - (currentLevel - 6) * 3;
        movesMade = 0;
    }
    // Level 10-12: Timer + Limited moves
    else if (currentLevel >= 9 && currentLevel < 12)
    {
        mazeWidth = 19;
        mazeHeight = 15;
        timedMode = true;
        limitedMovesMode = true;
        timeLimit = 50.0f - (currentLevel - 9) * 5.0f;
        timeRemaining = timeLimit;
        movesRemaining = 22 - (currentLevel - 9) * 2;
        movesMade = 0;
    }
    // Level 13-15: Fog mode only
    else if (currentLevel >= 12 && currentLevel < 15)
    {
        mazeWidth = 21;
        mazeHeight = 17;
        fogMode = true;
    }
    // Level 16-18: Fog mode + Timer
    else if (currentLevel >= 15 && currentLevel < 18)
    {
        mazeWidth = 23;
        mazeHeight = 17;
        fogMode = true;
        timedMode = true;
        timeLimit = 45.0f - (currentLevel - 15) * 5.0f;
        timeRemaining = timeLimit;
    }
    // Level 19-21: Teleport only
    else if (currentLevel >= 18 && currentLevel < 21)
    {
        mazeWidth = 25;
        mazeHeight = 19;
        teleportMode = true;
    }
    // Level 22-24: Teleport + Timer
    else if (currentLevel >= 21 && currentLevel < 24)
    {
        mazeWidth = 25;
        mazeHeight = 19;
        teleportMode = true;
        timedMode = true;
        timeLimit = 60.0f - (currentLevel - 21) * 5.0f;
        timeRemaining = timeLimit;
    }
    // Level 25-27: Teleport + Limited moves
    else if (currentLevel >= 24 && currentLevel < 27)
    {
        mazeWidth = 27;
        mazeHeight = 19;
        teleportMode = true;
        limitedMovesMode = true;
        movesRemaining = 20 - (currentLevel - 24) * 2;
        movesMade = 0;
    }
    // Level 28-30: Teleport + Timer + Limited moves
    else if (currentLevel >= 27 && currentLevel < 30)
    {
        mazeWidth = 27;
        mazeHeight = 19;
        teleportMode = true;
        timedMode = true;
        limitedMovesMode = true;
        timeLimit = 35.0f - (currentLevel - 27) * 3.0f;
        timeRemaining = timeLimit;
        movesRemaining = 18 - (currentLevel - 27);
        movesMade = 0;
    }
    // Level 31-35: All modes combined
    else if (currentLevel >= 30 && currentLevel < 35)
    {
        mazeWidth = 29;
        mazeHeight = 21;
        teleportMode = true;
        timedMode = true;
        limitedMovesMode = true;
        fogMode = true;
        timeLimit = 30.0f - (currentLevel - 30) * 2.0f;
        timeRemaining = timeLimit;
        movesRemaining = 15 - (currentLevel - 30);
        movesMade = 0;
    }
    // Beyond level 35: Random hard challenges
    else
    {
        mazeWidth = 31;
        mazeHeight = 23;
        teleportMode = GetRandomValue(false, true);
    }

    // Refresh explored cells if fog mode is enabled
    if (fogMode)
    {
        exploredCells.clear();
        exploredCells.resize(mazeHeight, std::vector<bool>(mazeWidth, false));
    }

    offsetX = (screenWidth - mazeWidth * cellSize) / 2.0f;
    offsetY = (screenHeight - mazeHeight * cellSize) / 2.0f;
}

void Game::GenerateLevel() 
{
    ConfigureLevelDifficulty();

    // Generate maze
    MazeGenerator::Generate(maze, mazeWidth, mazeHeight);

    // Set start position
    ball.cellX = 0;
    ball.cellY = 0;
    ball.x = offsetX + ball.cellX * cellSize + cellSize / 2.0f;
    ball.y = offsetY + ball.cellY * cellSize + cellSize / 2.0f;

    // Set goal position
    goalX = mazeWidth - 1;
    goalY = mazeHeight - 1;

    // Generate teleports if in teleport mode
    if (teleportMode) 
    {
        int numTeleports = 1;

        if (currentLevel >= 24) numTeleports = 2; // Level 25+
        if (currentLevel >= 27) numTeleports = 3; // Level 28+
        if (currentLevel >= 30) numTeleports = 4; // Level 31+
    
        MazeGenerator::GenerateTeleports(maze, teleports, mazeWidth, mazeHeight, goalX, goalY, numTeleports);
    }

    // Create ball controller
    if (ballController) 
    {
        delete ballController;
    }
    
    ballController = new BallController(ball, maze, teleports, mazeWidth, mazeHeight, cellSize, offsetX, offsetY, exploredCells, fogMode);
    
    ballController->SetGoal(goalX, goalY);
    ballController->SetTeleportMode(teleportMode);
    ballController->UpdateAvailableDirections(); 
    
    if (fogMode)
    {
        ballController->UpdateFogAroundBall();
    }
   
    levelComplete = false;
}

void Game::Update()
{
    if (levelComplete) return;

    // Update timer
    if (timedMode) 
    {
        timeRemaining -= GetFrameTime();
    
        if (timeRemaining <= 0.0f) 
        {
            RestartLevel();
        }
    }

    // Update ball movement
    ballController->Update(GetFrameTime());

    // Check if reached goal
    if (ball.cellX == goalX && ball.cellY == goalY && !ballController->IsMoving()) 
    {
        levelComplete = true;
    }
}

void Game::HandleInput() 
{
    if (levelComplete) 
    {
        if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
        {
            NextLevel();
        }
        
        return;
    }

    // Check if out of moves
    if (limitedMovesMode && !ballController->IsMoving() && 
        movesMade >= movesRemaining && (ball.cellX != goalX || ball.cellY != goalY)) 
    {
        if (IsKeyPressed(KEY_R)) 
        {
            RestartLevel();
        }
        
        return;
    }

    if (ballController->IsMoving()) return;

    const std::vector<int>& availableDirections = ballController->GetAvailableDirections();

    // Keyboard input
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) 
    {
        for (int dir : availableDirections) 
        {
            if (dir == 0) 
            {
                ballController->Move(0, limitedMovesMode, movesMade, movesRemaining);
                break;
            }
        }
    }
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) 
    {
        for (int dir : availableDirections) 
        {
            if (dir == 1) 
            {
                ballController->Move(1, limitedMovesMode, movesMade, movesRemaining);
                break;
            }
        }
    }
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) 
    {
        for (int dir : availableDirections) 
        {
            if (dir == 2) 
            {
                ballController->Move(2, limitedMovesMode, movesMade, movesRemaining);
                break;
            }
        }
    }
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) 
    {
        for (int dir : availableDirections) 
        {
            if (dir == 3) 
            {
                ballController->Move(3, limitedMovesMode, movesMade, movesRemaining);
                break;
            }
        }
    }

    // Mouse input
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    {
        Vector2 mousePos = GetMousePosition();

        for (int dir : availableDirections) 
        {
            float dotX = ball.x;
            float dotY = ball.y;

            if (dir == 0) dotY -= cellSize;
            else if (dir == 1) dotX += cellSize;
            else if (dir == 2) dotY += cellSize;
            else if (dir == 3) dotX -= cellSize;

            float dist = sqrt(pow(mousePos.x - dotX, 2) + pow(mousePos.y - dotY, 2));
            if (dist < 25.0f) 
            {
                ballController->Move(dir, limitedMovesMode, movesMade, movesRemaining);
                break;
            }
        }
    }
}

void Game::NextLevel() 
{
    currentLevel++;

    // Save progress when advancing to next level
    SaveManager::SaveProgress(currentLevel);
    
    GenerateLevel();
}

void Game::RestartLevel() 
{
    GenerateLevel();
}

bool Game::IsBallMoving() const 
{
    return ballController ? ballController->IsMoving() : false;
}

const std::vector<int>& Game::GetAvailableDirections() const 
{
    return ballController->GetAvailableDirections();
}
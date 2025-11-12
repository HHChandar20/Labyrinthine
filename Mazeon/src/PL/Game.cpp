#include "Game.h"
#include "../BLL/MazeGenerator.h"
#include "raylib.h"

Game::Game()
    : currentLevel(0), levelComplete(false),
    timedMode(false), teleportMode(false), limitedMovesMode(false),
    timeLimit(0.0f), timeRemaining(0.0f), movesRemaining(0), movesMade(0),
    mazeWidth(0), mazeHeight(0), offsetX(0), offsetY(0),
    goalX(0), goalY(0) {}

Game::~Game() {}

void Game::Initialize() 
{
    GenerateLevel();
}

void Game::ConfigureLevelDifficulty() 
{
    // Reset modes
    timedMode = false;
    teleportMode = false;
    limitedMovesMode = false;
    teleports.clear();

    // Level 1: Very easy (9x7)
    if (currentLevel == 0) 
    {
        mazeWidth = 9;
        mazeHeight = 7;
    }
    // Level 2: Easy (13x9)
    else if (currentLevel == 1) 
    {
        mazeWidth = 13;
        mazeHeight = 9;
    }
    // Level 3: Medium (17x11)
    else if (currentLevel == 2) 
    {
        mazeWidth = 17;
        mazeHeight = 11;
    }
    // Level 4: Medium-Hard (19x13)
    else if (currentLevel == 3) 
    {
        mazeWidth = 19;
        mazeHeight = 13;
    }
    // Levels 5-9: Timed mode (progressively less time)
    else if (currentLevel >= 4 && currentLevel < 9) 
    {
        mazeWidth = 21;
        mazeHeight = 15;
        timedMode = true;
        timeLimit = 65.0f - (currentLevel - 4) * 5.0f;
        timeRemaining = timeLimit;
    }
    // Levels 10-14: Teleport mode
    else if (currentLevel >= 9 && currentLevel < 14) 
    {
        mazeWidth = 23;
        mazeHeight = 17;
        teleportMode = true;
        timedMode = true;
        timeLimit = 50.0f - (currentLevel - 9) * 3.0f;
        timeRemaining = timeLimit;
    }
    // Levels 15-20: Limited moves mode
    else if (currentLevel >= 14 && currentLevel < 20) 
    {
        mazeWidth = 25;
        mazeHeight = 17;
        limitedMovesMode = true;
        teleportMode = true;
        movesRemaining = 27 - (currentLevel - 14) * 2;
        movesMade = 0;
    }
    // Beyond level 20: All modes combined
    else 
    {
        mazeWidth = 27;
        mazeHeight = 19;
        timedMode = GetRandomValue(0, 1);
        teleportMode = true;
        limitedMovesMode = GetRandomValue(0, 1);
        timeLimit = 40.0f;
        timeRemaining = timeLimit;
        movesRemaining = 15;
        movesMade = 0;
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
        int numTeleports = 2 + (currentLevel - 9) / 2;
    
        MazeGenerator::GenerateTeleports(maze, teleports, mazeWidth, mazeHeight, goalX, goalY, numTeleports);
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
            // Time's up - restart level
            GenerateLevel();
        }
    }

    // Check if reached goal
    if (ball.cellX == goalX && ball.cellY == goalY) 
    {
        levelComplete = true;
    }
}
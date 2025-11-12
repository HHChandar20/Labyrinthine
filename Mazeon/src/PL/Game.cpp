#include "Game.h"
#include "../BLL/MazeGenerator.h"

Game::Game()
    : currentLevel(0), levelComplete(false),
    mazeWidth(0), mazeHeight(0), offsetX(0), offsetY(0),
    goalX(0), goalY(0) {}

Game::~Game() {}

void Game::Initialize() 
{
    GenerateLevel();
}

void Game::GenerateLevel() 
{
    // Start with a simple 9x7 maze
    mazeWidth = 9;
    mazeHeight = 7;

    offsetX = (screenWidth - mazeWidth * cellSize) / 2.0f;
    offsetY = (screenHeight - mazeHeight * cellSize) / 2.0f;

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

    levelComplete = false;
}
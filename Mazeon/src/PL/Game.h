#pragma once

#include "../DAL/Ball.h"
#include "../DAL/Cell.h"
#include "../DAL/Teleport.h"
#include <vector>

class Game 
{
private:
    // Screen settings
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    const int cellSize = 40;

    // Maze data
    std::vector<std::vector<Cell>> maze;
    int mazeWidth;
    int mazeHeight;
    float offsetX;
    float offsetY;

    // Game objects
    Ball ball;
    int goalX, goalY;
    std::vector<Teleport> teleports;

    // Level state
    int currentLevel;
    bool levelComplete;

public:
    Game();
    ~Game();

    void Initialize();
    void GenerateLevel();

    // Getters
    const std::vector<std::vector<Cell>>& GetMaze() const { return maze; }
    const Ball& GetBall() const { return ball; }
    int GetGoalX() const { return goalX; }
    int GetGoalY() const { return goalY; }
    const std::vector<Teleport>& GetTeleports() const { return teleports; }
    int GetMazeWidth() const { return mazeWidth; }
    int GetMazeHeight() const { return mazeHeight; }
    int GetCellSize() const { return cellSize; }
    float GetOffsetX() const { return offsetX; }
    float GetOffsetY() const { return offsetY; }
    int GetCurrentLevel() const { return currentLevel; }
    bool IsLevelComplete() const { return levelComplete; }
};
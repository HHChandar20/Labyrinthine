#pragma once

#include "../Labyrinthine.DAL/Ball.h"
#include "../Labyrinthine.DAL/Cell.h"
#include "../Labyrinthine.DAL/Teleport.h"
#include "../Labyrinthine.BLL/BallService.h"
#include "raylib.h"

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

    // Services
    BallService* ballService;

    // Level state
    int currentLevel;
    bool levelComplete;

    // Game modes
    bool timedMode;
    float timeLimit;
    float timeRemaining;

    bool teleportMode;

    bool limitedMovesMode;
    int movesRemaining;
    int movesMade;

    bool fogMode;
    std::vector<std::vector<bool>> exploredCells;

    void ConfigureLevelDifficulty();

public:
    Game();
    ~Game();

    void Initialize();
    void StartNewGame();      // NEW: Start from level 0
    void ContinueGame();      // NEW: Load saved progress
    void GenerateLevel();
    void Update();
    void HandleInput();
    void NextLevel();
    void RestartLevel();

    // Getters
    const std::vector<std::vector<Cell>>& GetMaze() const { return maze; }
    int GetMazeWidth() const { return mazeWidth; }
    int GetMazeHeight() const { return mazeHeight; }
    int GetCellSize() const { return cellSize; }

    int GetGoalX() const { return goalX; }
    int GetGoalY() const { return goalY; }

    const std::vector<Teleport>& GetTeleports() const { return teleports; }

    const Ball& GetBall() const { return ball; }
    float GetOffsetX() const { return offsetX; }
    float GetOffsetY() const { return offsetY; }
    bool IsBallMoving() const;
    const std::vector<int>& GetAvailableDirections() const;

    int GetCurrentLevel() const { return currentLevel; }
    bool IsLevelComplete() const { return levelComplete; }

    bool IsTimedMode() const { return timedMode; }
    float GetTimeRemaining() const { return timeRemaining; }

    bool IsLimitedMovesMode() const { return limitedMovesMode; }
    int GetMovesRemaining() const { return movesRemaining; }
    int GetMovesMade() const { return movesMade; }

    bool IsFogMode() const { return fogMode; }
    std::vector<std::vector<bool>> GetExploredCells() const { return exploredCells; }
    float GetCurrentFogRadius() const;

    bool IsTeleportMode() const { return teleportMode; }

    void SetLevelComplete(bool complete) { levelComplete = complete; }
};
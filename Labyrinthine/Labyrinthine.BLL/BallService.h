#pragma once

#include "../Labyrinthine.DAL/Ball.h"
#include "../Labyrinthine.DAL/Cell.h"
#include "../Labyrinthine.DAL/PathNode.h"
#include "../Labyrinthine.DAL/Teleport.h"
#include <cmath>
#include <vector>

class BallService 
{
private:
    Ball& ball;
    std::vector<std::vector<Cell>>& maze;
    std::vector<Teleport>& teleports;

    std::vector<std::vector<bool>>& exploredCells;
    bool fogMode;
    float fogRadius;
    float pulseTime;
    float baseFogRadius;

    bool isMoving;
    std::vector<PathNode> currentPath;
    int pathIndex;
    float moveProgress;
    float moveSpeed;

    int mazeWidth;
    int mazeHeight;
    int cellSize;
    float offsetX;
    float offsetY;
    int goalX;
    int goalY;
    bool teleportMode;

    std::vector<int> availableDirections;

    bool IsIntersection(int x, int y);
    void FindPathToNextIntersection(int direction);
    void HandleTeleport();

public:
    BallService(Ball& b, std::vector<std::vector<Cell>>& m,
        std::vector<Teleport>& t, int width, int height,
        int cellSz, float offX, float offY,
        std::vector<std::vector<bool>>& explored, bool fogMode);

    void UpdateAvailableDirections();
    void SetGoal(int x, int y);
    void SetTeleportMode(bool enabled);
    void Move(int direction, bool limitedMoves, int& movesMade, int movesRemaining);
    void Update(float deltaTime);
    void UpdateFogAroundBall();
    float GetCurrentFogRadius() const { return fogRadius; }

    bool IsMoving() const { return isMoving; }
    const std::vector<int>& GetAvailableDirections() const { return availableDirections; }
    void Reset();
};
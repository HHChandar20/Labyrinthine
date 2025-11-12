#include "BallController.h"
#include <cmath>

BallController::BallController(Ball& b, std::vector<std::vector<Cell>>& m,
    std::vector<Teleport>& t, int width, int height,
    int cellSz, float offX, float offY)
    : ball(b), maze(m), teleports(t), mazeWidth(width), mazeHeight(height),
    cellSize(cellSz), offsetX(offX), offsetY(offY),
    isMoving(false), pathIndex(0), moveProgress(0.0f), moveSpeed(7.5f),
    goalX(0), goalY(0), teleportMode(false) {}

void BallController::SetGoal(int x, int y) 
{
    goalX = x;
    goalY = y;
}

void BallController::SetTeleportMode(bool enabled) 
{
    teleportMode = enabled;
}

void BallController::UpdateAvailableDirections() 
{
    availableDirections.clear();

    Cell& cell = maze[ball.cellY][ball.cellX];

    if (!cell.walls[0]) availableDirections.push_back(0); // up
    if (!cell.walls[1]) availableDirections.push_back(1); // right
    if (!cell.walls[2]) availableDirections.push_back(2); // down
    if (!cell.walls[3]) availableDirections.push_back(3); // left
}

void BallController::Update(float deltaTime) 
{
    // TODO: Implement ball movement update
}

void BallController::Reset() 
{
    isMoving = false;
    currentPath.clear();
    pathIndex = 0;
    moveProgress = 0.0f;
    availableDirections.clear();
}
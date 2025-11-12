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

bool BallController::IsIntersection(int x, int y) 
{
    if (x < 0 || x >= mazeWidth || y < 0 || y >= mazeHeight) return false;

    Cell& cell = maze[y][x];
    int openPaths = 0;

    if (!cell.walls[0]) openPaths++;
    if (!cell.walls[1]) openPaths++;
    if (!cell.walls[2]) openPaths++;
    if (!cell.walls[3]) openPaths++;

    return openPaths > 2 || (x == goalX && y == goalY) || (x == 0 && y == 0) || cell.isTeleport;
}

void BallController::FindPathToNextIntersection(int direction) 
{
    currentPath.clear();
    pathIndex = 0;
    moveProgress = 0.0f;

    int x = ball.cellX;
    int y = ball.cellY;

    while (true) 
    {
        // Move in direction
        if (direction == 0) y--;
        else if (direction == 1) x++;
        else if (direction == 2) y++;
        else if (direction == 3) x--;

        if (x < 0 || x >= mazeWidth || y < 0 || y >= mazeHeight) break;

        currentPath.push_back(PathNode(x, y, direction));

        // Check if we reached an intersection
        if (IsIntersection(x, y)) 
        {
            break;
        }

        // Find next direction (continue straight or turn)
        Cell& cell = maze[y][x];
        int newDir = -1;

        // Try to continue straight
        int straightDir = direction;
        
        if (straightDir == 0 && !cell.walls[0]) newDir = 0;
        else if (straightDir == 1 && !cell.walls[1]) newDir = 1;
        else if (straightDir == 2 && !cell.walls[2]) newDir = 2;
        else if (straightDir == 3 && !cell.walls[3]) newDir = 3;

        // If can't go straight, find another way (not backwards)
        if (newDir == -1) 
        {
            int opposite = (direction + 2) % 4;
        
            for (int d = 0; d < 4; d++) 
            {
                if (d != opposite && !cell.walls[d]) 
                {
                    newDir = d;
                    break;
                }
            }
        }

        if (newDir == -1) break; // Dead end
        direction = newDir;
    }

    if (!currentPath.empty()) 
    {
        isMoving = true;
    }
}

void BallController::Move(int direction) 
{
    FindPathToNextIntersection(direction);
}

void BallController::Update(float deltaTime) 
{
    // TODO: Implement ball movement interpolation
}

void BallController::Reset() 
{
    isMoving = false;
    currentPath.clear();
    pathIndex = 0;
    moveProgress = 0.0f;
    availableDirections.clear();
}
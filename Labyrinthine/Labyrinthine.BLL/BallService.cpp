#include "BallService.h"

BallService::BallService(Ball& b, std::vector<std::vector<Cell>>& m,
    std::vector<Teleport>& t, int width, int height,
    int cellSz, float offX, float offY, std::vector<std::vector<bool>>& explored, bool fogMode)
    : ball(b), maze(m), teleports(t), mazeWidth(width), mazeHeight(height),
    cellSize(cellSz), offsetX(offX), offsetY(offY),
    isMoving(false), pathIndex(0), moveProgress(0.0f), moveSpeed(7.5f),
    goalX(0), goalY(0), teleportMode(false), exploredCells(explored),
    fogMode(fogMode), baseFogRadius(3.0f), fogRadius(3.0f), pulseTime(0.0f) {}

void BallService::SetGoal(int x, int y) 
{
    goalX = x;
    goalY = y;
}

void BallService::SetTeleportMode(bool enabled) 
{
    teleportMode = enabled;
}

void BallService::UpdateAvailableDirections() 
{
    availableDirections.clear();

    Cell& cell = maze[ball.cellY][ball.cellX];

    if (!cell.walls[0]) availableDirections.push_back(0); // up
    if (!cell.walls[1]) availableDirections.push_back(1); // right
    if (!cell.walls[2]) availableDirections.push_back(2); // down
    if (!cell.walls[3]) availableDirections.push_back(3); // left
}

bool BallService::IsIntersection(int x, int y) 
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

void BallService::FindPathToNextIntersection(int direction) 
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

        // Check if intersection is reached
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

void BallService::HandleTeleport() 
{
    if (!teleportMode) return;

    Cell& cell = maze[ball.cellY][ball.cellX];

    if (cell.isTeleport) 
    {
        int pairId = cell.teleportPairId;

        for (const Teleport& tp : teleports) 
        {
            if (tp.id == pairId) 
            {
                // Find the other teleport
                if (ball.cellX == tp.x1 && ball.cellY == tp.y1) 
                {
                    ball.cellX = tp.x2;
                    ball.cellY = tp.y2;
                }
                else if (ball.cellX == tp.x2 && ball.cellY == tp.y2) 
                {
                    ball.cellX = tp.x1;
                    ball.cellY = tp.y1;
                }

                ball.x = offsetX + ball.cellX * cellSize + cellSize / 2.0f;
                ball.y = offsetY + ball.cellY * cellSize + cellSize / 2.0f;

                // Update fog after teleport
                if (fogMode)
                {
                    UpdateFogAroundBall();
                }

                break;
            }
        }
    }
}

void BallService::Move(int direction, bool limitedMoves, int& movesMade, int movesRemaining) 
{
    if (limitedMoves && movesMade >= movesRemaining) 
    {
        return; // No moves left
    }

    FindPathToNextIntersection(direction);

    if (isMoving && limitedMoves) 
    {
        movesMade++;
    }
}

void BallService::Update(float deltaTime) 
{
    // Update pulse animation
    if (fogMode)
    {
        pulseTime += deltaTime;
        fogRadius = baseFogRadius + sin(pulseTime * 3.14159f) * 0.5f;
    }

    if (!isMoving) return;

    moveProgress += moveSpeed * deltaTime;

    if (moveProgress >= 1.0f) 
    {
        moveProgress = 0.0f;
        pathIndex++;

        if (pathIndex >= currentPath.size()) 
        {
            // Reached destination
            isMoving = false;
            PathNode& lastNode = currentPath.back();
            ball.cellX = lastNode.x;
            ball.cellY = lastNode.y;
            ball.x = offsetX + ball.cellX * cellSize + cellSize / 2.0f;
            ball.y = offsetY + ball.cellY * cellSize + cellSize / 2.0f;

            // Handle teleport
            HandleTeleport();

            UpdateAvailableDirections();
        }
        else 
        {
            // Move to next cell in path
            ball.cellX = currentPath[pathIndex].x;
            ball.cellY = currentPath[pathIndex].y;
        }

        // Update fog when reaching new position
        if (fogMode)
        {
            UpdateFogAroundBall();
        }
    }

    // Interpolate ball position
    if (pathIndex < currentPath.size()) 
    {
        PathNode& current = currentPath[pathIndex];
        float targetX = offsetX + current.x * cellSize + cellSize / 2.0f;
        float targetY = offsetY + current.y * cellSize + cellSize / 2.0f;

        if (pathIndex > 0) 
        {
            PathNode& prev = currentPath[pathIndex - 1];
            float startX = offsetX + prev.x * cellSize + cellSize / 2.0f;
            float startY = offsetY + prev.y * cellSize + cellSize / 2.0f;

            ball.x = startX + (targetX - startX) * moveProgress;
            ball.y = startY + (targetY - startY) * moveProgress;
        }
        else 
        {
            float startX = offsetX + ball.cellX * cellSize + cellSize / 2.0f;
            float startY = offsetY + ball.cellY * cellSize + cellSize / 2.0f;

            ball.x = startX + (targetX - startX) * moveProgress;
            ball.y = startY + (targetY - startY) * moveProgress;
        }
    }
}

void BallService::UpdateFogAroundBall()
{
    if (!fogMode) return;

    int centerX = ball.cellX;
    int centerY = ball.cellY;
    float currentRadius = GetCurrentFogRadius();

    for (int y = centerY - static_cast<int>(currentRadius); y <= centerY + static_cast<int>(currentRadius); y++)
    {
        for (int x = centerX - static_cast<int>(currentRadius); x <= centerX + static_cast<int>(currentRadius); x++)
        {
            if (x >= 0 && x < mazeWidth && y >= 0 && y < mazeHeight)
            {
                // Simple circular visibility - mark as explored permanently
                float dist = sqrt(pow(x - centerX, 2) + pow(y - centerY, 2));
                if (dist <= currentRadius)
                {
                    exploredCells[y][x] = true;
                }
            }
        }
    }
}

void BallService::Reset() 
{
    isMoving = false;
    currentPath.clear();
    pathIndex = 0;
    moveProgress = 0.0f;
    availableDirections.clear();
}
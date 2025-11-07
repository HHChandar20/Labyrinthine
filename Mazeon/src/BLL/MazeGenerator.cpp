#include "MazeGenerator.h"
#include "raylib.h"

void MazeGenerator::Generate(std::vector<std::vector<Cell>>& maze, int width, int height) 
{
    // Initialize maze
    maze.clear();
    maze.resize(height);

    for (int y = 0; y < height; y++) 
    {
        maze[y].resize(width);
    
        for (int x = 0; x < width; x++) 
        {
            maze[y][x].x = x;
            maze[y][x].y = y;
            maze[y][x].visited = false;
            maze[y][x].isTeleport = false;
            maze[y][x].teleportPairId = -1;
        
            for (int i = 0; i < 4; i++) 
            {
                maze[y][x].walls[i] = true;
            }
        }
    }

    // TODO: Implement maze generation algorithm
}

void MazeGenerator::GenerateTeleports(std::vector<std::vector<Cell>>& maze,
    std::vector<Teleport>& teleports,
    int width, int height,
    int goalX, int goalY,
    int numTeleports)
{
    teleports.clear();
    
    // TODO: Implement teleport generation
}
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

    // Generate maze using recursive backtracking
    std::vector<Cell*> stack;
    
    Cell* current = &maze[0][0];
    current->visited = true;
    stack.push_back(current);

    while (!stack.empty()) 
    {
        current = stack.back();

        std::vector<Cell*> neighbors;
        int cx = current->x;
        int cy = current->y;

        // Check neighbors in all 4 directions
        // Up
        if (cy > 0 && !maze[cy - 1][cx].visited)
        {
            neighbors.push_back(&maze[cy - 1][cx]);
        }
        
        // Right
        if (cx < width - 1 && !maze[cy][cx + 1].visited)
        {
            neighbors.push_back(&maze[cy][cx + 1]);
        }
        
        // Down
        if (cy < height - 1 && !maze[cy + 1][cx].visited)
        {
            neighbors.push_back(&maze[cy + 1][cx]);
        }
        
        // Left
        if (cx > 0 && !maze[cy][cx - 1].visited)
        {
            neighbors.push_back(&maze[cy][cx - 1]);
        }


        if (!neighbors.empty()) 
        {
            Cell* next = neighbors[GetRandomValue(0, neighbors.size() - 1)];
            next->visited = true;

            // Remove walls
            int dx = next->x - current->x;
            int dy = next->y - current->y;

            if (dy == -1) 
            { 
                current->walls[0] = false;
                next->walls[2] = false;
            }
            
            if (dx == 1) 
            { 
                current->walls[1] = false; 
                next->walls[3] = false;
            }
            
            if (dy == 1) 
            { 
                current->walls[2] = false;
                next->walls[0] = false;
            }
            
            if (dx == -1) 
            { 
                current->walls[3] = false;
                next->walls[1] = false; 
            }

            stack.push_back(next);
        }
        else 
        {
            stack.pop_back();
        }
    }
}

void MazeGenerator::GenerateTeleports(std::vector<std::vector<Cell>>& maze,
    std::vector<Teleport>& teleports,
    int width, int height,
    int goalX, int goalY,
    int numTeleports)
{
    teleports.clear();
    
    //
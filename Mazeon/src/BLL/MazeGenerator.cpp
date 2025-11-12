#include "MazeGenerator.h"
#include "raylib.h"
#include <cmath>

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

        // Check neighbors
        if (cy > 0 && !maze[cy - 1][cx].visited)
        {
            neighbors.push_back(&maze[cy - 1][cx]);
        }
        
        if (cx < width - 1 && !maze[cy][cx + 1].visited)
        {
            neighbors.push_back(&maze[cy][cx + 1]);
        }
        
        if (cy < height - 1 && !maze[cy + 1][cx].visited)
        {
            neighbors.push_back(&maze[cy + 1][cx]);
        }
        
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

    for (int i = 0; i < numTeleports; i++) 
    {
        int x1, y1, x2, y2;
        int attempts = 0;

        do {
            x1 = GetRandomValue(1, width - 2);
            y1 = GetRandomValue(1, height - 2);
            x2 = GetRandomValue(1, width - 2);
            y2 = GetRandomValue(1, height - 2);
            attempts++;
        } while (attempts < 100 && (
            (x1 == x2 && y1 == y2) ||
            (x1 == 0 && y1 == 0) ||
            (x2 == 0 && y2 == 0) ||
            (x1 == goalX && y1 == goalY) ||
            (x2 == goalX && y2 == goalY) ||
            maze[y1][x1].isTeleport ||
            maze[y2][x2].isTeleport ||
            abs(x1 - x2) + abs(y1 - y2) < 5
            ));

        if (attempts < 100) 
        {
            maze[y1][x1].isTeleport = true;
            maze[y1][x1].teleportPairId = i;
            maze[y2][x2].isTeleport = true;
            maze[y2][x2].teleportPairId = i;

            teleports.push_back(Teleport(x1, y1, x2, y2, i));
        }
    }
}
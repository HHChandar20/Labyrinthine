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

        // Check neighbors (unvisited cells)
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
            // Choose random neighbor
            Cell* next = neighbors[GetRandomValue(0, neighbors.size() - 1)];
            next->visited = true;

            // Remove walls between current and next
            int dx = next->x - current->x;
            int dy = next->y - current->y;

            if (dy == -1) // next is above
            { 
                current->walls[0] = false;  // Remove top wall of current
                next->walls[2] = false;     // Remove bottom wall of next
            }
            
            if (dx == 1) // next is to the right
            { 
                current->walls[1] = false;  // Remove right wall of current
                next->walls[3] = false;     // Remove left wall of next
            }
            
            if (dy == 1) // next is below
            { 
                current->walls[2] = false;  // Remove bottom wall of current
                next->walls[0] = false;     // Remove top wall of next
            }
            
            if (dx == -1) // next is to the left
            { 
                current->walls[3] = false;  // Remove left wall of current
                next->walls[1] = false;     // Remove right wall of next
            }

            stack.push_back(next);
        }
        else 
        {
            // Backtrack
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
    
    // TODO: Implement teleport generation
}
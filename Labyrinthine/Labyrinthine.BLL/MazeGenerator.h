#pragma once

#include "../Labyrinthine.DAL/Cell.h"
#include "../Labyrinthine.DAL/Teleport.h"
#include <vector>
#include "raylib.h"
#include <cmath>

class MazeGenerator 
{
public:
    static void Generate(std::vector<std::vector<Cell>>& maze, int width, int height);
    static void GenerateTeleports(std::vector<std::vector<Cell>>& maze,
        std::vector<Teleport>& teleports,
        int width, int height,
        int goalX, int goalY,
        int numTeleports);
};
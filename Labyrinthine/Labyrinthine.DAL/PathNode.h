#pragma once

struct PathNode 
{
    int x, y;
    int direction;           // 0=up, 1=right, 2=down, 3=left

    PathNode() : x(0), y(0), direction(0) {}

    PathNode(int posX, int posY, int dir) : x(posX), y(posY), direction(dir) {}
};
#pragma once

struct Cell 
{
    int x, y;
    bool walls[4];           // top, right, bottom, left
    bool visited;
    bool isTeleport;
    int teleportPairId;

    Cell() : x(0), y(0), visited(false), isTeleport(false), teleportPairId(-1) 
    {
        for (int i = 0; i < 4; i++) 
        {
            walls[i] = true;
        }
    }
};
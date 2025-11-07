#pragma once

struct Cell 
{
    int x, y;
    bool walls[4];           // top, right, bottom, left
    bool visited;

    Cell() : x(0), y(0), visited(false) 
    {
        for (int i = 0; i < 4; i++) 
        {
            walls[i] = true;
        }
    }
};
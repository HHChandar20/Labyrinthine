#pragma once

struct Teleport 
{
    int x1, y1;              // First teleport position
    int x2, y2;              // Second teleport position
    int id;                  // Unique identifier

    Teleport() : x1(0), y1(0), x2(0), y2(0), id(0) {}

    Teleport(int x1Pos, int y1Pos, int x2Pos, int y2Pos, int identifier)
        : x1(x1Pos), y1(y1Pos), x2(x2Pos), y2(y2Pos), id(identifier) {}
};
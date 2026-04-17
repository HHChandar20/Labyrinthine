#pragma once

struct Ball 
{
    float x, y;              // Screen position
    int cellX, cellY;        // Grid position
    float radius;

    Ball() : x(0), y(0), cellX(0), cellY(0), radius(12.0f) {}

    Ball(float posX, float posY, int gX, int gY, float r) : x(posX), y(posY), cellX(gX), cellY(gY), radius(r) {}
};
#pragma once

#include "Game.h"
#include "raylib.h"
#include <vector>

class GameManager 
{
private:
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    Game game;

    // Colors
    Color bgColor;
    Color wallColor;
    Color ballColor;
    Color glowColor;
    Color goalColor;

    void DrawMaze();
    void DrawGoal();
    void DrawBall();

public:
    GameManager();
    ~GameManager();

    void Run();
    void Update();
    void Draw();
};
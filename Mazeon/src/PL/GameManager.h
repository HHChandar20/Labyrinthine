#pragma once

#include "Game.h"
#include "raylib.h"
#include <vector>

class GameManager 
{
private:
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
    void DrawUI();
    void DrawLevelCompleteScreen();

public:
    GameManager();
    ~GameManager();

    void Run();
    void Update();
    void Draw();
};
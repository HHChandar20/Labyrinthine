#pragma once

#include "Game.h"
#include <string>

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
    Color dotColor;
    std::vector<Color> teleportColors;

    void DrawMaze();
    void DrawTeleports();
    void DrawGoal();
    void DrawDirectionDots();
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
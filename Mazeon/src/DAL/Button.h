#pragma once
#include "raylib.h"
#include <string>

class Button
{
private:
    Rectangle bounds;
    std::string text;
    Color normalColor;
    Color hoverColor;
    Color textColor;
    bool isHovered;

public:
    Button(float x, float y, float width, float height, const std::string& buttonText);

    void Update();
    void Draw();
    bool IsClicked();
};
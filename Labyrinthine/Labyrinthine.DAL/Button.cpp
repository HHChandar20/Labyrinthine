#include "Button.h"

Button::Button(float x, float y, float width, float height, const std::string& buttonText)
    : text(buttonText), isHovered(false)
{
    bounds = { x, y, width, height };
    normalColor = { 60, 70, 90, 255 };
    hoverColor = { 120, 200, 255, 255 };
    textColor = WHITE;
}

void Button::Update()
{
    Vector2 mousePos = GetMousePosition();
    isHovered = CheckCollisionPointRec(mousePos, bounds);
}

void Button::Draw()
{
    Color currentColor = isHovered ? hoverColor : normalColor;

    // Draw button background
    DrawRectangleRec(bounds, currentColor);

    // Draw button border
    DrawRectangleLinesEx(bounds, 3, WHITE);

    // Draw text centered
    int textWidth = MeasureText(text.c_str(), 40);
    float textX = bounds.x + (bounds.width - textWidth) / 2;
    float textY = bounds.y + (bounds.height - 40) / 2;

    DrawText(text.c_str(), textX, textY, 40, textColor);
}

bool Button::IsClicked()
{
    return isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
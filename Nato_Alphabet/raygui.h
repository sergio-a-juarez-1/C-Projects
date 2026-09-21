#ifndef RAYGUI_H
#define RAYGUI_H
#define RAYGUI_STATIC
#include "raylib.h"

#ifdef RAYGUI_IMPLEMENTATION
bool GuiTextBox(Rectangle bounds, char *text, int textSize, bool editMode) {
    (void)textSize; // FIXED: Explicitly silence the unused parameter warning
    DrawRectangleRec(bounds, editMode ? LIGHTGRAY : WHITE);
    DrawRectangleLinesEx(bounds, 1.0f, BLACK);
    DrawText(text, bounds.x + 5, bounds.y + 10, 20, BLACK);
    if (CheckCollisionPointRec(GetMousePosition(), bounds) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return true;
    return false;
}
bool GuiButton(Rectangle bounds, const char *text) {
    bool pressed = false;
    Color col = LIGHTGRAY;
    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
        col = GRAY;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) pressed = true;
    }
    DrawRectangleRec(bounds, col);
    DrawRectangleLinesEx(bounds, 1.0f, BLACK);
    DrawText(text, bounds.x + (bounds.width/2) - (MeasureText(text, 16)/2), bounds.y + (bounds.height/2) - 8, 16, BLACK);
    return pressed;
}
void GuiLabel(Rectangle bounds, const char *text) {
    DrawText(text, bounds.x, bounds.y, 16, DARKGRAY);
}
#endif
#endif

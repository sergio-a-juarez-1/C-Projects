#ifndef RAYLIB_H
#define RAYLIB_H

#include <stdbool.h>

typedef struct Vector2 { float x; float y; } Vector2;
typedef struct Rectangle { float x; float y; float width; float height; } Rectangle;
typedef struct Color { unsigned char r; unsigned char g; unsigned char b; unsigned char a; } Color;

#define MOUSE_BUTTON_LEFT 0
#define WHITE (Color){ 255, 255, 255, 255 }
#define GRAY  (Color){ 130, 130, 130, 255 }
#define BLACK (Color){ 0, 0, 0, 255 }

void InitWindow(int width, int height, const char *title);
void SetTargetFPS(int fps);
bool WindowShouldClose(void);
Vector2 GetMousePosition(void);
bool IsMouseButtonPressed(int button);
bool CheckCollisionPointRec(Vector2 point, Rectangle rec);
void BeginDrawing(void);
void ClearBackground(Color color);
void DrawText(const char *text, int posX, int posY, int fontSize, Color color);
const char *TextFormat(const char *text, ...);
void DrawRectangleRec(Rectangle rec, Color color);
void DrawRectangleLinesEx(Rectangle rec, float lineThick, Color color);
void DrawRectangle(int posX, int posY, int width, int height, Color color);
void DrawRectangleLines(int posX, int posY, int width, int height, Color color);
void EndDrawing(void);
void CloseWindow(void);

#endif

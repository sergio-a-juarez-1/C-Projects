// main.c
#include "raylib.h"
#include "player.h"
#include "car_manager.h"
#include "scoreboard.h"

int main(void) {
    // Initialize Raylib context matching original 600x600 layout
    InitWindow(600, 600, "C Turtle Crossing Game");
    SetTargetFPS(60); // Ensures uniform frame rates across devices

    Player player;
    CarManager car_manager;
    Scoreboard scoreboard;

    InitPlayer(&player);
    InitCarManager(&car_manager);
    InitScoreboard(&scoreboard);

    // Main Game Loop
    while (!WindowShouldClose()) {
        // -------------------------
        // 1. Input Handling
        // -------------------------
        if (!scoreboard.game_over) {
            if (IsKeyPressed(KEY_UP)) {
                MovePlayerUp(&player);
            }
        }

        // -------------------------
        // 2. Logic Update
        // -------------------------
        if (!scoreboard.game_over) {
            CreateCar(&car_manager);
            MoveCars(&car_manager);

            // Check if player collided with a car
            if (CheckCarCollisions(&car_manager, &player)) {
                scoreboard.game_over = true;
            }

            // Check if player crossed finish line
            if (IsPlayerAtFinishLine(&player)) {
                LevelUpCars(&car_manager);
                IncreaseLevel(&scoreboard);
            }
        }

        // -------------------------
        // 3. Render / Draw
        // -------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawPlayer(&player);
            DrawCars(&car_manager);
            DrawScoreboard(&scoreboard);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

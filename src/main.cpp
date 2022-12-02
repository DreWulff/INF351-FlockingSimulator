#include "raylib.h"

int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Flocking Simulator");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 400, 400, 40, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}


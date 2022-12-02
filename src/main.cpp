#include "raylib.h"
#include <iostream>
#include <vector>
#include <random>

struct boid
{
    float x;
    float y;
    float vx;
    float vy;
};

// Globales
const int boidCount = 100;
const int screenWidth = 1600;
const int screenHeight = 900;
const float avoidFactor = 0.05;
const float matchingFactor = 0.05;
const float centeringFactor = 0.0005;
const float turnFactor = 0.2;
const float maxSpeed = 6;
const float minSpeed = 3;
const float visualRange = 40;
const float protectedRange = 8;
const float maxBias = 0.01;
const float biasIncrement = 0.00004;
const float biasVal = 0.001;

boid boids[boidCount];

void DrawBoids()
{
    for (auto birb: boids)
    {
        DrawTriangleLines((Vector2){ birb.x-10, birb.y-10 },
                              (Vector2){ birb.x-10, birb.y+10 },
                              (Vector2){ birb.x+15, birb.y }, DARKBLUE);
    }
}

int main(void) {
    // Interfaz grafica
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Flocking Simulator");
    SetTargetFPS(60);
    
    for (int i; i < boidCount; i++)
    {
        float initX = rand() % screenWidth;
        float initY = rand() % screenHeight;
        float initVX = rand() % maxSpeed+ minSpeed;
        float initVY = rand() % maxSpeed + minSpeed;
        boids[i] = boid{.x=initX, .y=initY, .vx=initVX, .vy=initVY};
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawBoids();
            DrawText("Congrats! You created your first window!", 400, 400, 40, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

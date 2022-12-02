#include "raylib.h"
#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <bits/stdc++.h>

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
        float speed = sqrt((birb.vx * birb.vx) + (birb.vy * birb.vy));
        float normalX = birb.vx / speed;
        float normalY = birb.vy / speed;
        DrawTriangle((Vector2){birb.x-10*normalX, birb.y-10*normalY },
                              (Vector2){ birb.x-10*normalX, birb.y+10*normalY },
                              (Vector2){ birb.x+15*normalX, birb.y+normalY }, DARKBLUE);
    }
}

int main(void) {
    // Interfaz grafica
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Flocking Simulator");
    SetTargetFPS(60);
    
    for (int i = 0; i < boidCount; i++)
    {
        float initX = rand() % (int) screenWidth;
        float initY = rand() % (int) screenHeight;
        float initVX = rand() % (int) (maxSpeed + minSpeed);
        float initVY = rand() % (int) (maxSpeed + minSpeed);
        boids[i] = boid{.x=initX, .y=initY, .vx=initVX, .vy=initVY};
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawBoids();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

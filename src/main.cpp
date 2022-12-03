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

struct point {
    float x;
    float y;
};

point rotate_point(float cx, float cy, float angle, point point) {
    float s = sin(angle);
    float c = cos(angle);

    // translate point back to origin:
    point.x -= cx;
    point.y -= cy;

    // rotate point
    float xnew = point.x * c - point.y * s;
    float ynew = point.x * s + point.y * c;

    // translate point back:
    point.x = xnew + cx;
    point.y = ynew + cy;
    return point;
}

// Globales
const int boidCount = 100;
const int screenWidth = 1600;
const int screenHeight = 900;
const float avoidFactor = 0.05;
const float matchingFactor = 0.05;
const float centeringFactor = 0.0005;
const float turnFactor = 0.2;
const float maxSpeed = 3;
const float minSpeed = 1;
const float visualRange = 40;
const float protectedRange = 8;
const float maxBias = 0.01;
const float biasIncrement = 0.00004;
const float biasVal = 0.001;
const int leftMargin = 150;
const int rightMargin = 1450;
const int upperMargin = 750;
const int lowerMargin = 150;

boid boids[boidCount];

point Turning (boid birb)
{
    float vx;
    float vy;
    if (birb.x < leftMargin)    {vx = turnFactor;}
    if (birb.x > rightMargin)   {vx = -turnFactor;}
    if (birb.y < lowerMargin)   {vy = turnFactor;}
    if (birb.y < upperMargin)   {vy = -turnFactor;}
    return ( point {.x=vx, .y=vy} );
}

point Separation (boid birb)
{
    float vx = 0;
    float vy = 0;
    for (auto other: boids)
    {
        float distance = sqrt((other.x - birb.x)*(other.x - birb.x) + (other.y - birb.y)*(other.y - birb.y));
        if (distance < protectedRange)
        {
            vx = vx + (birb.x - other.x);
            vy = vy + (birb.y - other.y);
        }
    }
    return ( point {.x=vx*avoidFactor, .y=vy*avoidFactor} );
}

point Alignment (boid birb)
{
    float vx_avg = 0;
    float vy_avg = 0;
    float neighbours = 0;
    for (auto other: boids)
    {
        float distance = sqrt((other.x - birb.x)*(other.x - birb.x) + (other.y - birb.y)*(other.y - birb.y));
        if (distance < visualRange)
        {
            vx_avg += other.vx;
            vy_avg += other.vy;
            neighbours += 1;
        }
    }
    vx_avg = vx_avg / neighbours;
    vy_avg = vy_avg / neighbours;
    return ( point{.x=vx_avg*matchingFactor, .y=vy_avg*matchingFactor} );
}

point Cohesion (boid birb)
{
    float x_avg = 0;
    float y_avg = 0;
    float neighbours = 0;
    for (auto other: boids)
    {
        float distance = sqrt((other.x - birb.x)*(other.x - birb.x) + (other.y - birb.y)*(other.y - birb.y));
        if (distance < visualRange)
        {
            x_avg += other.x;
            y_avg += other.y;
            neighbours += 1;
        }
    }
    x_avg = x_avg / neighbours;
    y_avg = y_avg / neighbours;
    return ( point{.x=x_avg*centeringFactor, .y=y_avg*centeringFactor} );
}

void UpdateBoids()
{
    boid newBirbs[boidCount] = boids;
    for (int i = 0; i < boidCount; i++)
    {
        boid birb = boids[i];
        point turn  = Turning(birb);
        point avoid = Separation(birb);
        point align = Alignment(birb);
        point cohes = Cohesion(birb);

        float vx = birb.vx + turn.x + avoid.x + align.x + cohes.x;
        float vy = birb.vy + turn.y + avoid.y + align.y + cohes.y;
        float speed = sqrt((vx*vx) + (vy*vy));
        if (speed > maxSpeed)
        {
            vx = (vx/speed) * maxSpeed;
            vy = (vy/speed) * maxSpeed;
        }
        if (speed < minSpeed)
        {
            vx = (vx/speed) * minSpeed;
            vy = (vy/speed) * minSpeed;
        }
        newBirbs[i].x += vx;
        newBirbs[i].y += vy;
        newBirbs[i].vx = vx;
        newBirbs[i].vy = vy;
    }
    boids = newBirbs;
}

void DrawBoids()
{
    for (auto birb: boids)
    {
        float angle = atan(birb.vy / birb.vx);
        if (birb.vx > 0.0 && birb.vy > 0.0) angle = angle;
        if (birb.vx < 0.0 && birb.vy > 0.0) angle += M_PI;
        if (birb.vx < 0.0 && birb.vy < 0.0) angle += M_PI;
        if (birb.vx > 0.0 && birb.vy < 0.0) angle += 2 * M_PI; 

        point p1 = rotate_point(birb.x, birb.y, angle, point{.x=birb.x - 10, .y=birb.y - 10});
        point p2 = rotate_point(birb.x, birb.y, angle, point{.x=birb.x - 10, .y=birb.y + 10});
        point p3 = rotate_point(birb.x, birb.y, angle, point{.x=birb.x + 20, .y=birb.y});
        DrawTriangle((Vector2){p1.x, p1.y}, (Vector2){p2.x, p2.y}, (Vector2){p3.x, p3.y}, DARKBLUE);
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
        
        float angle = rand() % 360;
        float magnitude = (rand() % (int) (maxSpeed - minSpeed)) + minSpeed;
        
        float initVX = cos((2 * M_PI) * (angle / 360)) * magnitude;
        float initVY = sin((2 * M_PI) * (angle / 360)) * magnitude;
        boids[i] = boid{.x=initX, .y=initY, .vx=initVX, .vy=initVY};
    }

    while (!WindowShouldClose()) {
        UpdateBoids();

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawBoids();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

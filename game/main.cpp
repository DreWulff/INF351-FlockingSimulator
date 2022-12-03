#include <math.h>
#include <random>

#include "raylib.h"

// Definición de tipos
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

// Constantes
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
const int pi = 3.14159265358979323846;

// Globales
boid boids[boidCount];

// Funciones
point RotatePoint(float cx, float cy, float angle, point point)
{
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

void UpdateBoids()
{
	for (auto& birb: boids)
	{
		birb.x += birb.vx;
		birb.y += birb.vy;
	}
}

void DrawBoids()
{
    for (auto birb: boids)
    {
        float angle = atan(birb.vy / birb.vx);
        if (birb.vx > 0.0 && birb.vy > 0.0) angle = angle;
        if (birb.vx < 0.0 && birb.vy > 0.0) angle += pi;
        if (birb.vx < 0.0 && birb.vy < 0.0) angle += pi;
        if (birb.vx > 0.0 && birb.vy < 0.0) angle += 2 * pi;

        point p1 = RotatePoint(birb.x, birb.y, angle, point{.x=birb.x - 10, .y=birb.y - 10});
        point p2 = RotatePoint(birb.x, birb.y, angle, point{.x=birb.x - 10, .y=birb.y + 10});
        point p3 = RotatePoint(birb.x, birb.y, angle, point{.x=birb.x + 20, .y=birb.y});
        DrawTriangle((Vector2){p1.x, p1.y}, (Vector2){p2.x, p2.y}, (Vector2){p3.x, p3.y}, DARKBLUE);
    }
}

// Función main
int main ()
{
	// Inicializar boids
	for (int i = 0; i < boidCount; i++)
    {
        float initX = rand() % (int) screenWidth;
        float initY = rand() % (int) screenHeight;

        float angle = rand() % 360;
        float magnitude = (rand() % (int) (maxSpeed - minSpeed)) + minSpeed;

        float initVX = cos((2 * pi) * (angle / 360)) * magnitude;
        float initVY = sin((2 * pi) * (angle / 360)) * magnitude;
        boids[i] = boid{.x=initX, .y=initY, .vx=initVX, .vy=initVY};
    }

	// Interfaz grafica
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Flocking Simulator");
    SetTargetFPS(60);

	// Main loop
	while (!WindowShouldClose())
	{
		// Update
		UpdateBoids();

		// Draw
		BeginDrawing();
		ClearBackground(WHITE);
		DrawBoids();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
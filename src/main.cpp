#include <math.h>
#include <random>
#include <time.h>
#include <iostream>

#include "raylib.h"

// Definición de tipos
struct boid
{
    float x;
    float y;
    float vx;
    float vy;
};

struct vector2
{
    float x;
    float y;
};

// Constantes
const int screenWidth = 1600;
const int screenHeight = 900;
const int boidSize = 6;
const int pi = 3.14159265358979323846;

// Funcion de lectura de output
void read_file(const char *filename, int* iterations, int* boidCount, boid** boids) {
	FILE *fp = fopen(filename, "r");
	// Leer M, N, P
	fscanf(fp, "%d %d", iterations, boidCount);
    float aux_x;
    float aux_y;
    float aux_vx;
    float aux_vy;
    fscanf(fp, "\n");

	// Leer boids
    *boids = (boid*) malloc(sizeof(struct boid) * (*iterations) * (*boidCount));
    for (int frame = 0; frame < (*iterations); frame++)
    {
        for (int birb = 0; birb < (*boidCount); birb++)
        {
            fscanf(fp, "%f ", &aux_x);
            fscanf(fp, "%f ", &aux_y);
            fscanf(fp, "%f ", &aux_vx);
            fscanf(fp, "%f ", &aux_vy);
            (*boids)[frame * (*boidCount) + birb] = boid{.x=aux_x, .y=aux_y, .vx=aux_vx, .vy=aux_vy};
        }
        fscanf(fp, "\n");
	}
}

// Funcion de rotacion de puntos para dibujo de triangulos
vector2 RotatePoint(float cx, float cy, float angle, vector2 point)
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
    return ( point );
}

// Funcion de dibujo
void DrawBoids(int iteration, boid *boids, int boidCount)
{
    for (int i = 0; i < boidCount; i++)
    {
        float angle = atan(boids[iteration * boidCount + i].vy / boids[iteration * boidCount + i].vx);
        if (boids[iteration * boidCount + i].vx > 0.0 && boids[iteration * boidCount + i].vy > 0.0) angle = angle;
        if (boids[iteration * boidCount + i].vx < 0.0 && boids[iteration * boidCount + i].vy > 0.0) angle += pi;
        if (boids[iteration * boidCount + i].vx < 0.0 && boids[iteration * boidCount + i].vy < 0.0) angle += pi;
        if (boids[iteration * boidCount + i].vx > 0.0 && boids[iteration * boidCount + i].vy < 0.0) angle += 2 * pi;

        vector2 p1 = RotatePoint(boids[iteration * boidCount + i].x, boids[iteration * boidCount + i].y, angle, vector2{.x=boids[iteration * boidCount + i].x - boidSize, .y=boids[iteration * boidCount + i].y - boidSize});
        vector2 p2 = RotatePoint(boids[iteration * boidCount + i].x, boids[iteration * boidCount + i].y, angle, vector2{.x=boids[iteration * boidCount + i].x - boidSize, .y=boids[iteration * boidCount + i].y + boidSize});
        vector2 p3 = RotatePoint(boids[iteration * boidCount + i].x, boids[iteration * boidCount + i].y, angle, vector2{.x=boids[iteration * boidCount + i].x + 2 * boidSize, .y=boids[iteration * boidCount + i].y});
        DrawTriangle((Vector2){p1.x, p1.y}, (Vector2){p2.x, p2.y}, (Vector2){p3.x, p3.y}, DARKBLUE);
    }
}


// Función main
int main (int argc, char *argv[])
{
    // Lectura de output
    std::cout << "Reading file...\n";
    int boidCount;
    int iterations;
    boid *boids;
    read_file(argv[1], &iterations, &boidCount, &boids);

    // Crear ventana
    InitWindow(screenWidth, screenHeight, "Flocking Simulator");
    SetTargetFPS(60);

    // Dibujar
    for (int i = 0; i < iterations && !WindowShouldClose(); i++)
    {
        // Draw
        BeginDrawing();
        ClearBackground(WHITE);
            DrawBoids(i, boids, boidCount);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
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

struct vector2
{
    float x;
    float y;
};

// Constantes
const int screenWidth = 1600;
const int screenHeight = 900;

const int boidSize = 6;
const int boidCount = 1000;

const float avoidFactor = 0.05;
const float matchingFactor = 0.05;
const float centeringFactor = 0.005;
const float turnFactor = 0.2;
const float maxSpeed = 4;
const float minSpeed = 2;
const float visualRange = 30;
const float protectedRange = 10;
const int screenMargin = 400;

const int pi = 3.14159265358979323846;

// Globales
boid boids[boidCount];
boid aux[boidCount];

// Funciones
void UpdateBoids()
{
    std::copy(std::begin(boids), std::end(boids), aux);

    for (size_t i = 0; i < boidCount; i++)
    {
        // Inicializar variables auxiliares
        float dxOfCloseBoids = 0.0;
        float dyOfCloseBoids = 0.0;
        float averageVx = 0.0;
        float averageVy = 0.0;
        float averageX = 0.0;
        float averageY = 0.0;
        float neighborCount = 0.0;

        for (size_t j = 0; j < boidCount; j++) {
            if (i != j) {
                float dx = boids[i].x - boids[j].x;
                float dy = boids[i].y - boids[j].y;
                float distance = sqrt(dx * dx + dy * dy);

                if (distance < protectedRange) {
                    // Calcular separación
                    dxOfCloseBoids += dx;
                    dyOfCloseBoids += dy;
                }

                else if (distance < visualRange) {
                    // Calcular velocidad promedio de boids en el rango visual
                    averageVx += boids[j].vx;
                    averageVy += boids[j].vy;

                    // Calcular posición promedio de boids en el rango visual
                    averageX += boids[j].x;
                    averageY += boids[j].y;

                    // Calcular cantidad de vecinos
                    neighborCount += 1;
                }
            }
        }

        // Añadir separación
        aux[i].vx += dxOfCloseBoids * avoidFactor;
        aux[i].vy += dyOfCloseBoids * avoidFactor;

        if (neighborCount > 0) {
            // Añadir alineación
            averageVx /= neighborCount;
            averageVy /= neighborCount;
            aux[i].vx += (averageVx - boids[i].vx) * matchingFactor;
            aux[i].vy += (averageVy - boids[i].vy) * matchingFactor;

            // Añadir cohesión
            averageX /= neighborCount;
            averageY /= neighborCount;
            aux[i].vx += (averageX - boids[i].x) * centeringFactor;
            aux[i].vy += (averageY - boids[i].y) * centeringFactor;
        }

        // Añadir giro a velocidad
        if (boids[i].y < screenMargin) {
            aux[i].vy += turnFactor;
        }
        if (boids[i].y > screenHeight - screenMargin) {
            aux[i].vy += -turnFactor;
        }
        if (boids[i].x < screenMargin) {
            aux[i].vx += turnFactor;
        }
        if (boids[i].x > screenWidth - screenMargin) {
            aux[i].vx += -turnFactor;
        }

        // Restringir velocidad
        float speed = sqrt(aux[i].vx * aux[i].vx + aux[i].vy * aux[i].vy);
        if (speed < minSpeed) {
            aux[i].vx = (aux[i].vx / speed) * minSpeed;
            aux[i].vy = (aux[i].vy / speed) * minSpeed;
        }
        else {
            aux[i].vx = (aux[i].vx / speed) * maxSpeed;
            aux[i].vy = (aux[i].vy / speed) * maxSpeed;
        }

        // Añadir velocidad a posición
        aux[i].x += aux[i].vx;
        aux[i].y += aux[i].vy;

    }

    std::copy(std::begin(aux), std::end(aux), boids);
}

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

void DrawBoids()
{
    for (auto birb: boids)
    {
        float angle = atan(birb.vy / birb.vx);
        if (birb.vx > 0.0 && birb.vy > 0.0) angle = angle;
        if (birb.vx < 0.0 && birb.vy > 0.0) angle += pi;
        if (birb.vx < 0.0 && birb.vy < 0.0) angle += pi;
        if (birb.vx > 0.0 && birb.vy < 0.0) angle += 2 * pi;

        vector2 p1 = RotatePoint(birb.x, birb.y, angle, vector2{.x=birb.x - boidSize, .y=birb.y - boidSize});
        vector2 p2 = RotatePoint(birb.x, birb.y, angle, vector2{.x=birb.x - boidSize, .y=birb.y + boidSize});
        vector2 p3 = RotatePoint(birb.x, birb.y, angle, vector2{.x=birb.x + 2 * boidSize, .y=birb.y});
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

    // Crear ventana
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
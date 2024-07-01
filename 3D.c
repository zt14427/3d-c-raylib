#include <stdio.h>
#include <math.h>
#include "raylib.h"

#define STARTWIDTH 800
#define STARTHEIGHT 600
#define TARGETFPS 240

typedef struct PlayerParams {
    Camera cam;
    RenderTexture screen;
    float frameTime;
    int screenWidth;
    int screenHeight;
    float moveSpeed;
    float theta;
    float phi;
    float mouseSensitivity;
    int targetFPS;
} PlayerParams;

void updateState(PlayerParams* Player);
void getInputs(PlayerParams* Player);
void draw(PlayerParams* Player);

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(TARGETFPS);
    InitWindow(STARTWIDTH, STARTHEIGHT, "3D");

    PlayerParams Player = {
        .cam = {.fovy = 45.0f, .up.y = 1.0f, .target.x = 0.0f, .target.y = 1.0f, .target.z = 0.0f, .position.z = -3.0f, .position.y = 1.0f},
        .screen = LoadRenderTexture(STARTWIDTH, STARTHEIGHT),
        .frameTime = GetFrameTime(),
        .screenWidth = STARTWIDTH,
        .screenHeight = STARTHEIGHT,
        .moveSpeed = 10.0f,
        .theta = 0.0f,
        .phi = 0.0f,
        .mouseSensitivity = 0.01f,
        .targetFPS = 60
    };

    DisableCursor();

    while (!WindowShouldClose()) {
        updateState(&Player);
        getInputs(&Player);
        draw(&Player);
    }
}

void updateState(PlayerParams* Player) {
    (*Player).screenWidth = GetScreenWidth();
    (*Player).screenHeight = GetScreenHeight();
    (*Player).frameTime = GetFrameTime();
}

void getInputs(PlayerParams* Player) {
    if (IsKeyDown(KEY_W)) {
        (*Player).cam.position.z += (*Player).frameTime * (*Player).moveSpeed * sin((*Player).theta);
        (*Player).cam.target.z += (*Player).frameTime * (*Player).moveSpeed * sin((*Player).theta);
        (*Player).cam.position.x += (*Player).frameTime * (*Player).moveSpeed * cos((*Player).theta);
        (*Player).cam.target.x += (*Player).frameTime * (*Player).moveSpeed * cos((*Player).theta);
    }
    if (IsKeyDown(KEY_S)) {
        (*Player).cam.position.z -= (*Player).frameTime * (*Player).moveSpeed * sin((*Player).theta);
        (*Player).cam.target.z -= (*Player).frameTime * (*Player).moveSpeed * sin((*Player).theta);
        (*Player).cam.position.x -= (*Player).frameTime * (*Player).moveSpeed * cos((*Player).theta);
        (*Player).cam.target.x -= (*Player).frameTime * (*Player).moveSpeed * cos((*Player).theta);
    }
    if (IsKeyDown(KEY_A)) {
        (*Player).cam.position.z -= (*Player).frameTime * (*Player).moveSpeed * cos((*Player).theta);
        (*Player).cam.target.z -= (*Player).frameTime * (*Player).moveSpeed * cos((*Player).theta);
        (*Player).cam.position.x += (*Player).frameTime * (*Player).moveSpeed * sin((*Player).theta);
        (*Player).cam.target.x += (*Player).frameTime * (*Player).moveSpeed * sin((*Player).theta);
    }
    if (IsKeyDown(KEY_D)) {
        (*Player).cam.position.z += (*Player).frameTime * (*Player).moveSpeed * cos((*Player).theta);
        (*Player).cam.target.z += (*Player).frameTime * (*Player).moveSpeed * cos((*Player).theta);
        (*Player).cam.position.x -= (*Player).frameTime * (*Player).moveSpeed * sin((*Player).theta);
        (*Player).cam.target.x -= (*Player).frameTime * (*Player).moveSpeed * sin((*Player).theta);
    }

    (*Player).theta += GetMouseDelta().x * (*Player).mouseSensitivity;
    (*Player).phi -= GetMouseDelta().y * (*Player).mouseSensitivity;

    (*Player).cam.target.x = (*Player).cam.position.x + cos((*Player).phi) * cos((*Player).theta);
    (*Player).cam.target.z = (*Player).cam.position.z + cos((*Player).phi) * sin((*Player).theta);
    (*Player).cam.target.y = (*Player).cam.position.y + sin((*Player).phi);
}

void draw(PlayerParams* Player) {
    
    BeginTextureMode((*Player).screen);
        ClearBackground(BLACK);
        BeginMode3D((*Player).cam);
                
            DrawPlane((Vector3) { 0, 0, 0 }, (Vector2) { 50, 50 }, GRAY);
            int count = 5;
            int spacing = 5;
            for (int x = -count * spacing; x <= count * spacing; x += spacing) for (int z = -count * spacing; z <= count * spacing; z += spacing) DrawCube((Vector3) { (float)x, 1.5f, (float)z }, 1, 1, 1, (Color){255-(x*25),255-(z * 25),255 - (z+x * 12),255});
            DrawCube((*Player).cam.position, 1, 1, 1, RED);

        EndMode3D();

        DrawRectangle(0, 0, (*Player).screenWidth, 40, Fade(RAYWHITE, 0.8f));
        DrawText("preAlpha", (*Player).screenWidth - MeasureText("preAlpha", 20), 10, 20, BLACK);
        DrawFPS(0, 10);

    EndTextureMode();
    
    BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureRec((*Player).screen.texture, (Rectangle) {0.0f, 0.0f, (float)(*Player).screenWidth, (float)-(*Player).screenHeight}, (Vector2) { 0, 0 }, WHITE);
    EndDrawing();
}
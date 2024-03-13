#include <cstdio>
#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 850;

    InitWindow(screenWidth, screenHeight, "raylib [shaders] example - texture drawing");

    SetTraceLogLevel(LOG_ALL);

    Image imBlank = GenImageColor(screenWidth, screenHeight, BLANK);
    Texture2D texture = LoadTextureFromImage(imBlank);  // Load blank texture to fill on shader
    UnloadImage(imBlank);

    Shader shader = LoadShader(0, TextFormat("shaders/light.glsl", GLSL_VERSION));
    Vector2 resolution = {screenWidth, screenHeight};
    Vector2 mouse;
    float time = 0.0f;

    int resolutionLoc = GetShaderLocation(shader, "u_resolution");
    int mouseLoc = GetShaderLocation(shader, "u_mouse");
    int timeLoc = GetShaderLocation(shader, "u_time");

    SetShaderValue(shader, resolutionLoc, &resolution, UNIFORM_VEC2);
    SetShaderValue(shader, mouseLoc, &mouse, UNIFORM_VEC2);
    SetShaderValue(shader, timeLoc, &time, UNIFORM_FLOAT);

    SetTargetFPS(600);

    // Track the file modification time
    int lastModTime = GetFileModTime("shaders/light.glsl");

    // Main game loop
    int counter = 0;

    while (!WindowShouldClose())
    {
        // Update
        time = (float)GetTime();
        mouse = GetMousePosition();

        SetShaderValue(shader, timeLoc, &time, UNIFORM_FLOAT);
        SetShaderValue(shader, resolutionLoc, &resolution, UNIFORM_VEC2);
        SetShaderValue(shader, mouseLoc, &mouse, UNIFORM_VEC2);

        // Draw
        BeginDrawing();
        BeginShaderMode(shader);
        DrawTexture(texture, 0, 0, WHITE);
        EndShaderMode();

        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, MAROON);

        EndDrawing();

        if (IsKeyPressed(KEY_R)) {
            //int currentModTime = GetFileModTime("shaders/light.glsl");
            //if (currentModTime > lastModTime || IsKeyPressed(KEY_R) == 'r') {
                // File has been modified, reload the shader
                UnloadShader(shader);
                shader = LoadShader(0, "shaders/light.glsl");
                resolutionLoc = GetShaderLocation(shader, "u_resolution");
                mouseLoc = GetShaderLocation(shader, "u_mouse");
                timeLoc = GetShaderLocation(shader, "u_time");

                SetShaderValue(shader, resolutionLoc, &resolution, UNIFORM_VEC2);
                SetShaderValue(shader, mouseLoc, &mouse, UNIFORM_VEC2);
                SetShaderValue(shader, timeLoc, &time, UNIFORM_FLOAT);

                //lastModTime = currentModTime;
            //}

            fflush(stdout);
        }

        counter++;
    }

    // De-Initialization
    UnloadShader(shader);
    CloseWindow();

    return 0;
}

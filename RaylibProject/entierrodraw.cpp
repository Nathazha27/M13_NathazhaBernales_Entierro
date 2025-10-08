#include "raylib.h"
#include "entierrodraw.h"
#include <map>
#include<string>
using namespace std;
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

map<char, Model> models;

int Entierro_draw(int level_h, int level_w, map<char, string>textures_files, char** level_floor)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = Vector3 { 0.0f, 10.0f, 10.0f };  // Camera position
    camera.target = Vector3 { 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3 { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    float cubeSize = 1.0f;

    for (map<char, string>::iterator texture = textures_files.begin(); texture != textures_files.end(); ++texture) {
        Texture2D tmp = LoadTexture(texture->second.c_str());

        Mesh mesh = GenMeshCube(cubeSize, cubeSize, cubeSize);
        Model model = LoadModelFromMesh(mesh);
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tmp;
        models.insert({texture->first, model});
    }

    float offset = 0.0f;
    if (level_h % 2 == 0) {
        offset = 0.5f;
    }
    float initPosZ = level_h/2 * -1 + offset;
    if (level_h % 2 != 0) {
        offset = 0.0f;
    }
    float initPosX = level_w/2 * -1 + offset;
    Vector3 cubePosition = { initPosX, offset, initPosZ };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        for (int i = 0; i < level_h; i++) {
            for (int j = 0; j < level_w; j++) {
                /*DrawCube(cubePosition, cubeSize, cubeSize, cubeSize, RED);
                DrawCubeWires(cubePosition, cubeSize, cubeSize, cubeSize, GREEN);*/
                if (level_floor[i][j] != 0) {
                }
                cubePosition.x += cubeSize;
            }
            cubePosition.x = initPosX;
            cubePosition.z += cubeSize;
        }

        cubePosition = { initPosX, offset, initPosZ };

        DrawGrid(10, 1.0f);

        EndMode3D();

        DrawText("Welcome to the third dimension!", 10, 40, 20, DARKGRAY);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
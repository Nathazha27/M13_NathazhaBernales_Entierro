#include "raylib.h"
#include "entierrodraw.h"
#include <map>
#include<string>
#include<iostream>
using namespace std;

map<char, Model> models;

int Entierro_draw(int level_h, int level_w, map<char, string>textures_files, char** level_floor, char** level_stage, char** level_objects)
{
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

    for (map<char, string>::iterator it = textures_files.begin(); it != textures_files.end(); ++it) {
        Texture2D texture = LoadTexture(it->second.c_str());
        if (texture.id == 0) {
            cout << "Error: No se ha cargado la textura: " << it->second << endl;
        }
        else {
            cout << "Textura cargada correctamente: " << it->second << endl;
        }

        Mesh mesh = GenMeshCube(cubeSize, cubeSize, cubeSize);
        Model model = LoadModelFromMesh(mesh);
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
        models.insert({ it->first, model});
    }

    float offset = 0.0f;
    if (level_h % 2 == 0) {
        offset = 0.5f;
    }
    float initPosZ = level_h/2 * -1 + offset;
    if (level_w % 2 != 0) {
        offset = 0.0f;
    }

    float initPosX = level_w/2 * -1 + offset;
    Vector3 floorPos = { initPosX, offset, initPosZ };
    Vector3 stagePos;

    Vector3 playerPos;
    const char player = '@';

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            if (level_objects[i][j] == player) {
                playerPos = { initPosX + j * cubeSize, offset + cubeSize, initPosZ + i * cubeSize};
            }
        }
    }

    float speed = 0.05f;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsKeyDown(KEY_W)) playerPos.z -= speed;
        else if (IsKeyDown(KEY_S)) playerPos.z += speed;
        else if (IsKeyDown(KEY_A)) playerPos.x -= speed;
        else if (IsKeyDown(KEY_D)) playerPos.x += speed;

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        for (int i = 0; i < level_h; i++) {
            for (int j = 0; j < level_w; j++) {
                if (level_floor[i][j] != 0) {
                    DrawModel(models[level_floor[i][j]], floorPos, cubeSize, WHITE);
                }
                stagePos = { floorPos.x, floorPos.y + cubeSize, floorPos.z }; 
                if (level_stage[i][j] != 0) {
                    DrawModel(models[level_stage[i][j]], stagePos, cubeSize, WHITE);
                }
                if (level_objects[i][j] != player && level_objects[i][j] != 0) {
                    DrawModel(models[level_objects[i][j]], stagePos, cubeSize, WHITE);
                }
                floorPos.x += cubeSize;
            }
            floorPos.x = initPosX;
            floorPos.z += cubeSize;
        }

        DrawSphere(playerPos, 0.5f, RED);

        floorPos = { initPosX, offset, initPosZ };

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
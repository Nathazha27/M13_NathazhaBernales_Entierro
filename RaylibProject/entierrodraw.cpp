#include "raylib.h"
#include "entierrodraw.h"
#include <map>
#include<string>
#include<iostream>
using namespace std;

map<char, Model> models;

float initPosX;
float initPosZ;

const char player = '@';
const char collision = 'X';
const char coffin = 'C';

int quantCoffins = 0;

bool CheckCollision(char** level_collisions, float x, float y) {
    int pos_x = x - initPosX;
    int pos_y = y - initPosZ;

    if (level_collisions[pos_y][pos_x] == collision) {
        return true;
    }
    return false;
}

bool CheckCoffin(char** level_objects, float x, float y) {
    int pos_x = x - initPosX;
    int pos_y = y - initPosZ;

    if (level_objects[pos_y][pos_x] != '0' && level_objects[pos_y][pos_x] != player) {
        return true;
    }
        return false;
}

bool MoveCoffin(char** level_objects, char** level_collisions,char** level_floor, float first_x, float first_y, float x, float y) {
    int first_pos_x = first_x - initPosX;
    int first_pos_y = first_y - initPosZ;

    int pos_x = x - initPosX;
    int pos_y = y - initPosZ;

    if (level_objects[pos_y][pos_x] != coffin) {
        level_objects[first_pos_y][first_pos_x] = '0';
        if (level_collisions[pos_y][pos_x] != '0' && level_collisions[pos_y][pos_x] != collision) {
            level_objects[pos_y][pos_x] = '0';
            level_floor[pos_y][pos_x] = coffin;
        }
        else {
            level_objects[pos_y][pos_x] = coffin;
        }
        return true;
    }
    return false;
}

int Entierro_draw(int level_h, int level_w, map<char, string>textures_files, char** level_floor, char** level_stage, char** level_objects, char** level_collisions)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = Vector3 { 0.0f, 10.0f, 5.0f };  // Camera position
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
    initPosZ = float(level_h)/2 * -1 + offset;
    if (level_w % 2 != 0) {
        offset = 0.0f;
    }

    initPosX = float(level_w)/2 * -1 + offset;
    Vector3 floorPos = { initPosX, offset, initPosZ };
    Vector3 stagePos;

    Vector3 playerPos;

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            if (level_objects[i][j] == player) {
                playerPos = { initPosX + j * cubeSize, offset + cubeSize, initPosZ + i * cubeSize};
            }
        }
    }

    bool win = false;

    float temp_x;
    float temp_z;

    float temp_coffin_x;
    float temp_coffin_z;

    float coffinplace = cubeSize * 2;
    string coffins = "Coffins out: " + to_string(quantCoffins);

    int time;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        temp_x = playerPos.x;
        temp_z = playerPos.z;

        temp_coffin_x = playerPos.x;
        temp_coffin_z = playerPos.z;

        if (IsKeyPressed(KEY_W)) {
             temp_z -= cubeSize;
             temp_coffin_z -= coffinplace;
        }
        else if (IsKeyPressed(KEY_S)) {
            temp_z += cubeSize;
            temp_coffin_z += coffinplace;
        }
        else if (IsKeyPressed(KEY_A)) {
            temp_x -= cubeSize;
            temp_coffin_x -= coffinplace;
        }
        else if (IsKeyPressed(KEY_D)) {
            temp_x += cubeSize;
            temp_coffin_x += coffinplace;
        }

        if (!CheckCollision(level_collisions, temp_x, temp_z)) {
            if (CheckCoffin(level_objects, temp_x, temp_z)) {
                if (!CheckCollision(level_collisions, temp_coffin_x, temp_coffin_z)) {
                    if (MoveCoffin(level_objects, level_collisions, level_floor, temp_x, temp_z, temp_coffin_x, temp_coffin_z)) {
                        playerPos.x = temp_x;
                        playerPos.z = temp_z;
                    }
                }
            }
            else {
                playerPos.x = temp_x;
                playerPos.z = temp_z;
            }
        }
        else {
            temp_z = playerPos.z;
            temp_x = playerPos.x;

            temp_coffin_z = playerPos.z;
            temp_coffin_x = playerPos.x;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        for (int i = 0; i < level_h; i++) {
            for (int j = 0; j < level_w; j++) {
                if (level_floor[i][j] != '0') {
                    DrawModel(models[level_floor[i][j]], floorPos, cubeSize, WHITE);
                }
                stagePos = { floorPos.x, floorPos.y + cubeSize, floorPos.z }; 
                if (level_stage[i][j] != '0') {
                    DrawModel(models[level_stage[i][j]], stagePos, cubeSize, WHITE);
                }
                if (level_objects[i][j] != player && level_objects[i][j] != '0') {
                    quantCoffins++;
                    DrawModel(models[level_objects[i][j]], stagePos, cubeSize, WHITE);
                }
                floorPos.x += cubeSize;
            }
            floorPos.x = initPosX;
            floorPos.z += cubeSize;
        }
        coffins = "Coffins out: " + to_string(quantCoffins);

        DrawSphere(playerPos, 0.5f, RED);

        floorPos = { initPosX, offset, initPosZ };

        DrawGrid(10, 1.0f);

        EndMode3D();

        if (quantCoffins == 0) {
            if (!win) {
                time = GetTime();
            }
            string timeText = to_string(time);
            DrawText("OLE", screenWidth/2, screenHeight/2, 50, WHITE);
            DrawText(timeText.c_str(), screenWidth / 2, screenHeight/2 + 50, 40, WHITE);
            win = true;
        }

        quantCoffins = 0;

        DrawText(coffins.c_str(), 10, 40, 20, DARKGRAY);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //-----------------------------------------------------------------------------------

    return 0;
}


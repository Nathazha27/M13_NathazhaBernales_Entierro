#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>

#include "raylib.h"
#include "entierrodraw.h"

using namespace std;

float minversion = 0.3f;
float actualversion = 0.5f;
float version = -1.f;

string title = "";

int w_minsize = 2;
int h_minsize = 2;
int level_w = 0;
int level_h = 0;

int numtextures = 0;
map<char, string> textures_files;

int main()
{
    string tmp;

    ifstream level_file("first_level.erro");

    if (!level_file.is_open()) {
        cout << "ERROR 1: El archivo no existe" << endl;
        return 1;
    }

    getline(level_file, tmp, ';');

    if (tmp != "ERRO") {
        cout << "ERROR 2: El archivo no es de formato 'Erro'" << endl;
        return 2;
    }

    getline(level_file, tmp, ';');
    version = stof(tmp);

    if (version > actualversion || version < minversion) {
        cout << "ERROR 3: Version incompatible" << endl;
        return 3;
    }

    getline(level_file, tmp, '\n'); //Con esto nos situamos en la siguiente linea
    getline(level_file, tmp, ';');

    if (tmp != "TITLE") {
        cout << "ERROR 4: Error al leer el titulo" << endl;
        return 4;
    }

    getline(level_file, tmp, ';');

    if (tmp == "") {
        cout << "ERROR 5: El nivel no tiene ningun titulo" << endl;
        return 5;
    }

    title = tmp;

    cout << title << endl;

    getline(level_file, tmp, '\n');
    getline(level_file, tmp, ';');

    if (tmp != "SIZE") {
        cout << "ERROR 6: Error al leer el tamaño del nivel" << endl;
        return 6;
    }

    getline(level_file, tmp, ';');
    level_w = stoi(tmp);

    getline(level_file, tmp, ';');
    level_h = stoi(tmp);

    if (level_h < h_minsize || level_w < w_minsize) {
        cout << "ERROR 7: El alto o el ancho es demasiado pequeño" << endl;
        return 7;
    }

    getline(level_file, tmp, '\n');
    getline(level_file, tmp, ';');

    if (tmp != "TEXTURES") {
        cout << "ERROR 8: Error al leer texturas" << endl;
        return 8;
    }

    getline(level_file, tmp, ';');
    numtextures = stoi(tmp);

    if (numtextures < 1) {
        cout << "ERROR 9: No hay texturas" << endl;
        return 9;
    }

    getline(level_file, tmp, '\n');

    string tmp_key_s;
    char tmp_key;
    string tmp_value;

    for (int i = 0; i < numtextures; i++) {
        getline(level_file, tmp_key_s, ';');
        cout << tmp_key_s << endl;
        if (tmp_key_s == "") {
            cout << "ERROR 10: Una textura no tiene clave" << endl;
            return 10;
        }
        tmp_key = tmp_key_s[0];
        getline(level_file, tmp_value, ';');
        if (tmp_value == "") {
            cout << "ERROR 11: No hay textura" << endl;
            return 11;
        }
        textures_files.insert({ tmp_key, tmp_value });
        getline(level_file, tmp, '\n');
    }

    getline(level_file, tmp, ';');

    if (tmp != "FLOOR") {
        cout << "ERROR 12: Error al acceder al floor" << endl;
        return 12;
    }

    getline(level_file, tmp, '\n');

    char** floor = new char* [level_h];
    for (int i = 0; i < level_h; i++) {
        floor[i] = new char[level_w];
    }

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            getline(level_file, tmp, ';');
            floor[i][j] = tmp[0];
            if (j == level_w - 1) {
                getline(level_file, tmp, '\n');
            }
        }
    }

    cout << endl;

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            cout << floor[i][j] << " ";
            if (j == level_w - 1) {
                cout << endl;
            }
        }
    }

    getline(level_file, tmp, ';');

    if (tmp != "STAGE") {
        cout << "ERROR 13: Error al acceder al stage" << endl;
        return 13;
    }

    getline(level_file, tmp, '\n');

    char** stage = new char* [level_h];
    for (int i = 0; i < level_h; i++) {
        stage[i] = new char[level_w];
    }

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            getline(level_file, tmp, ';');
            stage[i][j] = tmp[0];
            if (j == level_w - 1) {
                getline(level_file, tmp, '\n');
            }
        }
    }

    cout << endl;

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            cout << stage[i][j] << " ";
            if (j == level_w - 1) {
                cout << endl;
            }
        }
    }

    getline(level_file, tmp, ';');

    if (tmp != "COLLISIONS") {
        cout << "ERROR 14: Error al acceder a las colisiones" << endl;
        return 14;
    }

    getline(level_file, tmp, '\n');

    char** collisions = new char* [level_h];
    for (int i = 0; i < level_h; i++) {
        collisions[i] = new char[level_w];
    }

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            getline(level_file, tmp, ';');
            collisions[i][j] = tmp[0];
            if (j == level_w - 1) {
                getline(level_file, tmp, '\n');
            }
        }
    }

    cout << endl;

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            cout << collisions[i][j] << " ";
            if (j == level_w - 1) {
                cout << endl;
            }
        }
    }

    getline(level_file, tmp, ';');

    if (tmp != "OBJECTS") {
        cout << "ERROR 15: Error al acceder a los objetos" << endl;
        return 15;
    }

    getline(level_file, tmp, '\n');

    char** objects = new char* [level_h];
    for (int i = 0; i < level_h; i++) {
        objects[i] = new char[level_w];
    }

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            getline(level_file, tmp, ';');
            objects[i][j] = tmp[0];
            if (j == level_w - 1) {
                getline(level_file, tmp, '\n');
            }
        }
    }

    cout << endl;

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            cout << objects[i][j] << " ";
            if (j == level_w - 1) {
                cout << endl;
            }
        }
    }

    level_file.close();

    Entierro_draw(level_h, level_w, textures_files, floor);

    return 0;
}

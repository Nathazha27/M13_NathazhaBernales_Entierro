#include <iostream>
#include <string>
#include <fstream>
#include <map>

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
map<char, string> textures;

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
        cout << "ERROR 2: El archivo no es de formato 'Enrro'" << endl;
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
        textures.insert({ tmp_key, tmp_value });
        getline(level_file, tmp, '\n');
    }

    level_file.close();

    return 0;
}



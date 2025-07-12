#include "captura.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

void cargarUsuarios(const string& nombre_archivo, unordered_map<long long, Perfil>& usuarios) {
    ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << nombre_archivo << endl;
        return;
    }

    string linea;
    getline(archivo, linea); // Saltar encabezado

    auto inicio = high_resolution_clock::now();

    long long suma_largo_nombres = 0;
    long long cantidad_usuarios = 0;

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        Perfil perfil;

        getline(ss, campo, ';'); // User_ID
        perfil.User_ID = stoll(campo);

        getline(ss, perfil.User_Name, ';'); // User_Name
        suma_largo_nombres += perfil.User_Name.length();

        getline(ss, campo, ';'); // Followers_Count
        perfil.Followers_Count = stoi(campo);

        getline(ss, campo, ';'); // Number_Tweets (se ignora)
        getline(ss, campo, ';'); // Friends_Count
        perfil.Friends_Count = stoi(campo);

        getline(ss, campo, ';'); // Created At (se ignora)

        perfil.CFC = perfil.User_ID;

        usuarios[perfil.User_ID] = perfil;
        cantidad_usuarios++;
    }

    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio).count();

    cout << "Tiempo de carga del archivo: " << duracion << " ms" << endl;

    // Cálculo del largo promedio de los nombres de usuario
    double promedio_largo = static_cast<double>(suma_largo_nombres) / cantidad_usuarios;
    cout << "Promedio de largo de los User_Name: " << promedio_largo << " caracteres" << endl;

}


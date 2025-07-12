#include "captura.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <vector>

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

    //long long suma_largo_nombres = 0;
    //long long cantidad_usuarios = 0;

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        Perfil perfil;

        getline(ss, campo, ';'); // User_ID
        perfil.User_ID = stoll(campo);

        getline(ss, perfil.User_Name, ';'); // User_Name
        //suma_largo_nombres += perfil.User_Name.length();

        getline(ss, campo, ';'); // Followers_Count
        perfil.Followers_Count = stoi(campo);

        getline(ss, campo, ';'); // Number_Tweets (se ignora)
        getline(ss, campo, ';'); // Friends_Count
        perfil.Friends_Count = stoi(campo);

        getline(ss, campo, ';'); // Created At (se ignora)

        perfil.CFC = perfil.User_ID;

        usuarios[perfil.User_ID] = perfil;
        //cantidad_usuarios++;
    }

    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio).count();

    cout << "Tiempo de carga del archivo: " << duracion << " ms" << endl;

    // Cálculo del largo promedio de los nombres de usuario
    //double promedio_largo = static_cast<double>(suma_largo_nombres) / cantidad_usuarios;
    //cout << "Promedio de largo de los User_Name: " << promedio_largo << " caracteres" << endl;

}

void cargarConexiones(const string& nombre_archivo,
                      const unordered_map<long long, Perfil>& usuarios,
                      unordered_map<long long, vector<long long>>& adyacencia) {
    ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << nombre_archivo << endl;
        return;
    }

    string linea;
    getline(archivo, linea); // Saltar encabezado

    vector<pair<string, string>> conexiones_crudas;

    auto inicio_lectura = high_resolution_clock::now();

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string origen, destino;

        getline(ss, origen, ';');
        getline(ss, destino, ';');

        conexiones_crudas.emplace_back(origen, destino);
    }

    auto fin_lectura = high_resolution_clock::now();

    unordered_map<string, long long> nombre_a_id;
    for (const auto& [id, perfil] : usuarios) {
        nombre_a_id[perfil.User_Name] = id;
    }

    auto inicio_conversion = high_resolution_clock::now();

    int perdidas = 0;
    for (const auto& [nombre_origen, nombre_destino] : conexiones_crudas) {
        if (nombre_a_id.count(nombre_origen) && nombre_a_id.count(nombre_destino)) {
            long long id_origen = nombre_a_id[nombre_origen];
            long long id_destino = nombre_a_id[nombre_destino];
            adyacencia[id_origen].push_back(id_destino);
        } else {
            perdidas++;
        }
    }

    auto fin_conversion = high_resolution_clock::now();

    cout << "Tiempo de lectura de conexiones: "
         << duration_cast<milliseconds>(fin_lectura - inicio_lectura).count()
         << " ms" << endl;

    cout << "Tiempo de conversión de User_Name a User_ID: "
         << duration_cast<milliseconds>(fin_conversion - inicio_conversion).count()
         << " ms" << endl;

    cout << "Total de conexiones perdidas por falta de usuarios: " << perdidas << endl;
}

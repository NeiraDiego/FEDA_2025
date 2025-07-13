#ifndef CAPTURA_H
#define CAPTURA_H

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
// Definición del struct para los usuarios
struct Perfil {
    long long User_ID;
    string User_Name;
    int Friends_Count;
    int Followers_Count;
    long long CFC; // Inicialmente igual al User_ID
};

// Función para leer los datos desde el archivo CSV
void cargarUsuarios(const string& nombre_archivo, unordered_map<long long, Perfil>& usuarios);

// Carga de conexiones (nombre → ID)
void cargarConexiones(const string& nombre_archivo,
                      const unordered_map<long long, Perfil>& usuarios,
                      unordered_map<long long, vector<long long>>& adyacencia);


#endif

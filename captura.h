#ifndef CAPTURA_H
#define CAPTURA_H

#include <string>
#include <unordered_map>
#include <vector>

// Definición del struct para los usuarios
struct Perfil {
    long long User_ID;
    std::string User_Name;
    int Friends_Count;
    int Followers_Count;
    long long CFC; // Inicialmente igual al User_ID
};

// Función para leer los datos desde el archivo CSV
void cargarUsuarios(const std::string& nombre_archivo, std::unordered_map<long long, Perfil>& usuarios);

// Carga de conexiones (nombre → ID)
void cargarConexiones(const std::string& nombre_archivo,
                      const std::unordered_map<long long, Perfil>& usuarios,
                      std::unordered_map<long long, std::vector<long long>>& adyacencia);


#endif

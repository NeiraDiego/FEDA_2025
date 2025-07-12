#include "captura.h"
#include "calculos.h"
#include "salidas.h"

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {
    unordered_map<long long, Perfil> usuarios;
    unordered_map<long long, vector<long long>> adyacencia;


// miniprueba con 8 usuarios y dos CFC
//    cargarUsuarios("twitter_users_prueba.csv", usuarios);
//    cargarConexiones("twitter_connections_prueba.csv", usuarios, adyacencia);

    cargarUsuarios("twitter_users_prueba.csv", usuarios);
    cargarConexiones("twitter_connections_prueba.csv", usuarios, adyacencia);


// ######## prueba impresio 100 usuarios #####
/*
    cout << "\nPrimeros 100 usuarios:\n";
    int count = 0;
    for (const auto& [id, perfil] : usuarios) {
        cout << "ID: " << perfil.User_ID
             << ", Nombre: " << perfil.User_Name
             << ", Followers: " << perfil.Followers_Count
             << ", Friends: " << perfil.Friends_Count << '\n';
        if (++count >= 100) break;
    }
*/

    vector<CFC> componentes;
    encontrarCFCs(adyacencia, usuarios, componentes);

    guardarUsuariosRelevantes(usuarios, "usuarios_relevantes.csv");
  
    return 0;
}

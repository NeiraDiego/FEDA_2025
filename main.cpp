#include "calculos.h"
#include "captura.h"
#include "salidas.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>
#include <iostream>
#include <chrono>
#include <queue>

using namespace std;

int main() {
unordered_map<long long, Perfil> usuarios;
    unordered_map<long long, vector<long long>> adyacencia;


// miniprueba con 8 usuarios y dos CFC
//    cargarUsuarios("twitter_users_prueba.csv", usuarios);
//    cargarConexiones("twitter_connections_prueba.csv", usuarios, adyacencia);

    cargarUsuarios("twitter_users.csv", usuarios);
    cargarConexiones("twitter_connections.csv", usuarios, adyacencia);


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

    unordered_map<long long, string> medios = {
        {7668952, "izquierda"},     // Cooperativa
        {12815132, "libertario"},   // El Mostrador
        {239534277, "derecha"},     // Soy Valdivia
        {3222731, "centro"}         // La Tercera
    };

auto porcentajes = calcularIdeologiaPorDistancia(1330680795799953408, adyacencia, medios);

cout << "Ideología (por distancia):\n";
for (const auto& [ideologia, valor] : porcentajes) {
    cout << ideologia << ": " << valor << "%\n";
}

  
    return 0;
}

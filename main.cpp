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


    cout << "Calculando ideologías directas...\n";

    auto inicio_ideologia = chrono::high_resolution_clock::now();
    unordered_map<long long, unordered_map<string, float>> ideologias_directas;

    for (const auto& [id, perfil] : usuarios) {
       ideologias_directas[id] = calcularIdeologiaPorDistancia(id, adyacencia, medios);
    }

    auto fin_ideologia = chrono::high_resolution_clock::now();
    auto tiempo_ideologia = chrono::duration_cast<chrono::milliseconds>(fin_ideologia - inicio_ideologia).count();

    cout << "Tiempo total en calcular ideologías directas: " << tiempo_ideologia << " ms\n";


    cout << "Calculando tendencias ideológicas de las CFCs...\n";

    auto inicio_tendencia = chrono::high_resolution_clock::now();

    calcularTendenciaDeCFCs(componentes, ideologias_directas);

    auto fin_tendencia = chrono::high_resolution_clock::now();
    auto tiempo_tendencia = chrono::duration_cast<chrono::milliseconds>(fin_tendencia - inicio_tendencia).count();

    cout << "Tiempo total en calcular tendencias ideológicas de las CFCs: "
         << tiempo_tendencia << " ms\n";


    cout << "Calculando ideologías en contexto...\n";
    auto inicio_ctx = chrono::high_resolution_clock::now();

    unordered_map<long long, unordered_map<string, pair<float, float>>> ideologia_contextual;
    calcularIdeologiaContextualDeUsuarios(usuarios, componentes, ideologias_directas, ideologia_contextual);

    auto fin_ctx = chrono::high_resolution_clock::now();
    auto tiempo_ctx = chrono::duration_cast<chrono::milliseconds>(fin_ctx - inicio_ctx).count();

    cout << "Tiempo total en calcular ideología contextual: " << tiempo_ctx << " ms\n";

  
    return 0;
}

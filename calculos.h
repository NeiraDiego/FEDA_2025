#ifndef CALCULOS_H
#define CALCULOS_H

#include "captura.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

using namespace std;

// Estructura para representar una Componente Fuertemente Conexa (CFC)
struct CFC {
    long long id;
    unordered_set<long long> miembros;
    float porcentaje_izquierda = 0.0;
    float porcentaje_libertario = 0.0;
    float porcentaje_derecha = 0.0;
    float porcentaje_centro = 0.0;
    unordered_set<long long> top_influyentes;
};

// Kosaraju: encuentra todas las CFCs
void encontrarCFCs(const unordered_map<long long, vector<long long>>& grafo,
                   unordered_map<long long, Perfil>& usuarios,
                   vector<CFC>& componentes);

// BFS para distancias mínimas
unordered_map<long long, int> bfs_distancias(const unordered_map<long long, vector<long long>>& grafo,
                                                  long long origen);

// Ideología basada en distancia a medios
unordered_map<string, float> calcularIdeologiaPorDistancia(
    long long origen,
    const unordered_map<long long, vector<long long>>& grafo,
    const unordered_map<long long, string>& medios_ideologicos
);

void calcularTendenciaDeCFCs(vector<CFC>& componentes,
                              const unordered_map<long long, unordered_map<string, float>>& ideologias_directas);

void calcularIdeologiaContextualDeUsuarios(
    const unordered_map<long long, Perfil>& usuarios,
    const vector<CFC>& componentes,
    const unordered_map<long long, unordered_map<string, float>>& ideologias_directas,
    unordered_map<long long, unordered_map<string, pair<float, float>>>& ideologia_contextual
);
#endif


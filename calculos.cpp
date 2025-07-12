#include "calculos.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Paso 1: DFS para almacenar orden de finalización
void dfsOrden(long long nodo,
              const unordered_map<long long, vector<long long>>& grafo,
              unordered_set<long long>& visitado,
              stack<long long>& pila) {
    visitado.insert(nodo);

    if (grafo.count(nodo)) {
        for (auto vecino : grafo.at(nodo)) {
            if (!visitado.count(vecino)) {
                dfsOrden(vecino, grafo, visitado, pila);
            }
        }
    }

    pila.push(nodo);
}

// Paso 2: Invertir el grafo
unordered_map<long long, vector<long long>> invertirGrafo(const unordered_map<long long, vector<long long>>& grafo) {
    unordered_map<long long, vector<long long>> invertido;

    for (const auto& [u, vecinos] : grafo) {
        for (auto v : vecinos) {
            invertido[v].push_back(u);
        }
    }

    return invertido;
}

// Paso 3: DFS para encontrar una CFC
void dfsCFC(long long nodo,
            const unordered_map<long long, vector<long long>>& grafo,
            unordered_set<long long>& visitado,
            unordered_set<long long>& miembros) {
    visitado.insert(nodo);
    miembros.insert(nodo);

    if (grafo.count(nodo)) {
        for (auto vecino : grafo.at(nodo)) {
            if (!visitado.count(vecino)) {
                dfsCFC(vecino, grafo, visitado, miembros);
            }
        }
    }
}

// Función principal: encuentra todas las CFCs
void encontrarCFCs(const std::unordered_map<long long, std::vector<long long>>& grafo,
                   std::unordered_map<long long, Perfil>& usuarios,
                   std::vector<CFC>& componentes) {
    auto inicio = high_resolution_clock::now();

    unordered_set<long long> visitado;
    stack<long long> pila;

    // 1. Recorrido DFS para llenar pila con orden de finalización
    for (const auto& [nodo, _] : grafo) {
        if (!visitado.count(nodo)) {
            dfsOrden(nodo, grafo, visitado, pila);
        }
    }

    // 2. Invertir el grafo
    auto grafo_invertido = invertirGrafo(grafo);

    // 3. DFS en grafo invertido para encontrar CFCs
    visitado.clear();
    
    int cantidad_CFC_tam_1 = 0;
    int cantidad_CFC_tam_mayor_1 = 0;
    while (!pila.empty()) {
        long long nodo = pila.top(); pila.pop();

        if (!visitado.count(nodo)) {
            unordered_set<long long> miembros;
            dfsCFC(nodo, grafo_invertido, visitado, miembros);
            CFC componente;
            componente.miembros = miembros;

            // Armar submapa con perfiles
            unordered_map<long long, Perfil> submapa;
            for (long long id : miembros) {
               auto it = usuarios.find(id);
               if (it != usuarios.end()) {
                    submapa[id] = it->second;
               }
            }
            if (miembros.size() == 1) cantidad_CFC_tam_1++;
            else cantidad_CFC_tam_mayor_1++;

           // Elegir ID según el más influyente
            auto top = topKUsuarios(submapa, 1, [](const Perfil& p) {
                return p.Followers_Count;
            });
            if (!top.empty()) {
                componente.id = top[0].User_ID;
            } else {
                componente.id = *miembros.begin(); // fallback
            }

            componentes.push_back(componente);
          }
    }

    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio).count();

    cout << "Tiempo total para encontrar las CFCs: " << duracion << " ms" << endl;

    // Imprimir las 3 primeras CFCs
    cout << "\nPrimeras 3 CFCs encontradas:\n";
    for (int i = 0; i < componentes.size() && i < 3; ++i) {
        const auto& cfc = componentes[i];
        cout << "CFC #" << i+1 << " (ID: " << cfc.id << ", Miembros: " << cfc.miembros.size() << ")\n";
    }
    cout << "\nTotal de CFCs encontradas: " << componentes.size() << endl;
    cout << "CFCs de tamaño 1: " << cantidad_CFC_tam_1 << endl;
    cout << "CFCs de tamaño > 1: " << cantidad_CFC_tam_mayor_1 << endl;

}
 

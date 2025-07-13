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
void encontrarCFCs(const unordered_map<long long, vector<long long>>& grafo,
                   unordered_map<long long, Perfil>& usuarios,
                   vector<CFC>& componentes) {
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

        if (miembros.size() == 1) cantidad_CFC_tam_1++;
        else cantidad_CFC_tam_mayor_1++;

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

        // Obtener hasta 5 más influyentes
        auto top5 = topKUsuarios(submapa, 5, [](const Perfil& p) {
            return p.Followers_Count;
        });

        if (!top5.empty()) {
            componente.id = top5[0].User_ID; // el más influyente
            for (const auto& perfil : top5) {
                componente.top_influyentes.insert(perfil.User_ID);
            }
        } else {
            componente.id = *miembros.begin(); // fallback
        }

        // Actualizar campo CFC de cada miembro si tamaño > 1
        if (miembros.size() > 1) {
            for (long long id : miembros) {
                if (usuarios.count(id)) {
                    // cast necesario si usuarios no es referencia no const
                    const_cast<Perfil&>(usuarios.at(id)).CFC = componente.id;
                }
            }
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

// bfs auxiliar para poder encontrar las tendencias políticas directas
unordered_map<long long, int> bfs_distancias(const unordered_map<long long, vector<long long>>& grafo,
                                             long long origen) {
    unordered_map<long long, int> distancia;
    queue<long long> q;

    distancia[origen] = 0;
    q.push(origen);

    while (!q.empty()) {
        long long actual = q.front(); q.pop();
        int d = distancia[actual];

        if (grafo.count(actual)) {
            for (long long vecino : grafo.at(actual)) {
                if (!distancia.count(vecino)) {
                    distancia[vecino] = d + 1;
                    q.push(vecino);
                }
            }
        }
    }

    return distancia;
}

// cálculo de las ideologías por distancia
unordered_map<string, float> calcularIdeologiaPorDistancia(long long origen,
    const unordered_map<long long, vector<long long>>& grafo,
    const unordered_map<long long, string>& medios_ideologicos) {

    // Realizamos BFS desde el usuario
    auto distancias = bfs_distancias(grafo, origen);

    unordered_map<string, float> resultado;
    float suma = 0.0;

    // Recorremos cada medio conocido
    for (const auto& [medio_id, ideologia] : medios_ideologicos) {
        if (distancias.count(medio_id)) {
            int d = distancias[medio_id];
            float peso = 1.0f / d;
            resultado[ideologia] += peso;
            suma += peso;
        }
    }

    // Normalizamos a porcentaje
    for (auto& [ideologia, valor] : resultado) {
        valor = (valor / suma) * 100.0f;
    }

    return resultado;
}

void calcularTendenciaDeCFCs(vector<CFC>& componentes,
                              const unordered_map<long long, unordered_map<string, float>>& ideologias_directas) {
    for (auto& cfc : componentes) {
        if (cfc.miembros.size() == 1) {
            // CFC de un solo nodo → usar su ideología directa
            long long unico_id = *cfc.miembros.begin();
            if (ideologias_directas.count(unico_id)) {
                const auto& ideologia = ideologias_directas.at(unico_id);
                cfc.porcentaje_izquierda   = ideologia.count("izquierda")   ? ideologia.at("izquierda")   : 0.0f;
                cfc.porcentaje_derecha     = ideologia.count("derecha")     ? ideologia.at("derecha")     : 0.0f;
                cfc.porcentaje_libertario  = ideologia.count("libertario")  ? ideologia.at("libertario")  : 0.0f;
                cfc.porcentaje_centro      = ideologia.count("centro")      ? ideologia.at("centro")      : 0.0f;
            }
        } else {
            // CFC con más de un nodo → promedio de top_influyentes
            float suma_izq = 0, suma_der = 0, suma_lib = 0, suma_centro = 0;
            int cuenta = 0;

            for (long long id : cfc.top_influyentes) {
                if (!ideologias_directas.count(id)) continue;

                const auto& ideologia = ideologias_directas.at(id);
                suma_izq += ideologia.count("izquierda") ? ideologia.at("izquierda") : 0.0f;
                suma_der += ideologia.count("derecha") ? ideologia.at("derecha") : 0.0f;
                suma_lib += ideologia.count("libertario") ? ideologia.at("libertario") : 0.0f;
                suma_centro += ideologia.count("centro") ? ideologia.at("centro") : 0.0f;
                cuenta++;
            }

            if (cuenta > 0) {
                cfc.porcentaje_izquierda   = suma_izq / cuenta;
                cfc.porcentaje_derecha     = suma_der / cuenta;
                cfc.porcentaje_libertario  = suma_lib / cuenta;
                cfc.porcentaje_centro      = suma_centro / cuenta;
            }
        }
    }
}

void calcularIdeologiaContextualDeUsuarios(
    const unordered_map<long long, Perfil>& usuarios,
    const vector<CFC>& componentes,
    const unordered_map<long long, unordered_map<string, float>>& ideologias_directas,
    unordered_map<long long, unordered_map<string, pair<float, float>>>& ideologia_contextual
) {
    // Mapear CFC por id para acceso rápido
    unordered_map<long long, const CFC*> cfc_por_id;
    for (const auto& cfc : componentes) {
        cfc_por_id[cfc.id] = &cfc;
    }

    for (const auto& [id, perfil] : usuarios) {
        if (!ideologias_directas.count(id)) continue;

        const auto& directa = ideologias_directas.at(id);
        auto& contextual = ideologia_contextual[id];

        const CFC* cfc = cfc_por_id.count(perfil.CFC) ? cfc_por_id.at(perfil.CFC) : nullptr;

        if (!cfc || cfc->miembros.size() <= 1) {
            // CFC de un solo usuario: ambos extremos = directa
            for (const auto& [clave, valor] : directa) {
                contextual[clave] = {valor, valor};
            }
        } else {
            // CFC con varios miembros
            for (const string& tendencia : {"izquierda", "derecha", "libertario", "centro"}) {
                float d = directa.count(tendencia) ? directa.at(tendencia) : 0.0f;
                float c = 0.0f;

                if (tendencia == "izquierda") c = cfc->porcentaje_izquierda;
                else if (tendencia == "derecha") c = cfc->porcentaje_derecha;
                else if (tendencia == "libertario") c = cfc->porcentaje_libertario;
                else if (tendencia == "centro") c = cfc->porcentaje_centro;

                contextual[tendencia] = {min(d, c), max(d, c)};
            }
        }
    }
}


#include "salidas.h"
#include "calculos.h"


#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <chrono>

using namespace std;

void guardarUsuariosRelevantes(const unordered_map<long long, Perfil>& usuarios, const string& nombre_salida) {
    ofstream salida(nombre_salida);
    if (!salida.is_open()) {
        cerr << "Error al crear el archivo: " << nombre_salida << endl;
        return;
    }

    // Top 10 por número de seguidores
    auto top_followers = topKUsuarios(usuarios, 10, [](const Perfil& p) {
        return p.Followers_Count;
    });

    salida << "Usuarios Influyentes\n";
    salida << "User_ID;User_Name;Followers_Count\n";
    for (const auto& perfil : top_followers) {
        salida << perfil.User_ID << ";" << perfil.User_Name << ";" << perfil.Followers_Count << "\n";
    }

    salida << "\n";

    // Top 10 por número de amigos
    auto top_friends = topKUsuarios(usuarios, 10, [](const Perfil& p) {
        return p.Friends_Count;
    });

    salida << "Usuarios Influenciables\n";
    salida << "User_ID;User_Name;Friends_Count\n";
    for (const auto& perfil : top_friends) {
        salida << perfil.User_ID << ";" << perfil.User_Name << ";" << perfil.Friends_Count << "\n";
    }

    salida.close();
    cout << "Archivo 'usuarios_relevantes.csv' generado con éxito.\n";
}


vector<Perfil> topKUsuarios(const unordered_map<long long, Perfil>& usuarios,
                            int k,
                            function<int(const Perfil&)> criterio) {
    using Pair = pair<int, Perfil>; // (valor, Perfil)
    auto comp = [](const Pair& a, const Pair& b) { return a.first > b.first; };
    priority_queue<Pair, vector<Pair>, decltype(comp)> min_heap(comp);

    int limite = min(k, static_cast<int>(usuarios.size()));

    for (const auto& [id, perfil] : usuarios) {
        int valor = criterio(perfil);

        if (min_heap.size() < limite) {
            min_heap.emplace(valor, perfil);
        } else if (valor > min_heap.top().first) {
            min_heap.pop();
            min_heap.emplace(valor, perfil);
        }
    }

    vector<Perfil> resultado;
    while (!min_heap.empty()) {
        resultado.push_back(min_heap.top().second);
        min_heap.pop();
    }

    sort(resultado.begin(), resultado.end(), [&](const Perfil& a, const Perfil& b) {
        return criterio(a) > criterio(b);
    });

    return resultado;
}

void exportarIdeologiaDirectaCSV(const unordered_map<long long, Perfil>& usuarios,
                                 const unordered_map<long long, unordered_map<string, float>>& ideologias_directas,
                                 const string& nombre_archivo) {
    cout << "Exportando ideología directa a archivo CSV...\n";
    auto inicio = chrono::high_resolution_clock::now();

    ofstream salida(nombre_archivo);
    if (!salida.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombre_archivo << endl;
        return;
    }

    // Cabecera
    salida << "User_ID;User_Name;CFC;Izquierda;Derecha;Libertario;Centro\n";

    for (const auto& [id, perfil] : usuarios) {
        salida << id << ";" << perfil.User_Name << ";" << perfil.CFC << ";";

        if (ideologias_directas.count(id)) {
            const auto& ideol = ideologias_directas.at(id);
            float izq = ideol.count("izquierda") ? ideol.at("izquierda") : 0.0f;
            float der = ideol.count("derecha") ? ideol.at("derecha") : 0.0f;
            float lib = ideol.count("libertario") ? ideol.at("libertario") : 0.0f;
            float cen = ideol.count("centro") ? ideol.at("centro") : 0.0f;

            salida << izq << ";" << der << ";" << lib << ";" << cen << "\n";
        } else {
            salida << "0;0;0;0\n";
        }
    }

    salida.close();

    auto fin = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio).count();

    cout << "Archivo '" << nombre_archivo << "' generado exitosamente en "
         << duracion << " ms\n";
}

void exportarIdeologiaContextualCSV(
    const unordered_map<long long, Perfil>& usuarios,
    const unordered_map<long long, unordered_map<string, pair<float, float>>>& ideologia_contextual,
    const string& nombre_archivo
) {
    cout << "Exportando ideología contextual a archivo CSV...\n";
    auto inicio = chrono::high_resolution_clock::now();

    ofstream salida(nombre_archivo);
    if (!salida.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombre_archivo << endl;
        return;
    }

    // Cabecera
    salida << "User_ID;User_Name;CFC;"
           << "Izquierda_Min;Izquierda_Max;"
           << "Derecha_Min;Derecha_Max;"
           << "Libertario_Min;Libertario_Max;"
           << "Centro_Min;Centro_Max\n";

    for (const auto& [id, perfil] : usuarios) {
        salida << id << ";" << perfil.User_Name << ";" << perfil.CFC << ";";

        if (ideologia_contextual.count(id)) {
            const auto& ideol = ideologia_contextual.at(id);

            auto get_rango = [&](const string& key) {
                return ideol.count(key) ? ideol.at(key) : make_pair(0.0f, 0.0f);
            };

            auto [izq_min, izq_max] = get_rango("izquierda");
            auto [der_min, der_max] = get_rango("derecha");
            auto [lib_min, lib_max] = get_rango("libertario");
            auto [cen_min, cen_max] = get_rango("centro");

            salida << izq_min << ";" << izq_max << ";"
                   << der_min << ";" << der_max << ";"
                   << lib_min << ";" << lib_max << ";"
                   << cen_min << ";" << cen_max << "\n";
        } else {
            salida << "0;0;0;0;0;0;0;0\n";
        }
    }

    salida.close();

    auto fin = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio).count();

    cout << "Archivo '" << nombre_archivo << "' generado exitosamente en "
         << duracion << " ms\n";
}


void exportarCFCsCSV(const vector<CFC>& componentes,
                     const unordered_map<long long, Perfil>& usuarios,
                     const unordered_map<long long, vector<long long>>& grafo,
                     const string& nombre_archivo) {
    cout << "Exportando resumen de CFCs a archivo CSV...\n";
    auto inicio = chrono::high_resolution_clock::now();

    ofstream salida(nombre_archivo);
    if (!salida.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombre_archivo << endl;
        return;
    }

    // Cabecera
    salida << "CFC_ID;Lider_User_Name;Num_Miembros;"
           << "Izquierda;Derecha;Libertario;Centro;"
           << "Miembros;Arcos\n";

    for (const auto& cfc : componentes) {
        string nombre_lider = usuarios.count(cfc.id) ? usuarios.at(cfc.id).User_Name : "Desconocido";
        salida << cfc.id << ";" << nombre_lider << ";" << cfc.miembros.size() << ";"
               << cfc.porcentaje_izquierda << ";" << cfc.porcentaje_derecha << ";"
               << cfc.porcentaje_libertario << ";" << cfc.porcentaje_centro << ";";

        // Generar lista de miembros
        string miembros;
        for (long long id : cfc.miembros) {
            miembros += to_string(id) + ",";
        }
        if (!miembros.empty()) miembros.pop_back(); // eliminar última coma
        salida << miembros << ";";

        // Generar lista de arcos internos
        string arcos;
        for (long long u : cfc.miembros) {
            if (!grafo.count(u)) continue;
            for (long long v : grafo.at(u)) {
                if (cfc.miembros.count(v)) {
                    arcos += to_string(u) + "→" + to_string(v) + ",";
                }
            }
        }
        if (!arcos.empty()) arcos.pop_back();
        salida << arcos << "\n";
    }

    salida.close();

    auto fin = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio).count();

    cout << "Archivo '" << nombre_archivo << "' generado exitosamente en "
         << duracion << " ms\n";
}


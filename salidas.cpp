#include "salidas.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>

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



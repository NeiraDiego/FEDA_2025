#include "captura.h"

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {
    unordered_map<long long, Perfil> usuarios;
    unordered_map<long long, vector<long long>> adyacencia;

    cargarUsuarios("twitter_users.csv", usuarios);
    cargarConexiones("twitter_connections.csv", usuarios, adyacencia);

    cout << "\nPrimeros 100 usuarios:\n";
    int count = 0;
    for (const auto& [id, perfil] : usuarios) {
        cout << "ID: " << perfil.User_ID
             << ", Nombre: " << perfil.User_Name
             << ", Followers: " << perfil.Followers_Count
             << ", Friends: " << perfil.Friends_Count << '\n';
        if (++count >= 100) break;
    }

    return 0;
}

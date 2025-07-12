#include "captura.h"

#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
    unordered_map<long long, Perfil> usuarios;

    cargarUsuarios("twitter_users.csv", usuarios);

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

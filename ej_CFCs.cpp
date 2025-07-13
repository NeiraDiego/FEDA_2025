#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ifstream archivo_csv("CFCs.csv");
    ofstream salida_txt("CFCs_legibles.txt");

    if (!archivo_csv.is_open()) {
        cerr << "No se pudo abrir el archivo 'CFCs.csv'" << endl;
        return 1;
    }

    if (!salida_txt.is_open()) {
        cerr << "No se pudo crear el archivo 'CFCs_legibles.txt'" << endl;
        return 1;
    }

    string linea;
    getline(archivo_csv, linea); // Saltar cabecera

    while (getline(archivo_csv, linea)) {
        stringstream ss(linea);
        string campo;
        vector<string> campos;

        // Separar por ';'
        while (getline(ss, campo, ';')) {
            campos.push_back(campo);
        }

        if (campos.size() < 9) continue;

        string cfc_id = campos[0];
        string lider = campos[1];
        int num_miembros = stoi(campos[2]);

        // Solo mostrar si tiene más de 1 miembro
        if (num_miembros <= 1) continue;

        salida_txt << "Lider: " << lider << "\n";
        salida_txt << "CFC_ID: " << cfc_id << "\n";
        salida_txt << "Miembros: " << num_miembros << "\n";
        salida_txt << "Ideología Izquierda: " << campos[3] << "%\n";
        salida_txt << "Ideología Derecha: " << campos[4] << "%\n";
        salida_txt << "Ideología Libertario: " << campos[5] << "%\n";
        salida_txt << "Ideología Centro: " << campos[6] << "%\n";
        salida_txt << "Lista de Miembros: " << campos[7] << "\n";
        salida_txt << "Lista de Arcos: " << campos[8] << "\n";
        salida_txt << "\n";
    }

    archivo_csv.close();
    salida_txt.close();

    cout << "Archivo 'CFCs_legibles.txt' generado exitosamente.\n";
    return 0;
}


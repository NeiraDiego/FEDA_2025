
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ifstream archivo_csv("ideologia_contextual.csv");
    ofstream salida_txt("usuarios_legibles.txt");

    if (!archivo_csv.is_open()) {
        cerr << "No se pudo abrir el archivo 'ideologia_contextual.csv'" << endl;
        return 1;
    }

    if (!salida_txt.is_open()) {
        cerr << "No se pudo crear el archivo 'usuarios_legibles.txt'" << endl;
        return 1;
    }

    string linea;
    getline(archivo_csv, linea); // Saltar cabecera

    int contador = 0;

    while (getline(archivo_csv, linea) && contador < 100) {
        stringstream ss(linea);
        string campo;
        vector<string> campos;

        while (getline(ss, campo, ';')) {
            campos.push_back(campo);
        }

        if (campos.size() < 11) continue;

        salida_txt << "User_Name: " << campos[1] << "\n";
        salida_txt << "User_ID: " << campos[0] << "\n";
        salida_txt << "CFC: " << campos[2] << "\n";
        salida_txt << "Izquierda: " << campos[3] << "% - " << campos[4] << "%\n";
        salida_txt << "Derecha: " << campos[5] << "% - " << campos[6] << "%\n";
        salida_txt << "Libertario: " << campos[7] << "% - " << campos[8] << "%\n";
        salida_txt << "Centro: " << campos[9] << "% - " << campos[10] << "%\n";
        salida_txt << "\n";

        contador++;
    }

    archivo_csv.close();
    salida_txt.close();

    cout << "Archivo 'usuarios_legibles.txt' generado con éxito para los primeros 100 usuarios.\n";
    return 0;
}


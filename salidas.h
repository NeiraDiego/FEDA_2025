#ifndef SALIDAS_H
#define SALIDAS_H

#include "captura.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

using namespace std;
// Exporta usuarios influyentes e influenciables al CSV
void guardarUsuariosRelevantes(const unordered_map<long long, Perfil>& usuarios,
                               const string& nombre_salida);

// devuelve los top-k usuarios según un criterio dado
vector<Perfil> topKUsuarios(const unordered_map<long long, Perfil>& usuarios,
                                 int k,
                                 function<int(const Perfil&)> criterio);

#endif



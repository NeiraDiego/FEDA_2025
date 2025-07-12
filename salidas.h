#ifndef SALIDAS_H
#define SALIDAS_H

#include "captura.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

// Exporta usuarios influyentes e influenciables al CSV
void guardarUsuariosRelevantes(const std::unordered_map<long long, Perfil>& usuarios,
                               const std::string& nombre_salida);

// devuelve los top-k usuarios según un criterio dado
std::vector<Perfil> topKUsuarios(const std::unordered_map<long long, Perfil>& usuarios,
                                 int k,
                                 std::function<int(const Perfil&)> criterio);

#endif



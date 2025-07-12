#ifndef CALCULOS_H
#define CALCULOS_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "salidas.h"


// Representación de una Componente Fuertemente Conexa (CFC)
struct CFC {
    long long id;  // user_id del miembro más influyente (por ahora se puede dejar igual a cualquier miembro)
    std::unordered_set<long long> miembros;
    float porcentaje_izquierda = 0.0;
    float porcentaje_derecha = 0.0;
    float porcentaje_libertario = 0.0;
    float porcentaje_centro = 0.0;
};

// Encuentra todas las CFCs usando Kosaraju
void encontrarCFCs(const std::unordered_map<long long, std::vector<long long>>& grafo,
                   const std::unordered_map<long long, Perfil>& usuarios,
                   std::vector<CFC>& componentes);

#endif


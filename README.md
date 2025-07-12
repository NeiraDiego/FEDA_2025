# Datos y estructuras:
## Usuarios (Vertices)
archivo:twitter_users.csv
Contiene una linea de encabezado y luego lineas con 6 campos separados por ";", que corresponden a:
- User_ID
- User_Name
- Followers_Count
- Nomber_Tweets
- Friends_Count
- Created At

### Estructuras:
unordered map clave-> User_ID y un struct para el resto de datos, esto tiene sentido ya que tiene una cantidad fija de campos. 
Analizamos si sería mejor trabajar con una tupla conociendo el orden de los datos pero determinamos que un struct hace mas facil de leer el código, hace que podamos incorporar los campos que ahora estamos ignorando con facilidad en un futuro y no tiene diferencia en el desempeño y ni en el tamaño en memoria significativas:
### Fuente: 
Consulta realizada en chatGPT que entregó las siguientes fuentes:
Codigo: 

``` cpp
#include <tuple>
#include <string>
#include <iostream>

struct Perfil {
    int User_ID;
    std::string User_Name;
    //int num_tweets;
    //int Friends;
    int Followers;
    //std::string Created_At;
};

int main() {
    std::tuple<int, std::string, int, int, int, std::string> t;
    Perfil p;

    std::cout << sizeof(t) << std::endl;
    std::cout << sizeof(p) << std::endl;
}
```


https://en.cppreference.com/w/cpp/utility/tuple.html
https://eel.is/c++draft/class.mem
Prueba: el assembly generado (-O2 o -O3) es el mismo. Puedes verlo en Compiler Explorer.
https://quick-bench.com/ 

### Tratamiento:
- User_ID: verificar si int soporta el mayor valor
- User_Name: necesario para Mostrar en pantalla en lenguaje humano (string)
- Followers_Count: lo mantenemos ya que se necesita para determinar los lideres y utilizaremos int
- Nomber_Tweets: lo desechamos ya que sacrificamos flexibilidad del código en post del tamaño de las estructuras en memoria, somos concientes de que no lo usamos en ninguna parte del análisis. Se podría agregar sin mayor dificultad en caso de ampliar el estudio
- Friends_Count: lo desechamos por la misma razón anterior
- Created At: lo desechamos por la misma razon anterior
- CFC: (Componente fuertemente conexa) es un int y su valor inicial es igual a User_ID

## Conexiones (Aristas)
archivo:twitter_connections.csv
Contiene una linea de encabezado y luego lineas con 2 datos separados por ;, que corresponden a:
- nodo de salida
- nodo de entrada
para cada conexión (de seguimiento unidireccional) entre usuarios
### Estructura: 
2-tuplas creemos que es la estructura simple que requiere el tipo de dato
### Tratamiento: 
debido a la decisión de utilizar los User_ID como clave en el UMap es que deberemos realizar una conversion de las aristas de User_Name->User_Name a User_ID->User_ID para poder relacionarla la estructura Perfiles (más detalles en las discusiones)

## CFS Componentes fuertemente conexas
Es un Struct que almacenará datos de las CFS con un ID Correspondiente al User_ID del miembro más popular, un vector que incluya todos los miembros, en el cuál solo buscaremos presencia por lo que unordered_set es suficiente (primero usabamos un vector pero vimos que las busquedas son mas eficientes en esta estrcutura) y 4 floats que guardarán el porcentaje de cada una de la tendencias políticas que le asignaremos a cada componente. Finalmente usaremos otro unordered_set para almacenar los (hasta 5) miembros más influyentes de la componente.

# implementacion:
## Captura de datos y carga en estructura de datos
### Captura de datos desde archivo usuarios

#### adicional: busqueda de n de caracteres promedio para evaluación del impacto de la modificación del índice

### Captura desde archivo conexiones y modificación

## Busqueda de personas mas influyentes (del dataset)

## Busqueda de componentes fuertemente conexas:
Para realizar la busqueda de componentes fuertemente conexas decidimos utilizar el algoritmo de Kosaraju por su complejidad 
- Hacer un recorrido DFS y acumular en un stack desde el último al primero
- Revertir la dirección de las aristas del grafo
- Recorrer por DFS los nodos no visitados y todos los que son alcanzables son parte de la componente

## Busqueda de personas mas influyentes (de cada componente)
- Seleccionamos con un ciclo for las 5 personas mas influyentes de cada componente, entendiendo por infuyente, que tenga mayor número de seguidores(en caso de que el componente esté compuesto por menos de 5 miembros, todos ellos son considerados los más influyentes) 
- La persona mas influyente determinará el ID de la componente

## Determinación de ideología directa
- Definimos una función dist que con uso de DFS nos permite determinar la distancia de un nodo u a un nodo v
- Definiremos la funcion para ideologia calcular la ideología directa de un nodo con la formula:
    - (hay que incluir que v-v hace que el nodo sea 100 de su propia ideología)
    - Calculamos con la funcion dist, la distancia del nodo a cada uno de los 4 medios y los almacenamos localmente para calcular:
    - % de ideologia directa I = 1 - (Distancia a medio con ideología I/Suma Distancias)

## Definición de la ideología de cada componente
- Definiremos los porcentajes de ideologia de la componente como el promedio de las ideologías de cada uno de los miembros influyentes utilizando la formula anterior

## Determinación de ideología social para un usuario en concreto
- Definimos el porcentaje de la ideología social como un rango de porcentajes para cada una de las cuatro ideologias cuyos limites son:
    - Limite inferior: min(ideología directa, ideoloía de su CFC)
    - Limite superior: max(ideología directa, ideoloía de su CFC)



### Discusiones: 
Fue necesario discriminar si conviene utilizar el User_ID o User_Name como clave ya que usar User_ID hace necesario cambiar cada dato (O(E)) y para cada una realizar una busqueda en el U-Map (O(V)) y reemplazarla (O(1)) y la ventaja se genera por la comparación de strings de un tamaño de 1 byte por character con una cantidad promedio por determinar v/s 4 bytes por ser de typo int (trabajamos con arquitectura de 64 bits). Para el cálculo más grande que puede ser la busqueda de componentes conexas con el algoritmo de Kosaraju (O(E+V)), la diferencia podría no ser significativa ya que el costo de ordenar (O(E)) + el costo del algoritmo suman asintóticamente lo mismo que no ordenar. Por lo que la diferencia se da a nivel de constantes que acompañan a las operaciones y debido a que el ordenamiento se produce una sola vez y realizaremos varias operaciones (no solo la busqueda de componentes conexas) estimamos que esta diferencia será significativa.

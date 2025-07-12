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
### Tratamiento:
- User_ID: verificar si int soporta el mayor valor, lo tuvimos que cambiar por long long
- User_Name: necesario para Mostrar en pantalla en lenguaje humano (string)
- Followers_Count: lo mantenemos ya que se necesita para determinar los lideres de infuencia y utilizaremos int
- Friends_Count: lo mantenemos ya que se necesita para determinar los usuarios mas influenciables y utilizaremos int
- Nomber_Tweets: lo desechamos ya que sacrificamos flexibilidad del código en pos del tamaño de las estructuras en memoria, somos concientes de que no lo usamos en ninguna parte del análisis. Se podría agregar sin mayor dificultad en caso de ampliar el estudio
- Created At: lo desechamos por la misma razon anterior
- CFC: (Componente fuertemente conexa) es un long long y su valor inicial es igual a User_ID

Recordemos que hay cuatro usuarios especiales:
Cooperativa (7668952;Cooperativa)  es reportada como un medio de izquierda
El Mostrador (12815132;elmostrador) es reportado como un medio libertario
Soy Valdivia (239534277;soyvaldiviacl) es reportado como un medio de derecha
La Tercera (3222731;latercera) es reportado como un medio de centro
Y son aquellos a los cuales se buscará la distancia para determinar la ideología 

## Conexiones (Aristas)
archivo:twitter_connections.csv
Contiene una linea de encabezado y luego lineas con 2 datos separados por ;, que corresponden a:
- nodo de entrada
- nodo de salida
para cada conexión (de seguimiento unidireccional) entre usuarios
### Estructura: 
Dado que la operación que mas utilizremos será la busqueda, utilizarémos una lista de adyacencia almacenadas en un unordered_map
### Tratamiento: 
debido a la decisión de utilizar los User_ID como clave en el UMap es que deberemos realizar una conversion de las aristas de User_Name->User_Name a User_ID->User_ID para poder relacionarla la estructura Perfiles (más detalles en las discusiones)

## CFS Componentes fuertemente conexas
Es un Struct que almacenará datos de las CFS con un ID Correspondiente al User_ID del miembro más popular, un vector que incluya todos los miembros, en el cuál solo buscaremos presencia por lo que unordered_set es suficiente (primero usabamos un vector pero vimos que las busquedas son mas eficientes en esta estrcutura) y 4 floats que guardarán el porcentaje de cada una de la tendencias políticas que le asignaremos a cada componente. Finalmente usaremos otro unordered_set para almacenar los (hasta 5) miembros más influyentes de la componente.

# implementacion:
## Captura de datos y carga en estructura de datos
### Captura de datos desde archivo usuarios
Leeremos el archivo twitter_users.csv y organizaremos los datos en el struct definido en la sección anterior. Ingorando por el momento los datos que no utilizaremos.

#### adicional: realizaremos un mini experimento para calcular de numero de caracteres promedio de los User_names para evaluación del impacto de la modificación del índice

### Captura desde archivo conexiones y modificación
capturaremos los datos desde twitter_connections.csv, dado que ya tendremos cargados los datos de los usuarios buscaremos cada uno de los nombres de usuarios y reemplazaremos el valor de v.User_name y de u.User_name para cada arista por v.User_ID y u_User_ID respectivamente 

## Busqueda de personas mas influyentes y mas influenciables (del dataset)
Realizaremos (por requisito del enunciado) un recorrido por todos los usuarios almacenando los 10 usuarios con mayor numero de followers y los 10 con mayor numero de friends, dato que se encuentra directamente almacenado en el struct y lo entregaremos 

## Busqueda de componentes fuertemente conexas:
Para realizar la busqueda de componentes fuertemente conexas decidimos utilizar el algoritmo de Kosaraju por su buen desempeño para digrafos grandes, ya que recorre solo una vez (asintóticamente) cada arista, consiste en: 
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
    - Si no existe camino a alguno de las ideologías la el % es 0 y se ignora para los siguientes calculos
    - Calculamos con la funcion dist, la distancia del nodo a cada uno de los (a lo mas) 4 medios y los almacenamos localmente para calcular:
    - % de ideologia directa I = 1 - (Distancia a medio con ideología I/Suma Distancias)

## Definición de la ideología de cada componente
- Definiremos los porcentajes de ideologia de la componente como el promedio de las ideologías de cada uno de los miembros influyentes utilizando la formula anterior

## Determinación de ideología social para un usuario en concreto
- Definimos el porcentaje de la ideología social como un rango de porcentajes para cada una de las cuatro ideologias cuyos limites son:
    - Limite inferior: min(ideología directa, ideoloía de su CFC)
    - Limite superior: max(ideología directa, ideoloía de su CFC)

## Estructura del programa
Utilizaremos 4 archivos cpp y sus 3 respectivos headers (menos para main.cpp):
- caputra.cpp: archivo con las funciones para captura de datos, su archivo de encabezados contiene el struct de usuarios
- calculos.cpp: código para las funciones que calculan las CFC y las ideologías, su .h incluye el struct de los CFC
- salidas.cpp: archivo para escribir las funciones que entregarán las salidas de forma organizada de la información
- main.cpp archivo para llamar todas las funciones y ejecutar el programa
además generaremos un archivo make para compilar todo

# Experimentación y Discusión: 
## Datos de entrada:
Lo primero que notamos al observar los archivos con los datos fue el tamaño relativo de las aristas con respecto al número de nodos. 
De los datos:
Número de usuarios(V): 41.858
Número arcos totales: 83.072
Grado de entrada de Cooperativa: 1.000
Grado de entrada de Soy Valdivia: 1.000
Grado de entrada de La Tercera: 700
Grado de entrada de El mostrador: 1000
Por lo que si quitamos estos 4 nodos el numero de conexiones queda en 79.272 lo que da un promedio aproximado de 1,8964 conexiones por nodo. Además, por simple observacion hay usuarios que tienen muchos seguidores, por lo que es altamente probable que existan multiples puentes y por lo tanto varias componentes altamente conexas. 
También notamos que el mostrador es el único medio que no sigue a nadie en la muestra y la tercera es sigue a muchos usuarios, los otros dos siguen al menos un usuario.

## Elección de estructuras:
### Usuarios (nodos V):
Antes de definir las estructuras y los algoritmos ya podíamos intuir que las operaciones que mas se repetirían serian la busqueda y el recorrido lineal, es por esto que determinamos que la estructura mas eficiente para almacenar los nodos o usuarios sería un unordered_map.

La siguiente discusión fue la elección de la llave para para el unordered_map. Fue necesario discriminar si convenía utilizar el User_ID o User_Name como clave.
Usar User_ID hacía necesario cambiar cada dato (O(E)) y para cada una realizar una busqueda en el U-Map (O(V)) y reemplazarla (O(1)) y la ventaja se genera por la comparación de strings de un tamaño de 1 byte por character del nombre con una cantidad promedio por determinar v/s 8 bytes por ser de typo long long (trabajamos con arquitectura de 64 bits). 
Para el cálculo más grande que puede ser la busqueda de componentes conexas con el algoritmo de Kosaraju (O(E+V)), la diferencia podría no ser significativa ya que el costo de ordenar (O(E)) + el costo del algoritmo suman asintóticamente lo mismo que no ordenar. Por lo que la diferencia se da a nivel de constantes que acompañan a las operaciones y debido a que el ordenamiento se produce una sola vez y realizaremos varias operaciones (no solo la busqueda de componentes conexas) estimamos que esta diferencia será significativa.

La parte anterior debió ser cambiada ya que originalmente habíamos elegido int, lo que hacía mas facil la comparación, pero al observar los datos (sobre todo los ultimos) encontramos ids que llegan a los 19 digitos por lo que tuvimos que aumentar la capacidad del dato.

Para el resto de los datos, la primera desición que tomamos fue arbitrariamente ignorar los campos de twitts y fecha de creación ya que para todo el analisis de este trabajo no se utilizaron estos datos y no hacía sentido almacenar estos en la RAM mientras no se utilizan. 
Luego, analizamos si sería mejor trabajar con una tupla conociendo el orden de los campos, pero determinamos que un struct hace mas facil de leer y/o corregir el código, y además hace que podamos incorporar mas adelante los campos que ahora estamos ignorando en caso de que queramos ampliar el estudio. Modificando solo el struct, la función de captura y la función donde que queramos incorporar o ampliar.

#### Diferencia de tupla v/s Struct: 
Consulta realizada en chatGPT que entregó las siguientes fuentes:
Codigo: 

``` cpp
#include <tuple>
#include <string>
#include <iostream>

struct Perfil {
    long long User_ID;
    std::string User_Name;
    //int num_tweets;
    int Friends;
    int Followers;
    //std::string Created_At;
    long long CFC;
};

int main() {
    std::tuple<int, std::string, /*int*/, int, int, /*std::string*/> t;
    Perfil p;

    std::cout << sizeof(t) << std::endl;
    std::cout << sizeof(p) << std::endl;
}
```


https://en.cppreference.com/w/cpp/utility/tuple.html
https://eel.is/c++draft/class.mem
Prueba: el assembly generado (-O2 o -O3) es el mismo. Puedes verlo en Compiler Explorer.
https://quick-bench.com/ 

## Cálculos
### Cálculo de CFC:
Como mencionamos antes, decidimos utilizar el algoritmo de Kosaraju por su buen desempeño para digrafos grandes ya que asintóticamente tiene una complejidad temporal de O(V+E) ya que recorre una multiplo fijo (independiente del tamaño de los datos) de veces cada arista y cada vertice.
Una vez que encontramos las componentes fuertemente conexas decidimos que las utilizaríamos para calcular la tendencia de los usuarios. Segun lo conversado en clases las CFC suelen ser interpretadas como comunidades humanas.

Una vez determinado como calcularíamos e interpretaríamos las CFC pasamos a definir como usaríamos la información que el grafo nos entrega para determinar las tendencias políticas de los diferentes usuarios. El primer gran supuesto que hicimos es que las personas que son mas populares en la red social suelen representar de buena manera la forma en que quieren leer los miembros de la comunidad. 
Dado que una sola persona (la mas pupular) podría generar algun sesgo por algún tema incidental, por ejemplo que siga al medio de comunicación opuesto a su ideología decidimos disminuir ese tipo de anomalía estadística tomando una muestra de 5.

De forma análoga a la estructura Union-Find que utilizamos en clases para el algoritmo de Kruscal, utilizaremos un lider para identificar las diferentes CFC, pero en este caso particular utilizaremos el miembro con mayor influencia (número de seguidores) como lider.

### Cálculo de tendencia política:
La primera opción que discutimos es la posibilidad de que las partiuclaridades de cada persona, que puede ser si participan mucho o poco en la red social, si siguen o no personas que piensan diferentes o suelen seguir solo a personas con la misma ideología, son detalles que son inconsistencias a las cuales se les puede disminuir utilizando grupos en lugar de individuos. Por lo que nuestra primera intuición fue utilizar la comunidad a la que pertenecen, es decir, su componente fuertemente conexa para determinar la corriente política del usuario.

Lo siguiente fue reconocer que todos los usuarios son particulares y distintos por lo que debíamos incoroporar las desiciones que tomó cada usuario en su participación el twitter como a qué personas y/o medios de comunicación sigue. En este sentido decidimos incorporar una característica propia de un grafo dirigido que nos entrega mucha información de la relación o cercanía de un nodo con otro, que es la distancia en el gráfico, por lo que decidimos determinar una relacion inversamente proporcional pero relativa (a la todas las distancias). También nos encontramos con el caso particular en que toda la CFC podrían no tener camino a alguno de los medios, para esto incorporamos el caso en que no exista camino definiendo la tendencia como 0, y como el camino sería infinito se deja de considerar para el resto de calculos: matemáticamente esto haría que la tendencía de cada uno de los otros fuera 100% lo que no hace sentido y además elimina la información de como la proporción de las diferentes tendencias políticas.

Elegimos incorporar ambos piezas de información en el cálculo de cada definitivo de las tendencias, la siguiente desición fue determinar como agregaríamos cada dato, vimos dos opciones que nos parecieron razonables, la primera fue establecer un promedio simple entre la tendencia calculada por distancias de usuario y la de su CFC, lo que podría generar porcentajes que sumen mas de 100%, y luego ponderar para llegar a la suma 100. La segunda opción y la que elegimos de forma arbitraria fue entregar, en lugar de un valor un rango de valores, determinados por los limites de las dos opciones que se promediaban en el cálculo anterior. Escogimos esto ya que nos pareció que: por un lado, incorpora la información del cálculo anterior y por otro lado el tamaño del rango nos entrega información adicional que nos parece relevante, rangos mas amplios informan que tan distantes estan las ideologías del usuario de la de su comunidad (CFC)


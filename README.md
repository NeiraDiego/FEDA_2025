# Datos y estructuras:
## Usuarios (Vertices)
archivo:twitter_users.csv

Contiene una linea de encabezado y luego lineas con 6 campos separados por ";", que corresponden a:
- User_ID
- User_Name
- Followers_Count
- Number_Tweets
- Friends_Count
- Created At

### Estructuras:
Como estructura principal para los usuarios decidimos utilizar un unordered map (unordered_map) usando como clave User_ID y un Struct para el resto de datos, Struct tiene sentido ya que tiene una cantidad fija de campos que almacenar y unordered_map ya que la mayoría de las operaciones a realizar será búsqueda.
``` cpp
struct Perfil {
    long long User_ID;
    string User_Name;
    int Friends_Count;
    int Followers_Count;
    long long CFC; // Inicialmente igual al User_ID
};
```

### Tratamiento:
- User_ID: verificar si int soporta el mayor valor, lo tuvimos que cambiar por long long
- User_Name: necesario para Mostrar en pantalla en lenguaje humano (string)
- Followers_Count: lo mantenemos ya que se necesita para determinar los líderes de infuencia y utilizaremos int como tipo de dato
- Friends_Count: lo mantenemos ya que se necesita para determinar los usuarios más influenciables y utilizaremos int como tipo de dato
- Number_Tweets: lo desechamos ya que sacrificamos flexibilidad del código en pos del tamaño de las estructuras en memoria, somos concientes de que no lo usamos en ninguna parte del análisis. Se podría agregar sin mayor dificultad en caso de ampliar el estudio
- Created At: lo desechamos por la misma razón anterior
- CFC: (Componente fuertemente conexa) es un long long y su valor inicial es igual a User_ID

Recordemos que hay cuatro usuarios especiales:
- Cooperativa (7668952;Cooperativa)  es reportada como un medio de izquierda
- El Mostrador (12815132;elmostrador) es reportado como un medio libertario
- Soy Valdivia (239534277;soyvaldiviacl) es reportado como un medio de derecha
- La Tercera (3222731;latercera) es reportado como un medio de centro
Y son aquellos a los cuales se buscará la distancia para determinar la ideología 

## Conexiones (Aristas)
archivo:twitter_connections.csv

Contiene una linea de encabezado y luego lineas con 2 datos separados por ;, que corresponden a:
- nodo de entrada
- nodo de salida
para cada conexión (de seguimiento unidireccional) entre usuarios

### Estructura: 
Dado que la operación que más utilizremos será la búsqueda, utilizarémos una lista de adyacencia almacenadas en un unordered_map

### Tratamiento:

debido a la decisión de utilizar los User_ID como clave en el UMap es que deberemos realizar una conversion de las aristas de User_Name->User_Name a User_ID->User_ID para poder relacionarla la estructura Perfiles (más detalles en las discusiones)

## CFC Componentes fuertemente conexas

Es un Struct que almacenará datos de las CFC con un ID Correspondiente al User_ID del miembro más popular, un vector que incluya todos los miembros, en el cuál solo buscaremos presencia por lo que unordered_set es suficiente (primero usabamos un vector pero vimos que las búsquedas son más eficientes en esta estrcutura) y 4 floats que guardarán el porcentaje de cada una de la tendencias políticas que le asignaremos a cada componente. Finalmente usaremos otro unordered_set para almacenar los (hasta 5) miembros más influyentes de la componente.
``` cpp
struct CFC {
    long long id;
    unordered_set<long long> miembros;
    float porcentaje_izquierda = 0.0;
    float porcentaje_libertario = 0.0;
    float porcentaje_derecha = 0.0;
    float porcentaje_centro = 0.0;
    unordered_set<long long> top_influyentes;
};
```

# implementacion:
## Captura de datos y carga en estructura de datos
### Captura de datos desde archivo usuarios
Leeremos el archivo twitter_users.csv y organizaremos los datos en el struct definido en la sección anterior. Ingorando por el momento los datos que no utilizaremos.

#### adicional: realizaremos un mini experimento para calcular de numero de caracteres promedio de los User_names para evaluar el impacto de la modificación del índice y decidir si es que el cambio de las aristas de strings a números (que finalmente serían long long) valía la pena

### Captura desde archivo conexiones y modificación
capturaremos los datos desde twitter_connections.csv, dado que ya tendremos cargados los datos de los usuarios buscaremos cada uno de los nombres de usuarios y reemplazaremos el valor de v.User_name y de u.User_name para cada arista por v.User_ID y u_User_ID respectivamente. Dado que los usuarios ya se encuentran en una estructura eficiente, unordered_map que entrega un valor esperado constante para búsqueda, la complejidad temporal de este es de O(E)

## Busqueda de personas más influyentes y más influenciables (del dataset)
Realizaremos (por requisito del enunciado) un recorrido por todos los usuarios almacenando los 10 usuarios con mayor numero de followers y los 10 con mayor numero de friends, dato que se encuentra directamente almacenado en el struct y lo entregaremos. Esto tiene una complejidad temporal de O(V)

## Busqueda de componentes fuertemente conexas:
Para realizar la búsqueda de componentes fuertemente conexas decidimos utilizar el algoritmo de Kosaraju por su buen desempeño para digrafos grandes, ya que recorre solo una vez (asintóticamente) cada arista, consiste en: 
- Hacer un recorrido DFS y acumular en un stack desde el último al primero
- Revertir la dirección de las aristas del grafo
- Recorrer por DFS los nodos no visitados y todos los que son alcanzables son parte de una componente y los que no, son su propia componente

Fuente: [https://www.youtube.com/@basicsstrong](https://www.youtube.com/watch?v=QlGuaHT1lzA)

## Busqueda de personas más influyentes (de cada componente)
- Seleccionamos con un ciclo for las 5 personas más influyentes de cada componente, entendiendo por infuyente, que tenga mayor número de seguidores(en caso de que el componente esté compuesto por menos de 5 miembros, todos ellos son considerados los más influyentes) 
- La persona más influyente determinará el ID de la componente

## Determinación de ideología directa
- Definimos una función dist que con uso de DFS nos permite determinar la distancia de un nodo u a un nodo v
- Definiremos la función para ideología calcular la ideología directa de un nodo con la formula:
    - (hay que incluir que v-v hace que el nodo sea 100 de su propia ideología)
    - Si no existe camino a alguno de las ideologías la el % es 0 y se ignora para los siguientes cálculos
    - Calculamos con la función dist, la distancia del nodo a cada uno de los (a lo mas) 4 medios y los almacenamos localmente para calcular:
    - % de ideología directa I = 1 - (Distancia a medio con ideología I/Suma Distancias)

## Definición de la ideología de cada componente
- Definiremos los porcentajes de ideología de la componente como el promedio de las ideologías de cada uno de los miembros influyentes utilizando la formula anterior

## Determinación de ideología social para un usuario en concreto
- Definimos el porcentaje de la ideología social como un rango de porcentajes para cada una de las cuatro ideologías cuyos limites son:
    - Limite inferior: min(ideología directa, ideología de su CFC)
    - Limite superior: max(ideología directa, ideología de su CFC)

## Estructura del programa
Utilizaremos 4 archivos cpp y sus 3 respectivos headers (menos para main.cpp):
- captura.cpp: archivo con las funciones para captura de datos, su archivo de encabezados contiene el struct de usuarios
- cálculos.cpp: código para las funciones que calculan las CFC y las ideologías, su .h incluye el struct de los CFC
- salidas.cpp: archivo para escribir las funciones que entregarán las salidas de forma organizada de la información
- main.cpp: archivo para llamar todas las funciones y ejecutar el programa
- Makefile: además generaremos un archivo make para compilar todo

Además generaremos algunos archivos de código adicionales para generar representaciones de más fácil lectura de los datos obtenidos:
- ej_CFC.cpp
- ej_usuarios.cpp
- ej_relevantes.cpp

# Experimentación y Discusión: 
## Datos de entrada:
Lo primero que notamos al observar los archivos con los datos fue el tamaño relativo de las aristas con respecto al número de nodos. 
De los datos:
- Número de usuarios(V): 41.858
- Número arcos totales: 83.072
- Grado de entrada de Cooperativa: 1.000
- Grado de entrada de Soy Valdivia: 1.000
- Grado de entrada de La Tercera: 700
- Grado de entrada de El mostrador: 1000

Por lo que si quitamos estos 4 nodos el numero de conexiones queda en 79.272 lo que da un promedio aproximado de 1,8964 conexiones por nodo. Además, por simple observacion hay usuarios que tienen muchos seguidores, por lo que es altamente probable que existan multiples puentes y por lo tanto varias componentes altamente conexas. Lo que implicaría que muchos usuarios no formaran parte de CFCs

Los datos que observamos, si es que tratamos de evaluarlos con los criterios que tenemos sobre grafos en general, podemos decir que por el número de arcos, que el grafo fundamental (sin direcciones) incluso suponemos que no hay arcos de ida y vuelta está mucho más cerca de un arbol (A = V - 1) que de un grafo completo (A = V^2). Lo que no nos entrega información para segurar nada pero nos permite intuir que los CFC podrían ser pocos y pequeños.

También notamos que el mostrador es el único medio que no sigue a nadie en la muestra y la tercera es sigue a muchos usuarios, los otros dos siguen al menos un usuario.

## Elección de estructuras:
### Usuarios (nodos V):
Antes de definir las estructuras y los algoritmos ya podíamos intuir que las operaciones que más se repetirían serian de búsqueda, es por esto que determinamos que la estructura más eficiente para almacenar los nodos o usuarios sería un unordered_map.

La siguiente discusión fue la elección de la llave para para el unordered_map. Fue necesario discriminar si convenía utilizar el User_ID o User_Name como clave.

Usar User_ID hacía necesario cambiar cada dato (O(E)) y para cada una realizar una búsqueda en el U-Map (O(V)) y reemplazarla (O(1)) y la ventaja se genera por la comparación de strings de un tamaño de 1 byte por character del nombre con una cantidad promedio por determinar v/s 8 bytes por ser de typo long long (trabajamos con arquitectura de 64 bits). Una vez escrito y ejecutado el código encontramos que el tamaño promedio de los strings era de 11.58, lo que genera una ventaja aunque no fue tan grande como esperabamos, sobre todo por que cuando lo planteamos esperábamos usar int pero esto no soportaba los datos.

Para el cálculo más grande que puede ser la determinación de distancias con BFS para cada nodo (O(V * E)), la diferencia podría no ser significativa ya que el costo de reemplazar (O(E)) + el costo del algoritmo suman asintóticamente lo mismo que no reemplazar. Pero si se genera una diferencia a en las constantes que acompañan las operaciones y debido a que el reemplazo se produce una sola vez y realizaremos varias operaciones (no solo el BFS) estimamos que esta diferencia sí tendrá efecto.

Para el resto de los datos, la primera desición que tomamos fue arbitrariamente ignorar los campos de twitts y fecha de creación ya que para todo el analisis de este trabajo no se utilizaron estos datos y no hacía sentido almacenar estos en la RAM mientras no se utilizan. 

Luego, analizamos si sería mejor trabajar con una tupla conociendo el orden de los campos, pero determinamos que un struct hace más fácil de leer y/o corregir el código, y además hace que podamos incorporar más adelante los campos que ahora estamos ignorando en caso de que queramos ampliar el estudio. Modificando solo el struct, la función de captura y la función en la que queramos incorporar o ampliar.

#### Diferencia de tupla v/s Struct: 
Consulta realizada en chatGPT que entregó el siguiente código: 

Para diferencias en tamaño hicimos el siguiente mini experimento:
``` cpp
#include <tuple>
#include <string>
#include <iostream>

struct Perfil {
    long long User_ID;
    std::string User_Name;
    // int num_tweets;
    int Friends;
    int Followers;
    // std::string Created_At;
    long long CFC;
};

int main() {
    std::tuple<int, std::string, /*int*/, int, int, /*std::string*/> arr_t[1000];
    Perfil arr_p[1000];

    std::cout << "Tamaño total del array de 1000 tuplas: " << sizeof(arr_t) << " bytes" << std::endl;
    std::cout << "Tamaño total del array de 1000 perfiles: " << sizeof(arr_p) << " bytes" << std::endl;
}
```
Y estas fuentes:

- https://en.cppreference.com/w/cpp/utility/tuple.html
- https://eel.is/c++draft/class.mem
- Prueba: el assembly generado (-O2 o -O3) es el mismo. Puedes verlo en Compiler Explorer.
- https://quick-bench.com/ 

El resultado es que no había diferencia significativa, pero los Struct eran más fáciles de trabajar

## Cálculos
### Cálculo de CFC:
Como mencionamos antes, decidimos utilizar el algoritmo de Kosaraju por su buen desempeño para digrafos grandes ya que asintóticamente tiene una complejidad temporal de O(V+E) ya que recorre una multiplo fijo (independiente del tamaño de los datos) de veces cada arista y cada vertice.
Una vez que encontramos las componentes fuertemente conexas decidimos que las utilizaríamos para calcular la tendencia de los usuarios. Segun lo conversado en clases las CFC suelen ser interpretadas como comunidades humanas.

Una vez determinado como calcularíamos e interpretaríamos las CFC pasamos a definir como usaríamos la información que el grafo nos entrega para determinar las tendencias políticas de los diferentes usuarios. El primer gran supuesto que hicimos es que las personas que son más populares en la red social suelen representar de buena manera la forma en que quieren leer los miembros de la comunidad. 

Dado que una sola persona (la más pupular) podría generar algun sesgo por algún tema incidental, por ejemplo que siga al medio de comunicación opuesto a su ideología decidimos disminuir ese tipo de anomalía estadística tomando una muestra de 5.

De forma análoga a la estructura Union-Find que utilizamos en clases para el algoritmo de Kruscal, utilizaremos un líder para identificar las diferentes CFC, pero en este caso particular utilizaremos el miembro con mayor influencia (número de seguidores) como líder.

## Análisis asintótico:
Como mensionamos anteriormente escogimos el algoritmo de Kosaraju ya que garantiza una complejidad temporal del algoritmo que es O(V + E). Y con esto entrega todos los CFC, es decir no se necesita repetir para encontrar cada uno de ellos, lo que es eficiente en este caso que notamos que habría un gran numero de ellos (si consideramos los de tamaño 1), ambas cosas que se verificaron experimentalmente con la ejecución del código.

### Cálculo de tendencia política:
La primera opción que discutimos es la posibilidad de que las particularidades de cada persona, que puede ser si participan mucho o poco en la red social, si siguen o no personas que piensan diferentes o suelen seguir solo a personas con la misma ideología, son detalles que son inconsistencias a las cuales se les puede disminuir utilizando grupos en lugar de individuos. Por lo que nuestra primera intuición fue utilizar la comunidad a la que pertenecen, es decir, su componente fuertemente conexa para determinar la corriente política del usuario.

Lo siguiente fue reconocer que todos los usuarios son particulares y distintos por lo que debíamos incorporar las decisiones que tomó cada usuario en su participación el twitter como a qué personas y/o medios de comunicación sigue. En este sentido decidimos incorporar una característica propia de un grafo dirigido que nos entrega mucha información de la relación o cercanía de un nodo con otro, que es la distancia en el gráfico, por lo que decidimos determinar una relacion inversamente proporcional pero relativa (a la todas las distancias). También nos encontramos con el caso particular en que toda la CFC podrían no tener camino a alguno de los medios, para esto incorporamos el caso en que no exista camino definiendo la tendencia como 0, y como el camino sería infinito se deja de considerar para el resto de cálculos: matemáticamente esto haría que la tendencía de cada uno de los otros fuera 100% lo que no hace sentido y además elimina la información de como la proporción de las diferentes tendencias políticas.

Finalmente tenemos el caso de que cada medio no tiene necesariamente un camino a si mismo, pero en teoría el resultado (lo comprobamos experimentalmente es infinito, por lo que, analizamos 3 opciones:
1. Crear el arco v-v forzando a que haya un camino siempre a si mismo de distancia 1
2. Modificar el código para el caso específico que de el valor infinito y asigne el 0
3. Finalmente la opcion que escogimos es calcular todos iguales y luego reemplazar los valores por los deseados para esos 4 usuarios

Elegimos incorporar ambos piezas de información en el cálculo de cada definitivo de las tendencias, la siguiente desición fue determinar como agregaríamos cada dato, vimos dos opciones que nos parecieron razónables, la primera fue establecer un promedio simple entre la tendencia calculada por distancias de usuario y la de su CFC, lo que podría generar porcentajes que sumen más de 100%, y luego ponderar para llegar a la suma 100. La segunda opción y la que elegimos de forma arbitraria fue entregar, en lugar de un valor un rango de valores, determinados por los limites de las dos opciones que se promediaban en el cálculo anterior. Escogimos esto ya que nos pareció que: por un lado, incorpora la información del cálculo anterior y por otro lado el tamaño del rango nos entrega información adicional que nos parece relevante, rangos más amplios informan que tan distantes estan las ideologías del usuario de la de su comunidad (CFC)

## Análisis asintótico:
El cálculo de las distancias a cada uno de los nodos de los medios de comunicación  es la parte de todo el proyecto que tiene una mayor complejidad temporal, el hecho de que se deba recorrer con BFS dos veces tiene una complejidad temporal de O(E) pero el se debe realizar para cada uno de los usuarios, por lo el algoritmo en su conjunto para obtener los datos para todos los usuarios de O(V * E) por lo que es el algoritmo u operación más costosa que pudimos encontrar. Lo que se ve claramente reflejado en el resultado experimental de los tiempos obtenidos.

La modificación de los datos de los medios de comunicación tiene tiempo constante (suponiendo que el numero de medios es fijo) e insignificante en comparación al resto del código.

Finalmente, el cálculo de la tendencia contextual tiene una complejidad para cáda módulo constante, ya que en el struct de los usuarios está almacenado el CFC al que pertenece, donde debemos hacer consultas en unordered_maps por lo que la complejidad temporal total de esta operación en su conjunto (para todos los usuarios) es de O(V)

### Problema encontrado para usuarios influyentes:
Durante el desarrollo del código, nos encontramos con una situación que no habíamos previsto de antemano: cuando itentamos imprimir los 10 usuarios más influyentes nos dimos cuenta de que el uso de unordered map en lugar de un vector para almacenar los usuarios. Hacía necesario copiar todos los datos en otra estructura que permita ordenar, por ejemplo un vector y luego realizar un ordenamiento cada vez que quisieramos encontrar un top de usuarios, lo que eliminaba la ventaja de elegir esa un unordered_map

En lugar de esto, encontramos una solucion diferente, hicimos un cambio en el que creamos una función que nos permite conocer el los top k usuarios, generalizada en k para poser usarla en el digrafo completo pero también en cada CFC para encontrar el líderes y el conjunto de líderes internos que necesitabamos para el resto de cálculos.

Esto genera una mejora en terminos temporales de O(E log E) a O(E log k) y como k es constante esto es O(E) que es recorrer solo una vez todos los usuarios (E) para encontrar el top K. 

# Resultados:
## Ambiente de trabajo
Se realizaron los experimentos en un computador con windows 11 corriendo WSL2 con un procesadro ARM 64 bits y con las bases de datos entregadas

## Tiempos de lecturas de datos
- Tiempo de carga del archivo de usuarios: 70 ms
- Tiempo de lectura de conexiones: 52 ms
- Tiempo de conversión de User_Name a User_ID: 45 ms
- Tiempo total para encontrar las CFCs: 190 ms

## Resultados iniciales de los datos
- Total de CFCs encontradas: 39230
- CFCs de tamaño 1: 39214
- CFCs de tamaño > 1: 16

## Tiempo de procesamiento de los datos
- Tiempo total en calcular ideologías directas: 78999 ms
- Tiempo total en calcular tendencias ideológicas de las CFCs: 22 ms
- Tiempo total en calcular ideología contextual: 57 ms
- Archivo 'ideología_contextual.csv' generado exitosamente en 102 ms
- Archivo 'ideología_directa.csv' generado exitosamente en 75 ms
- Archivo 'CFCs.csv' generado exitosamente en 89 ms

# Conclusiones:
## Resultados de los experimentos
Con respecto a los resultados de la ejecución del código, pudimos comprobar la complejidad temporal de lectura de nuestro unordered_map de usuarios, esto se ve por que el tiempo de convertir es muy cercano al tiempo de carga de las conexiones (O(E)). 

También pudimos comprobar la eficiencia temporal de Kosaraju (O(V + E)) que tuvo un tiempo algo mayor que la suma de capturar cada uno (O(E) + O(V))

Para el cálculo de distancias para establecer las ideologías notamos claramente como afecta un solo cálculo al tiempo de ejecución de todo el proyecto por tratarse de O(V * E)

Nos llamó poderosamente la atención la enorme cantidad, en primer lugar de usuarios que no fueron asignados a ninguna CFC y, en segundo lugar, el numero de usuarios sin tendencia política asignada.

## Conclusiones finales
Una vez observados los resultados podemos concluir: 
### Con respecto a los resultados de ideología política:
Por un lado, nos parece que el análisis de lo que podría significar la relación entre datos de entrada y los obtenidos puede seguir teniendo sentido con respecto a la ideología política de los usuarios. Para poder mejorar los resultados, en el sentido de entregar más información, el dataset que nos podría permitir encontrar estas relaciones con los supuestos y cálculos utilizados debería ser mucho más centrados en las conexiones o arcos en relación a la cantidad de usuarios. 

Formalizando, deberíamos elegir un dataset con una cantidad de arcos lo suficientemente grande en relación a la cantidad de vértices que nos entregue una probabilidad alta de que para particiones medianas del grafo, los d-(D')>0 para todo D' de modo de garantizar componentes fuertemente conexas. La determinación de ese tamaño de subgrafo, de la probabilidad y por tanto del cálculo de esa proporción, escapan al alcance de este trabajo y del curso. Pero de forma arbitraría, me gustaría intentar, por ejemplo, con un grafo de 10.000 usuarios y 300.000 a ver como varía la cantidad de CFCs y de resultados de ideología distintas de 0.


### Con respecto a los resultados de las estructuras, algoritmos y código
Creemos que las respuestas a los cuestionamientos, como la elección de Structs sobre tuplas, el uso de U-Maps y U-Sets en varias partes del código cuando las operaciones consistirían principalmente en buscar, fueron acertadas. Su impacto fue siempre positivo y nos entregan lineamientos y enseñanzas sobre cuando es conveniente usar estas estructuras, pero creemos que su impacto total global fue limitado.

La elección de Kosaraju para el CFC también fue acertado ya que dentro del desempeño global del código el tiempo de ejecución fue razónable. Y nos entregó sin problemás una de las respuestas solicitadas por el enunciado.

Para la determinación de los elementos más influyentes y más influenciables, nos hizo generar un aprendizaje importante, como unordered_map era recomendable para las búsquedas por Indice. Como la estructura no nos permitía ordenar (O(V log V)) creamos la funcion encontrar los K usuarios mayores, de tiempo lineal (O(v)), mejor que la idea que teníamos de ordenar y recuperar los primeros valores.

Finalmente y más importante de acuerdo a los tiempos totales, nos parece que el encontrar los caminos más cortos a los diferentes medios de comunicación fue tenía un costo temporal muy alto, y durante la redacción de estas últimás lineas encontramos una manera que podría ser más eficiente pero no tuvimos tiempo de probarla. Que sería generar un grafo compacto, en el que reemplazamos los arcos a cualquier miembro de la CFC por arcos directos al líder y así reducir el tamaño del grafo y mejorar el resultado.

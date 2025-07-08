# FEDA_2025
Proyecto Final FEDA 2025

Descripción

En este proyecto nos enfocaremos en el análisis de la red social Twitter. En particular, estudiaremos un subconjunto de los seguidores de los medios de comunicación Cooperativa (@Cooperativa), El Mostrador (@elmostrador), Soy Valdivia (@soyvaldiviacl) y La Tercera (@latercera). Visto como una red, Twitter tiene enlaces dirigidos entre cuentas. De esta manera, si un usuario A es seguidor de un usuario B en Twitter, lo recíproco, que B sea seguidor de A, no es necesariamente cierto.

Cada uno de los 4 medios de comunicación tiene una tendencia política reportada en la publicación científica "On the nature of real and perceived bias in the mainstream media". De esta manera,

Cooperativa es reportada como un medio de izquierda
El Mostrador es reportado como un medio libertario
Soy Valdivia es reportado como un medio de derecha
La Tercera es reportado como un medio de centro
La principal interrogante que deberá responder con su proyecto es ¿qué tendencia política tiene un usuario en Twitter? Para esto trabajaremos con la siguiente simplificación: Si el usuario A es seguidor de un medio de comunicación con tendencia política X, entonces diremos que el usuario A comparte la misma tendencia política X. En el caso que el usuario A sea seguidor de 2 medios de comunicación con tendencias políticas X e Y, respectivamente, se puede reportar que el usuario A tiene una tendencia política mixta (p% tendencia X y (1-p)% tendencia Y). El mismo criterio se utiliza cuando es seguidor de más de 2 medios de comunicación con tendencia políticas distintas.

De manera más precisa, para este proyecto se pide lo siguiente:

Reportar todas las componentes fuertemente conexas presentes en la red  (ver dataset). Estas componentes pueden ser reportadas por medio de tablas, gráficas o alguna otra manera que el alumno considere adecuada.
Reportar los usuarios más influyentes y los más influenciables, entendiendo como influyente aquel que es seguido por muchos usuarios, mientras que influenciable es aquel que sigue a muchos usuarios. Reportar un ranking con los top 10 de cada categoría.
Asignar una tendencia política a cada usuario de Twitter. Se debe reportar el porcentaje de cercanía a las tendencias de izquierda, libertario, derecha y centro, representadas por los 4 medios de comunicación Cooperativa, El mostrador, Soy Valdivia y La tercera. En este punto tambien se pide definir su propia fórmula para calcular dichos porcentajes, la cual debe considerar al menos uno de los dos puntos anteriores (componentes fuermente conexas y/o usuarios más influyentes-influenciables)
 

Se debe entregar un reporte con las siguientes secciones:

Introducción: En esta sección se espera que se contextualice el trabajo, explicando de manera acotada el problema planteado y los principales descubrimientos.
Algoritmos y estructuras de datos utilizadas: En esta sección se deben describir las estructuras de datos utilizadas en la realización de su proyecto, así como los algoritmos que usan dicha estructura. Detalle el tipo de estructuras, las operaciones utilizadas, complejidades de espacio y tiempo, además de una justificación clara para la elección de las estructuras y algoritmos. Utilice figuras para explicar las propiedades más releventes de su elección.
Implementación: Otra sección importante es la dedicada a la implementación de su solución. En esta sección se deben incorporar todos los detalles de la implementación, como la fórmula usada para definir los porcentajes de tendencia política. Si se utilizó código fuente proveniente de libros, sitios Web, bibliotecas, etc, debe ser citado de manera correcta. Además, se debe incluir un enlace para descargar el código fuente (idealmente, de un repositorio de Github). Nota: El lenguaje de programación que deben utilizar es C++. Es posible que utilicen otros lenguajes de programación como apoyo al proyecto principal (ej, para generación de gráficas, ejecución de experimentos, etc)
Experimentación y Discusión: Incluir una sección experimental donde se evalúe la implementación con el dataset entregado. Se debe reportar el espacio de la estructura de datos, además de los tiempos de ejecución promedio para los distintos métodos utilizados. Además, se deben discutir los principales hallazgos: top 10 de usuarios influyentes/influenciables, tendencia política de los miembros de la red, componentes conexas, entre otros hallazgos que el alumno estime necesarios. Se espera una discusión extensa en cada punto.
Conclusiones: En esta sección de cierre se deben incluir conclusiones adecuadas para lo realizado en el proyecto. Se deben considerar potenciales mejoras y limitaciones de la estructura y su implementación, así como del estudio mismo y su significancia.
 

Dataset

Descargar dataset: dataset_twitter.zip 

Para este proyecto utilizaremos un dataset extraído con la biblioteca de Python Tweepy.  El dataset es un subconjunto de la red de Twitter, la cual consta de 41.858 usuarios, con 83.073 relaciones entre usuarios. Este dataset está compuesto por los siguientes dos archivos:

twitter_users.csv: Archivo CSV (con valores separados por ';') que contiene la información de los usuarios de la red. La siguiente tabla muestra un ejemplo del contenido del archivo
User ID	User Name	Number Tweets	Friends Count	Followers Count	Created At
7668952	Cooperativa	1216687	669836	3032942	Mon Jul 23 22:31:14 +0000 2007
239534277	soyvaldiviacl	248530	4486	16658	Mon Jan 17 21:09:03 +0000 2011
3222731	latercera	993420	220544	1985664	Mon Apr 02 15:22:14 +0000 2007
12815132	elmostrador	263011	179	1811157	Tue Jan 29 04:20:54 +0000 2008
Donde los campos tienen el siguiente significado:

User ID: Número identificador del usuario en Twitter. Este campo es único.
User Name: Nombre de usuario en Twitter. Este campo es único.
Number Tweets: Número de tweets publicados por el usuario.
Friends Count: Número de cuentas que el usuario sigue en toda la red social. Note que este valor puede ser mucho mayor a la cantidad de cuentas que sigue el usuario en el dataset que se usará para el proyecto.
Followers Count: Número de seguidores que tiene el usuario en toda la red social. Note que este valor puede ser mucho mayor a la cantidad followers presentes en el dataset que se usará para el proyecto.
Created At: Fecha en la que el usuario creó su cuenta en la red social.
 

twitter_connections.csv: Archivo CSV (con valores separados por ';') que contiene la información de las relaciones entre los usuarios. La siguiente tabla muestra un ejemplo de su contenido
Followee	Follower
Cooperativa	Lucas46613336
Cooperativa	maggyrubilar
Cooperativa	matias_witt
Cooperativa	ErnaParra
Donde los campos tienen el siguiente significado:

Followee: Usuario que es seguido por otro usuario.
Follower: Usuario que sigue a otro usuario
Por ejemplo, en el caso (Cooperativa,ErnaParra) quiere decir que el usuario ErnaParra sigue al usuario Cooperativa en la red social.

 

Grupos

Este proyecto se desarrollará en grupos de 2 personas. Los grupos serán formados por los estudiantes. Es responsabilidad de los mismos estudiantes registrar su grupo por medio de un email a los profesores del curso.

 

Presentaciones

El proyecto considera presentaciones durante el mes de Julio. Las presentaciones serán de nivel técnico, en las que se espera que los alumnos expliquen y defiendan sus soluciones a bajo nivel. La presentación tendrá una duración de 15 minutos + 5 minutos de preguntas.

 

Plantilla para el informe

El formato de entrega del informe será en PDF. Para ello, se pide que los alumnos escriban su informe en LaTeX, usando alguna planilla disponible de manera online. A continuación se listan 4 ejemplos de plantillas:

https://es.overleaf.com/latex/templates/association-for-computing-machinery-acm-large-1-column-format-template/fsyrjmfzcwyy
https://es.overleaf.com/latex/templates/association-for-computing-machinery-acm-large-2-column-format-template/qwcgpbmkkvpq
https://es.overleaf.com/latex/templates/association-for-computing-machinery-acm-sigplan-proceedings-template/rfvsrhgmghtc
https://es.overleaf.com/latex/templates/association-for-computing-machinery-acm-sig-conference-proceedings-with-small-format-template/kxbkrwxkdgdh

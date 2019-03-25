# ¿Cómo implementar el algoritmo de Dijkstra en C++?

En este tutorial aprenderemos a implementar el algoritmo de Dijkstra para hallar camino de costo mínimo en un grafo dirigido con costos no-negativos.

## El grafo

Antes de comenzar, tenemos que implementar una estructura de datos adecuada para almacenar la información de nuestro grafo. En general, nos va a interesar almacenar el grafo en una lista de adyacencias, ya que el mismo será seguramente disperso (tal que tenga mayor sentido preguntar por caminos de costo mínimo).

### El tamaño del grafo

Solemos mantener el tamaño máximo del grafo como una constante `MAX_N`. Aunque es posible crear un grafo que crezca/decrezca dinámicamente con cada caso, las peticiones/liberaciones de memoria consumen tiempo de procesamiento.

```
#define MAX_N 1000
```

En el ejemplo anterior definimos `MAX_N` como 1000, pero puede ser el valor que convenga dependiendo del problema a resolver.

### Lista de adyacencias

Para implementar una lista de adyacencia usaremos el tipo de datos `vector` que permite almacenar arreglos de tamaño variable (para representar los adyacentes de cada nodo). En total tendremos `MAX_N` vectores, uno por cada potencial nodo. Además, debemos mantener la cantidad actual de nodos `N`.

```
#include <vector>
...
vector<int> adyacentes[MAX_N];
int N;
```

De esta forma, `adyacentes[i]` tendrá el vector con los índices correspondientes a todos los nodos adyacentes al nodo con índice `i`.

#### Lista de adyacencias _tuneada_

La lista de adyacencias presentada establece quién se conecta con quién, pero no da información alguna sobre los costos. Para resolver este problema, crearemos un nuevo tipo de datos que representa a un arco. Este arco tendrá un nodo destino y un costo asociado.

```
struct arco {
   int destino;
   double costo;
};
```

En el código anterior definimos el costo como un `double`, pero puede ser del tipo que mejor convenga para resolver el problema.

Ahora, podemos crear una mejor representación para el grafo.

```
vector<arco> adyacentes[MAX_N];
int N;
```

De esta forma, `adyacentes[i]` tendrá el vector con los arcos `a` correspondientes a todos los nodos adyacentes al nodo con índice `i`, donde `a.destino` tendrá el índice del nodo destino y `a.arco` tendrá el costo asociado.

### Inicializar el grafo

Para inicializar el grafo, basta con recorrer cada uno de los vectores que representan a los adyacentes y borrar su contenido. Esto se puede hacer para los primeros `N` vectores.

```
void limpiar_grafo() {
	for (int i=0; i<N; i++)
		adyacentes[i].clear();
}
```

_Nota: Si las restricciones de memoria son grandes, podría ser conveniente borrar todos los adyacentes (hasta `MAX_N`)_

## Lectura del grafo

Las entradas para problemas de grafos vienen en muchas variedades. A continuación veremos una posible forma de leer la entrada y almacenarla en nuestra estructura de datos.

### Estructura del archivo

El archivo comenzará con dos números enteros: `N` y `M`, representando la cantidad de nodos y la cantidad de conexiones, respectivamente.

Le seguirán `M` lineas, donde cada línea tendrá tres números: `origen`, `destino` y `costo`, representando a una conexión que parte desde el nodo con índice `origin` y llega hasta el nodo con índice `destino`, con costo `costo`.

Por ejemplo:

```
5 6
0 1 1.0
0 2 1.0
1 3 2.0
2 3 1.0
3 4 3.0
0 4 2.0
```

### Procesamiento del archivo

La lectura de la entrada puede hacerse mediante el uso de el _stream_ de entrada `cin`, para lo cual tenemos que importar la librería `<iostream>`.

```
#include<iostream>
...
void leer_grafo() {
	int M;
	cin >> N >> M;
	while (M--) {
		int origen;
		arco nuevo_arco;
		cin >> origen >> nuevo_arco.destino >> nuevo_arco.costo;
		adyacentes[origen].push_back(nuevo_arco);
	}
}
```

Nótese que la cantidad de conexiones `M` es declarada como variable local del procedimiento, ya que su valor no es relevante al salir del mismo.

Por esta misma razón es que podemos hacer `while (M--)`. Al salir del ciclo el valor de `M` será 0 _(cero)_ y su valor original de habrá perdido.

## Dijkstra

La implementación del algoritmo de Dijkstra involucra una cola de prioridades. Para esto usaremos un `priority_queue`, que forma para de la librería `<queue>`.

Las colas de prioridades en C++ reciben elementos cuyo tipo es ordenable. Para esto, construyamos un nuevo tipo de datos que represente la información que necesita el algoritmo.

### Estructura para almacenar información

```
struct info {
	int nodo;
	double costo_acumulado;
};
```

También podría sernos de utilizad una función que dado un nodo y un costo, construya un valor de tipo `info`. Crearemos una función con este objetivo en mente:

```
info construir_info(int nodo, double costo_acumulado) {
	info nueva_info;
	nueva_info.nodo = nodo;
	nueva_info.costo_acumulado = costo_acumulado;
	return nueva_info;
}
```

También podemos hacer esto al estilo c++ usando un constructor para el tipo `info`.

```
struct info {
	int nodo;
	double costo_acumulado;
	info(int nodo, double costo_acumulado) :
		nodo(nodo),
		costo_acumulado(costo_acumulado) {}
};
```

Ahora debemos plantear un orden sobre elementos de este nuevo tipo, para que la cola de prioridades pueda manejar correctamente la información suministrada.

Hay varias formas de hacer esto en C++. La más sencilla es crear una estructura que englobe una función comparadora con el comportamiento deseado. La función debe ser implementada de tal forma que evalúe en _true_ cuando `segundo` deba tener  más prioridad que `primero`.

```
struct comparador_para_info {
	bool operator()(info const& primero, info const& segundo) {
		return primero.costo_acumulado > segundo.costo_acumulado;
	}
};
```
Esta sintaxis es lo que se conoce como una _"sobrecarga de operadores"_ y permite reescribir el comportamiento de un operador en particular. En este caso, modificamos el operador de evaluación de funciones, pero se pueden sobrecargar otros operadores.

### Implementando el algoritmo

Una vez tenemos nuestro tipo de datos `info` con su orden definido, podemos comenzar a implementar nuestro algoritmo. Crearemos una función `dijkstra` que devuelva el costo del camino de costo mínimo entre el nodo con índice `desde`, hasta el nodo con índice `hasta`.

Además, declararemos un arreglo de visitados que nos será util para llevar el control en nuestro algoritmo.

```
...
bool visitado[MAX_N];

double dijkstra(int desde, int hasta) {
	...
```

En primer lugar hay que declarar la cola de prioridades e insertar el primer elemento (el nodo `desde`, con costo acumulado 0).

```
	...
	priority_queue<info, vector<info>, comparador_para_info> cola;
	cola.push(info(desde, 0));
	...
```

Nótese que los parámetros para el tipo de `priority_queue` incluyen tres cosas:

* El tipo de los elementos en la cola.
* El tipo del contenedor usado para representar el _heap_ internamente.
* El comparador a ser usado para la cola.

Si los últimos dos parámetros no son pasados, por defecto toma `vector<T>`, si la cola de prioridades es para elementos de algún tipo `T` y como comparador usa el orden natural del tipo (colocando los elementos mayores con mayor prioridad).

Ahora debemos reinicializar el arreglo de visitados, asegurando que todas las posiciones están en `false`.

```
	...
	memset(visitado, false, sizeof(visitado));
	...
```

_Nota: Recuerde que para usar la función `memset` debe importar la librería `<cstring>`._

Ahora debemos mantenernos ciclando mientras hayan elementos en la cola de prioridades, inspeccionando aquel que siga en orden de prioridad.

```
	...
	while (!cola.empty()) {
		info actual = cola.top();
		cola.pop();
		...
```

Nótese que `cola.top()` consulta el elemento siguiente en la cola de prioridades, mientras que `cola.pop()` lo elimina de la cola.

Si el nodo actual ya fue visitado, entonces no hay por qué seguir indagando con ese nodo. De lo contrario, se marca como visitado.

```
		...
		if (visitado[actual.nodo])
			continue;
		visitados[actual.nodo] = true;
		...
```

Si el nodo actual es el nodo al que se quería llegar (el nodo con índice `hasta`) entonces se devuelve el costo encontrado.

```
		...
		if (actual.nodo == hasta)
			return actual.costo_acumulado;
		...
```

De lo contrario se investigan todos los adyacentes del nodo actual y se agregan a la cola de prioridades. Nótese que el costo acumulado para un adyacente del nodo actual es el costo acumulado para el nodo actual más el costo que toma moverse desde el actual hasta el adyacente en cuestión.

```
		...
		for (int i=0; i<adyacentes[actual.nodo].size(); i++) {
			arco siguiente = adyacentes[actual.nodo][i];
			cola.push(info(siguiente.destino,
			               actual.costo_acumulado + siguiente.costo));
		}
		...
```

Al final del ciclo, si no se encuentra llegar hasta el nodo destino, entonces debemos reportar un costo infinito.

```
		...
	}
	return numeric_limits<double>::infinity();
}
```

Para poder usar este valor especial, hay que importar la librería `<limits>`.

## Probando la implementación

Podemos probar nuestra implementación, extendiendo la estructura de nuestro archivo de entrada.

Después de la entrada ya expuesta, vendrá un número `Q`, representando la cantidad de consultas a realizarse.

Le seguirán `Q` lineas, cada uno con dos valores `desde` y `hasta`. Para cada uno de estos valores vamos a querer conocer el costo del camino de costo mínimo entre el nodo con índice `desde` y el nodo con índice `hasta`.

Además, podemos agregar un valor `T` al inicio del archivo que represente la cantidad de casos de prueba a tratar.

Por ejemplo:

```
1
5 6
0 1 1.0
0 2 1.0
1 3 2.0
2 3 1.0
3 4 3.0
0 4 2.0
3
0 3
3 1
2 3
```

### Cuerpo principal del programa

Implementamos ahora un `main` que una todo y realice las consultas propuestas:

```
int main() {
	int T;
	cin >> T;
	while (T--) {
		leer_grafo();
		int Q;
		cin >> Q;
		while (Q--) {
			int desde, hasta;
			cin >> desde >> hasta;
			cout << dijkstra(desde, hasta) << endl;
		}
		limpiar_grafo();
	}
	return 0;
}
```

### Salida esperada

Si se ejecuta el programa con la entrada propuesta, la salida esperada ha de ser la siguente.

```
2
inf
1
```

## Ejecutando el programa

Para ejecutar el programa, basta con descargarse los archivos `dijkstra.cpp` y `dijkstra.in` del repositorio en donde se encuentra este tutorial.

Luego, desde la consola, navegar hasta la carpeta en donde se descargaran.

Para compilar el programa, puede hacer:

```
make dijkstra
```

Para ejecutar el programa, puede hacer:

```
./dijkstra < dijkstra.in
```

## Ejercicios recomendados en SPOJ

A continuación algunos ejercicios con los que pueden practicar aplicar Dijkstra a problemas particulares.

### Ejercicios de práctica

* Easy Dijkstra Problem: [EZDIJKST](https://www.spoj.com/problems/EZDIJKST/)
* Mice and Maze: [MICEMAZE](https://www.spoj.com/problems/MICEMAZE/)

### Ejercicios _interesantes_

* Almost Shortest Path: [SAMER08A](https://www.spoj.com/problems/SAMER08A/)
* Magical Bridges: [AMR11F](https://www.spoj.com/problems/AMR11F/)